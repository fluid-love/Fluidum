#pragma once

#include "type.h"

namespace FS::Internal {

	template<typename... Data>
	class Manager final {
	public:
		//Destructor�ŃV�[���̏I����҂�
		FluidumUtils_Class_Default_ConDestructor(Manager)
			FluidumUtils_Class_Delete_CopyMove(Manager)

	private:
		using DataTuple = std::tuple<Data...>;

		//�K�v�ȃf�[�^��tuple�œn�����̃f�[�^��index���擾����
		template<FU::Concept::IsTuple RequiredData>
		using DataIndices = FU::Tuple::SameTupleTypeIndices<DataTuple, RequiredData>::Sequence;

		using ClassCode = ::FU::Class::ClassCode;

	private:
		using SceneUniquePtr = std::unique_ptr<SceneBase<Data...>>;

		FD::Manager<Data...> data{};

		std::vector<SceneUniquePtr> scenes = {};

		//���݊Ǘ����Ă���V�[����code��ۊǁDscene�̏d�������o
		std::vector<ClassCode::CodeType> sceneCodes = {};

		/*call���[�v���ō폜���s����index���v�f���𒴂���\�������邽��,
		  ��x�����N���X�R�[�h���L�^���Ēu���Č�ŏ����D*/
		std::vector<ClassCode::CodeType> deleteSceneCodes = {};

		std::mutex mtx{};
		std::mutex sceneMtx{};
		//constructor�̃X���b�h
		const std::thread::id mainThreadID = std::this_thread::get_id();

		//callbacks
		CallBackType addCallback = [](bool, FU::Class::ClassCode::CodeType) {};
		CallBackType deleteCallback = [](bool, FU::Class::ClassCode::CodeType) {};

		struct Async final {
		public:
			//code ���g�̃R�[�h 
			Async(const ClassCode::CodeType code, SceneUniquePtr&& scene)
				:code(code), scene(std::move(scene))
			{}

			//tsd::jthread��request_stop�̂���join����΂��
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
			//�K�i�ł�Destructor��std::jthread->SceneUniquePtr�̏��ԂŔj������邱�Ƃ��ۏ؂���Ă���
			//�t�ɂ��Ă��܂���SceneUniquePtr���j������Ă���ɂ������킸call�֐����ŃA�N�Z�X���Ă��܂�����`����ƂȂ�
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
			//callloop���ɗ��܂����폜�v��������
			//�I����Ă�񓯊��V�[�����݂�Ώ���
			this->deleteScenes();
			this->deleteAsyncScenes();

			std::lock_guard<std::mutex> sceneLock(sceneMtx);
			for (std::size_t i = 0, size = scenes.size(); i < size; i++) {
				scenes[i]->call();
			}
		}



	public:

		//�V�[���̒ǉ�
		template<IsSceneAble<Data...> Scene, typename... Arg>
		void addScene(Arg&&... arg) {
			std::unique_lock<std::mutex> lock(this->mtx);

			//�����V�[���̒ǉ����m�F
			this->checkAlreadyAdded<Scene>();

			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//�R���X�g���N�^�ŃV�[���̒ǉ�������Ɩ������b�N�ɂȂ��Ă��܂��̂�
			lock.unlock();
			auto ptr = this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
			lock.lock();
			try {
				//�l��
				//�񓯊��V�[������Ă΂���call�̃��[�v���ɓ���Ă��܂��̂�lock
				//�����V�[���ł͖������Ȃ���call��lock���������Ă���
				std::unique_lock<std::mutex> sceneLock(sceneMtx, std::defer_lock);
				if (mainThreadID != std::this_thread::get_id())
					sceneLock.lock();
				this->scenes.emplace_back(std::move(ptr));
			}
			catch (const std::exception&) {
				this->sceneCodes.pop_back();
				std::rethrow_exception(std::current_exception());
			}

			//�ǉ�����V�[���̃R�[�h��ǉ�
			//lock�������O�ɒǉ����Ă��܂��ƃR���X�g���N�^�ŃV�[���ǉ����ꂽ�Ƃ��ɏ���������
			this->sceneCodes.emplace_back(classCode);

			this->addCallback(false, classCode);
		}

		//�񓯊��V�[���̒ǉ�
		template<IsSceneAble<Data...> Scene, typename... Arg>
		void addAsyncScene(Arg&&... arg) {
			std::unique_lock<std::mutex> lock(this->mtx);

			////�����V�[���̒ǉ����m�F
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


		//call���[�v�����I�������ɂ��̃V�[���͍폜�����
		//�񓯊��V�[���͂��̊֐��ŏ����Ȃ�
		template<IsSceneAble<Data...> Scene>
		void addDeleteCode() {
			std::lock_guard<std::mutex> lock(this->mtx);
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//���݂̃V�[���ɋl�܂�Ă��Ȃ���΂Ȃ�Ȃ�
			{
				auto result = std::find(sceneCodes.cbegin(), sceneCodes.cend(), classCode);
				if (result == sceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyDeleted<Scene>();
			}

			//���ɏ����R�[�h�ɓ����R�[�h�������Ă͂����Ȃ�
			{
				auto result = std::find(deleteSceneCodes.cbegin(), deleteSceneCodes.cend(), classCode);
				if (result != deleteSceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyDeleted<Scene>();
			}

			this->deleteSceneCodes.emplace_back(classCode);
		}


		//�񓯊��V�[��������
		//�����񓯊��V�[���̏I����/�����V�[���̏I����Ƀ`�F�b�N�����ď���
		template<IsSceneAble<Data...> Scene>
		void deleteAsyncScene() {
			std::lock_guard<std::mutex> lock(this->mtx);
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//�ǂ��ɂ��邩������		
			auto result = std::find_if(asyncs.begin(), asyncs.end(), [=](const auto& x) {return x->code == classCode; });
			if (result == asyncs.end())
				::FS::Exception::Internal::throwAlreadyDeleted<Scene>();

			//����call���ʂ�����I���悤�ɖ���
			(*result)->requestStop();
		}


		//constructor���Ăяo���̂�
		template<IsSceneAble<Data...> Scene, typename... Arg>
		void callConstructor(Arg&&... arg) {
			this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
		}


		//callback���Z�b�g����
		void setAddCallback(CallBackType callback) {
			std::lock_guard<std::mutex> lock(this->mtx);
			this->addCallback = callback;
		}
		void setDeleteCallback(CallBackType callback) {
			std::lock_guard<std::mutex> lock(this->mtx);
			this->deleteCallback = callback;
		}


	private:
		//�ς܂�Ă����폜����R�[�h���Q�Ƃ��ăV�[�����폜
		void deleteScenes() {
			std::unique_lock<std::mutex> lock(this->mtx);

			//��Ȃ牽�����Ȃ�
			if (this->deleteSceneCodes.empty())
				return;

			//deleteSceneCodes�r���Ō��ɒǉ������\��������̂ł��Ȃ�
			//�r���Œǉ����ꂽ�̂��܂߂đS�������̂Ŗ���T�C�Y�̊m�F
			for (std::size_t i = 0; i < deleteSceneCodes.size(); i++) {

				//�폜����R�[�h�����݂̃V�[���R�[�h����T��
				auto itr = std::find(sceneCodes.cbegin(), sceneCodes.cend(), deleteSceneCodes[i]);
				auto distance = std::distance(sceneCodes.cbegin(), itr);

				//�f�X�g���N�^�ō폜��v������ƃf�b�h���b�N�Ȃ̂ł����������
				//addScene����΂ꂽ��A�N�Z�X�����̂Ń��b�N
				std::unique_lock<std::mutex> sceneLock(sceneMtx);
				lock.unlock();
				this->scenes.erase(scenes.cbegin() + distance);
				lock.lock();
				sceneLock.unlock();

				this->sceneCodes.erase(itr);
				this->deleteCallback(false, deleteSceneCodes[i]);
			}
			//�����Ă���
			this->deleteSceneCodes.clear();



		}

		//�I������񓯊��V�[��������
		void deleteAsyncScenes() {
			//�񓯊��V�[��
			if (asyncs.empty())
				return;

			//�������N���X�R�[�h
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

		//�V�[���̍쐬
		template<IsSceneAble<Data...> Scene, typename... Arg>
		std::unique_ptr<SceneBase<Data...>> makeScenePtr(Arg&&... arg) {
			//Constructor�֐��̈����̌^��tuple�ɂ���
			using ArgTuple = FU::Tuple::ClassFunctionArgsToTuple<&Scene::Constructor>::Type;

			//Constructor�̈����ɓ����^�̃f�[�^�͋֎~
			static_assert(FU::Tuple::isSameTypeInTuple<ArgTuple>() == false, "Constructor�֐��̈����ɓ����^�����Ȃ��ł��������D");

			//�K�v�ȃf�[�^��index
			using RequiredDataIndices = DataIndices<ArgTuple>;

			auto makeScene = [&]<std::size_t ...Index>(std::index_sequence<Index...>) {
				return std::make_unique<Scene>(data.get<Index>() ..., std::forward<Arg>(arg)...);
			};

			//�쐬
			return makeScene(RequiredDataIndices());
		}

	private://friend
		friend class ::FS::FluidumScene<Data...>;
	};
}

