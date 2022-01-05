#pragma once

#include "type.h"

namespace FS::Internal {

	template<typename... Data>
	class Manager final {
	public:
		FluidumUtils_Class_Default_ConDestructor(Manager)
			FluidumUtils_Class_Delete_CopyMove(Manager)

	private:
		using DataTuple = std::tuple<Data...>;

		template<FU::Concept::IsTuple RequiredData>
		using DataIndices = FU::Tuple::SameTupleTypeIndices<DataTuple, RequiredData>::Sequence;

		//unique number
		using ClassCode = ::FU::Class::ClassCode;

	private:
		using SceneUniquePtr = std::unique_ptr<SceneBase<Data...>>;

		FD::Manager<Data...> data{};

		std::vector<SceneUniquePtr> scenes = {};

		//current scenes
		std::vector<ClassCode::CodeType> sceneCodes = {};

		std::vector<ClassCode::CodeType> deleteSceneCodes = {};

		//sync
		std::mutex mtx{};
		std::mutex sceneMtx{};
		const std::thread::id mainThreadID = std::this_thread::get_id();

		//callbacks
		CallBackType addCallback = [](bool, FU::Class::ClassCode::CodeType) {};
		CallBackType deleteCallback = [](bool, FU::Class::ClassCode::CodeType) {};

		struct Async final {
		public:
			Async(const ClassCode::CodeType code, SceneUniquePtr&& scene)
				:code(code), scene(std::move(scene))
			{}

			~Async() = default;

			void async() {
				th = std::jthread(&Async::call, this);
			}
			FluidumUtils_Class_Delete_Copy(Async)
				FluidumUtils_Class_Default_Move(Async)
		public:
			void requestStop() noexcept {
				th.request_stop();
			}
			void join() {
				th.join();
			}

		public:
			std::atomic_bool finish = false;
			ClassCode::CodeType code;
		private:
			//Guaranteed to be destroyed by Destructor in the order of std::jthread->SceneUniquePtr
			//If it is reversed, the SceneUniquePtr is accessed in the call function even though it is destroyed, resulting in undefined behavior.
			SceneUniquePtr scene;
			std::jthread th{};

		private:
			void call() {
				auto token = th.get_stop_token();
				while (!token.stop_requested()) {
					this->scene->call();
				}
				this->finish = true;
			}
		};

		std::list<std::unique_ptr<Async>> asyncs{};

	private:
		void call() {
			this->deleteScenes();
			this->deleteAsyncScenes();

			std::lock_guard<std::mutex> sceneLock(sceneMtx);
			for (Size i = 0, size = scenes.size(); i < size; i++) {
				scenes[i]->call();
			}
		}

	public:

		template<IsSceneAble<Data...> Scene, typename... Arg>
		void addScene(Arg&&... arg) {
			std::unique_lock<std::mutex> lock(this->mtx);

			this->checkAlreadyAdded<Scene>();

			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//if this function call in the constructor -> dead lock
			lock.unlock();
			auto ptr = this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
			lock.lock();
			try {
				std::unique_lock<std::mutex> sceneLock(sceneMtx, std::defer_lock);
				if (mainThreadID != std::this_thread::get_id())
					sceneLock.lock();
				this->scenes.emplace_back(std::move(ptr));
			}
			catch (const std::exception&) {
				this->sceneCodes.pop_back();
				std::rethrow_exception(std::current_exception());
			}


			this->sceneCodes.emplace_back(classCode);

			this->addCallback(false, classCode);
		}


		template<IsSceneAble<Data...> Scene, typename... Arg>
		void addAsyncScene(Arg&&... arg) {
			std::unique_lock<std::mutex> lock(this->mtx);

			this->checkAlreadyAdded<Scene>();

			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();


			lock.unlock();
			auto ptr = this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
			lock.lock();

			try {
				this->asyncs.emplace_back(std::make_unique<Async>(classCode, std::move(ptr)));
			}
			catch (const std::exception&) {
				this->asyncs.pop_back();
				std::rethrow_exception(std::current_exception());
			}
			this->asyncs.back()->async();

			this->addCallback(true, classCode);
		}


		//async scene -> deleteAsyncScene
		template<IsSceneAble<Data...> Scene>
		void addDeleteCode() {
			std::lock_guard<std::mutex> lock(this->mtx);
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			{
				auto result = std::find(sceneCodes.cbegin(), sceneCodes.cend(), classCode);
				if (result == sceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyDeleted<Scene>();
			}

			{
				auto result = std::find(deleteSceneCodes.cbegin(), deleteSceneCodes.cend(), classCode);
				if (result != deleteSceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyDeleted<Scene>();
			}

			this->deleteSceneCodes.emplace_back(classCode);
		}

		template<IsSceneAble<Data...> Scene>
		void deleteAsyncScene() {
			std::lock_guard<std::mutex> lock(this->mtx);
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			auto result = std::find_if(asyncs.begin(), asyncs.end(), [=](const auto& x) {return x->code == classCode; });
			if (result == asyncs.end())
				::FS::Exception::Internal::throwAlreadyDeleted<Scene>();

			(*result)->requestStop();
		}

		template<IsSceneAble<Data...> Scene, typename... Arg>
		void callConstructor(Arg&&... arg) {
			this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
		}

		//set callback
		void setAddCallback(CallBackType callback) {
			std::lock_guard<std::mutex> lock(this->mtx);
			this->addCallback = callback;
		}
		void setDeleteCallback(CallBackType callback) {
			std::lock_guard<std::mutex> lock(this->mtx);
			this->deleteCallback = callback;
		}


	private:
		void deleteScenes() {
			std::unique_lock<std::mutex> lock(this->mtx);

			if (this->deleteSceneCodes.empty())
				return;

			for (Size i = 0, size = deleteSceneCodes.size(); i < size; i++) {

				//Find the code to delete from the current scene codes.
				auto itr = std::find(sceneCodes.cbegin(), sceneCodes.cend(), deleteSceneCodes[i]);
				auto distance = std::distance(sceneCodes.cbegin(), itr);

				//Requesting deletion by destructor is a deadlock, so release it once.
				std::unique_lock<std::mutex> sceneLock(sceneMtx);
				lock.unlock();
				this->scenes.erase(scenes.cbegin() + distance);
				lock.lock();
				sceneLock.unlock();

				this->sceneCodes.erase(itr);
				this->deleteCallback(false, deleteSceneCodes[i]);
			}

			//It's done.
			this->deleteSceneCodes.clear();
		}

		//Delete an async scene that has ended.
		void deleteAsyncScenes() {

			if (asyncs.empty())
				return;

			//removed class code. for callback.
			std::vector<ClassCode::CodeType> codes{};

			asyncs.remove_if(
				[&](auto& x) {
					bool result = x->finish;
					if (result)
						codes.emplace_back(x->code);
					return result;
				}
			);

			for (auto x : codes)
				this->deleteCallback(true, x);
		}



		template<IsSceneAble<Data...> Scene>
		void checkAlreadyAdded() const {
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//main
			{
				auto result = std::find(sceneCodes.cbegin(), sceneCodes.cend(), classCode);
				if (result != sceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyAdded<Scene>();
			}
			//async
			{
				auto result = std::find_if(asyncs.cbegin(), asyncs.cend(), [=](const auto& x) {return x->code == classCode; });
				if (result != asyncs.cend())
					::FS::Exception::Internal::throwAlreadyAdded<Scene>();
			}

		}

		template<IsSceneAble<Data...> Scene, typename... Arg>
		std::unique_ptr<SceneBase<Data...>> makeScenePtr(Arg&&... arg) {
			//Change the argument type of the "Constructor" function to tuple.
			using ArgTuple = FU::Tuple::FunctionArgsToTupleType<&Scene::Constructor>;

			static_assert(FU::Tuple::IsSameTypeInTuple<ArgTuple>() == false, "No same data types are allowed.");

			//Required data
			using RequiredDataIndices = DataIndices<ArgTuple>;

			auto makeScene = [&]<Size ...Index>(std::index_sequence<Index...>) {
				return std::make_unique<Scene>(data.get<Index>() ..., std::forward<Arg>(arg)...);
			};

			//make
			return makeScene(RequiredDataIndices());
		}

	private://friend
		friend class ::FS::FluidumScene<Data...>;
	};
}

