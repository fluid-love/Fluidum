#pragma once

#include "using.h"

namespace FS {

	//Error if the same data type is created twice.
	//Don't use them as global variables. The initialization may fail.
	//Calling the constructor in the main thread.
	template<typename ...Data>
	class FluidumScene final {
	public:

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

		template<typename T>
		auto getData() {
			using DataTuple = std::tuple<Data...>;
			static_assert(FU::Tuple::IsSameTypeContain<DataTuple, T>());
			constexpr auto index = FU::Tuple::GetSameTypeIndex<DataTuple, T>();
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