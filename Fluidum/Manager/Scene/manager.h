#pragma once

#include "type.h"

namespace FS::Internal {

	template<typename... Data>
	class Manager final {
	public:
		//Destructorでシーンの終了を待つ
		FluidumUtils_Class_Default_ConDestructor(Manager)
			FluidumUtils_Class_Delete_CopyMove(Manager)

	private:
		using DataTuple = std::tuple<Data...>;

		//必要なデータをtupleで渡しそのデータのindexを取得する
		template<FU::Concept::IsTuple RequiredData>
		using DataIndices = FU::Tuple::SameTupleTypeIndices<DataTuple, RequiredData>::Sequence;

		using ClassCode = ::FU::Class::ClassCode;

	private:
		using SceneUniquePtr = std::unique_ptr<SceneBase<Data...>>;

		FD::Manager<Data...> data{};

		std::vector<SceneUniquePtr> scenes = {};

		//現在管理しているシーンのcodeを保管．sceneの重複を検出
		std::vector<ClassCode::CodeType> sceneCodes = {};

		/*callループ中で削除を行うとindexが要素数を超える可能性があるため,
		  一度消すクラスコードを記録して置いて後で消す．*/
		std::vector<ClassCode::CodeType> deleteSceneCodes = {};

		std::mutex mtx{};
		std::mutex sceneMtx{};
		//constructorのスレッド
		const std::thread::id mainThreadID = std::this_thread::get_id();

		//callbacks
		CallBackType addCallback = [](bool, FU::Class::ClassCode::CodeType) {};
		CallBackType deleteCallback = [](bool, FU::Class::ClassCode::CodeType) {};

		struct Async final {
		public:
			//code 自身のコード 
			Async(const ClassCode::CodeType code, SceneUniquePtr&& scene)
				:code(code), scene(std::move(scene))
			{}

			//tsd::jthreadでrequest_stopのあとjoinがよばれる
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
			//規格ではDestructorでstd::jthread->SceneUniquePtrの順番で破棄されることが保証されている
			//逆にしてしまうとSceneUniquePtrが破棄されているにもかかわずcall関数内でアクセスしてしまい未定義動作となる
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
			//callloop中に溜まった削除要請を処理
			//終わってる非同期シーンが在れば消す
			this->deleteScenes();
			this->deleteAsyncScenes();

			std::lock_guard<std::mutex> sceneLock(sceneMtx);
			for (std::size_t i = 0, size = scenes.size(); i < size; i++) {
				scenes[i]->call();
			}
		}



	public:

		//シーンの追加
		template<IsSceneAble<Data...> Scene, typename... Arg>
		void addScene(Arg&&... arg) {
			std::unique_lock<std::mutex> lock(this->mtx);

			//同じシーンの追加を確認
			this->checkAlreadyAdded<Scene>();

			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//コンストラクタでシーンの追加があると無限ロックになってしまうので
			lock.unlock();
			auto ptr = this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
			lock.lock();
			try {
				//詰む
				//非同期シーンから呼ばれればcallのループ中に入れてしまうのでlock
				//同期シーンでは無視しないとcallでlockがかかっている
				std::unique_lock<std::mutex> sceneLock(sceneMtx, std::defer_lock);
				if (mainThreadID != std::this_thread::get_id())
					sceneLock.lock();
				this->scenes.emplace_back(std::move(ptr));
			}
			catch (const std::exception&) {
				this->sceneCodes.pop_back();
				std::rethrow_exception(std::current_exception());
			}

			//追加するシーンのコードを追加
			//lock解除より前に追加してしまうとコンストラクタでシーン追加されたときに順序が狂う
			this->sceneCodes.emplace_back(classCode);

			this->addCallback(false, classCode);
		}

		//非同期シーンの追加
		template<IsSceneAble<Data...> Scene, typename... Arg>
		void addAsyncScene(Arg&&... arg) {
			std::unique_lock<std::mutex> lock(this->mtx);

			////同じシーンの追加を確認
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


		//callループが一回終わった後にそのシーンは削除される
		//非同期シーンはこの関数で消せない
		template<IsSceneAble<Data...> Scene>
		void addDeleteCode() {
			std::lock_guard<std::mutex> lock(this->mtx);
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//現在のシーンに詰まれていなければならない
			{
				auto result = std::find(sceneCodes.cbegin(), sceneCodes.cend(), classCode);
				if (result == sceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyDeleted<Scene>();
			}

			//次に消すコードに同じコードがあってはいけない
			{
				auto result = std::find(deleteSceneCodes.cbegin(), deleteSceneCodes.cend(), classCode);
				if (result != deleteSceneCodes.cend())
					::FS::Exception::Internal::throwAlreadyDeleted<Scene>();
			}

			this->deleteSceneCodes.emplace_back(classCode);
		}


		//非同期シーンを消す
		//消す非同期シーンの終了後/同期シーンの終了後にチェックを入れて消す
		template<IsSceneAble<Data...> Scene>
		void deleteAsyncScene() {
			std::lock_guard<std::mutex> lock(this->mtx);
			constexpr ClassCode::CodeType classCode = ClassCode::GetClassCode<Scene>();

			//どこにあるかを検索		
			auto result = std::find_if(asyncs.begin(), asyncs.end(), [=](const auto& x) {return x->code == classCode; });
			if (result == asyncs.end())
				::FS::Exception::Internal::throwAlreadyDeleted<Scene>();

			//次にcallをぬけたら終わるように命令
			(*result)->requestStop();
		}


		//constructorを呼び出すのみ
		template<IsSceneAble<Data...> Scene, typename... Arg>
		void callConstructor(Arg&&... arg) {
			this->makeScenePtr<Scene>(std::forward<Arg>(arg)...);
		}


		//callbackをセットする
		void setAddCallback(CallBackType callback) {
			std::lock_guard<std::mutex> lock(this->mtx);
			this->addCallback = callback;
		}
		void setDeleteCallback(CallBackType callback) {
			std::lock_guard<std::mutex> lock(this->mtx);
			this->deleteCallback = callback;
		}


	private:
		//積まれていた削除するコードを参照してシーンを削除
		void deleteScenes() {
			std::unique_lock<std::mutex> lock(this->mtx);

			//空なら何もしない
			if (this->deleteSceneCodes.empty())
				return;

			//deleteSceneCodes途中で後ろに追加される可能性があるのでしない
			//途中で追加されたのも含めて全部消すので毎回サイズの確認
			for (std::size_t i = 0; i < deleteSceneCodes.size(); i++) {

				//削除するコードを現在のシーンコードから探す
				auto itr = std::find(sceneCodes.cbegin(), sceneCodes.cend(), deleteSceneCodes[i]);
				auto distance = std::distance(sceneCodes.cbegin(), itr);

				//デストラクタで削除を要請するとデッドロックなのでいったん解除
				//addSceneがよばれたらアクセスが被るのでロック
				std::unique_lock<std::mutex> sceneLock(sceneMtx);
				lock.unlock();
				this->scenes.erase(scenes.cbegin() + distance);
				lock.lock();
				sceneLock.unlock();

				this->sceneCodes.erase(itr);
				this->deleteCallback(false, deleteSceneCodes[i]);
			}
			//消しておく
			this->deleteSceneCodes.clear();



		}

		//終わった非同期シーンを消す
		void deleteAsyncScenes() {
			//非同期シーン
			if (asyncs.empty())
				return;

			//消したクラスコード
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

		//シーンの作成
		template<IsSceneAble<Data...> Scene, typename... Arg>
		std::unique_ptr<SceneBase<Data...>> makeScenePtr(Arg&&... arg) {
			//Constructor関数の引数の型をtupleにする
			using ArgTuple = FU::Tuple::ClassFunctionArgsToTuple<&Scene::Constructor>::Type;

			//Constructorの引数に同じ型のデータは禁止
			static_assert(FU::Tuple::isSameTypeInTuple<ArgTuple>() == false, "Constructor関数の引数に同じ型を入れないでください．");

			//必要なデータのindex
			using RequiredDataIndices = DataIndices<ArgTuple>;

			auto makeScene = [&]<std::size_t ...Index>(std::index_sequence<Index...>) {
				return std::make_unique<Scene>(data.get<Index>() ..., std::forward<Arg>(arg)...);
			};

			//作成
			return makeScene(RequiredDataIndices());
		}

	private://friend
		friend class ::FS::FluidumScene<Data...>;
	};
}

