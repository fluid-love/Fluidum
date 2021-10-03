#pragma once

#include "manager.h"

namespace FS::Internal {

	template<typename...Data>
	std::unique_ptr<Manager<Data...>> GManager = nullptr;

}

namespace FS::Internal {

	//このクラスをpublic継承する
	template<typename... Data>
	class SceneBase {
	public:
		SceneBase() = default;//抽象クラスなので継承せずには作成不可
		virtual ~SceneBase() = default;
		FluidumUtils_Class_Delete_CopyMove(SceneBase)

	public:
		virtual void call() = 0;

	public:

		//シーンの追加
		//同期シーンが一周したら追加される．つまり次のループから呼ばれることになる．
		template<IsSceneAble<Data...> Scene, typename... Args>
		void addScene(Args&&... args) {
			GManager<Data...>->addScene<Scene>(std::forward<Args>(args)...);
		}

		//シーンの追加を試みる
		//既に作成されていれば，作成しない->return false
		template<IsSceneAble<Data...> Scene, typename... Args>
		bool tryAddScene(Args&&... args) {
			try {
				this->addScene<Scene>(std::forward<Args>(args)...);
			}
			catch (const ::FS::Exception::AlreadyAdded&) {
				return false;
			}
			return true;
		}

		//非同期シーンの追加
		//すぐに非同期として関数が呼ばれる
		template<IsSceneAble<Data...> Scene, typename... Args>
		void addAsyncScene(Args&&... args) {
			GManager<Data...>->addAsyncScene<Scene>(std::forward<Args>(args)...);
		}

		//シーンの削除
		//同期シーンの一周が終わったあとに消される
		//すぐに次の処理へと移る
		template<IsSceneAble<Data...> Scene>
		void deleteScene() {
			GManager<Data...>->addDeleteCode<Scene>();
		}

		//シーンの削除を試みる
		//すでに削除（削除されていない）場合は，削除しない->return false
		template<IsSceneAble<Data...> Scene>
		bool tryDeleteScene() {
			try {
				this->deleteScene<Scene>();
			}
			catch (const ::FS::Exception::AlreadyDeleted&) {
				return false;
			}
			return true;
		}

		//非同期シーンの削除
		template<IsSceneAble<Data...> Scene>
		void deleteAsyncScene() {
			GManager<Data...>->deleteAsyncScene<Scene>();
		}

	public://data

		template<FD::IsDataAble T>
		void makeData() {
			static_assert(std::is_pointer_v<T>);
			GManager<Data...>->data.makeData<T>();
		}

		template<FD::IsDataAble T>
		void deleteData() {
			static_assert(std::is_pointer_v<T>);
			GManager<Data...>->data.deleteData<T>();
		}

		//constなデータポインタを返す
		//変更したい場合はコンストラクタでデータを取得すること
		template<FD::IsDataAble T>
		_NODISCARD const auto* getData() {
			static_assert(std::is_pointer_v<T>);
			//pointerにしてunique_ptrを確認．違うのであれば戻す
			constexpr auto index = FU::Tuple::getSameTypeIndex<std::tuple<Data...>, T>();
			return GManager<Data...>->data.get<index>();
		}

		//dataが作成されているか
		template<FD::IsDataAble T>
		_NODISCARD bool isDataCreated() const {
			static_assert(std::is_pointer_v<T>);
			constexpr auto index = FU::Tuple::getSameTypeIndex<std::tuple<Data...>, T>();
			return GManager<Data...>->data.isDataCreated<index>();
		}

	};

}