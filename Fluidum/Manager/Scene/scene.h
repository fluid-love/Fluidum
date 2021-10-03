#pragma once

#include "using.h"

namespace FS {

	//同じデータ型で二回作成したらエラー
	//グローバル変数として使わないことDataの初期化に影響がでる
	//mainloopを行うスレッドでコンストラクタを呼ぶこと．
	template<typename ...Data>
	class FluidumScene final {
	public:
		//GManagerの初期化　DataのコンストラクタのためにGlobal変数の初期化を待つ
		FluidumScene() {
			if (!Internal::GManager<Data...>)
				Internal::GManager<Data...> = std::make_unique<Internal::Manager<Data...>>();
			else
				throw std::runtime_error("GManager is already maked.");
		}
		~FluidumScene() noexcept {
			if (Internal::GManager<Data...>)
				Internal::GManager<Data...>.reset();
			else {
				try {
					std::cerr << "GManager is already reset." << std::endl;
				}
				catch (...) {
					abort();
				}
			}
		}
		FluidumUtils_Class_Delete_CopyMove(FluidumScene)

	public:
		using SceneBaseType = SceneBase<Data...>;

	public:
		template<Internal::IsSceneAble<Data...> Scene, typename...Arg>
		void addScene(Arg&&... arg) {
			Internal::GManager<Data...>->addScene<Scene>(std::forward<Arg>(arg)...);
		}

		void call() {
			Internal::GManager<Data...>->call();
		}

		//dataを取得
		template<typename T>
		auto getData() {
			using DataTuple = std::tuple<Data...>;
			static_assert(FU::Tuple::isSameTypeContain<DataTuple, T>(), "FS::FluidumScene::getData 取得しようとしたデータの型がありません．");
			constexpr auto index = FU::Tuple::getSameTypeIndex<DataTuple, T>();
			return Internal::GManager<Data...>->data.get<index>();
		}

		//callback
		void setAddCallback(CallBackType callback) {
			Internal::GManager<Data...>->setAddCallback(callback);
		}
		void setDeleteCallback(CallBackType callback) {
			Internal::GManager<Data...>->setDeleteCallback(callback);
		}


	};



}