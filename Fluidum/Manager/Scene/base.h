#pragma once

#include "manager.h"

namespace FS::Internal {

	template<typename...Data>
	std::unique_ptr<Manager<Data...>> GManager = nullptr;

}

namespace FS::Internal {

	//inherit this class
	//access-specifier: public
	template<typename... Data>
	class SceneBase {
	public:
		SceneBase() = default;//As an abstract class, it cannot be created without inheritance.
		virtual ~SceneBase() = default;
		FluidumUtils_Class_Delete_CopyMove(SceneBase);

	public:
		virtual void call() = 0;

	public:

		//Added scene is called from the next loop.
		template<IsSceneAble<Data...> Scene, typename... Args>
		void addScene(Args&&... args) {
			GManager<Data...>->addScene<Scene>(std::forward<Args>(args)...);
		}

		//If it has already been created, do not create it -> return false
		template<IsSceneAble<Data...> Scene, typename... Args>
		bool tryAddScene(Args&&... args) {
			try {
				this->addScene<Scene>(std::forward<Args>(args)...);
			}
			catch (const ::FS::Exception::Error&) {
				return false;
			}
			return true;
		}

		//Function is called immediately
		template<IsSceneAble<Data...> Scene, typename... Args>
		void addAsyncScene(Args&&... args) {
			GManager<Data...>->addAsyncScene<Scene>(std::forward<Args>(args)...);
		}

		//Scene is deleted after one round.
		template<IsSceneAble<Data...> Scene>
		void deleteScene() {
			GManager<Data...>->addDeleteCode<Scene>();
		}

		template<IsSceneAble<Data...> Scene>
		bool tryDeleteScene() {
			try {
				this->deleteScene<Scene>();
			}
			catch (const ::FS::Exception::Error&) {
				return false;
			}
			return true;
		}

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

		template<FD::IsDataAble T>
		[[nodiscard]] bool isDataCreated() const {
			static_assert(std::is_pointer_v<T>);
			constexpr auto index = FU::Tuple::GetSameTypeIndex<std::tuple<Data...>, T>();
			return GManager<Data...>->data.isDataCreated<index>();
		}

	public:
		template<IsSceneAble<Data...> Scene, typename... Args>
		void callConstructor(Args&&... args) {
			GManager<Data...>->callConstructor<Scene>(std::forward<Args>(args)...);
		}

		//Delete once and add again.
		/*
		Scenes are deleted at the end of the loop,
		so consecutive calls to deleteScene and addScene will result in an error.		
		*/
		//Calling it from the same scene can be dangerous because it may access the freed memory.
		template<IsSceneAble<Data...> Scene, typename... Args>
		void recreateScene(Args&&... args) {
			GManager<Data...>->recreate<Scene>(std::forward<Args>(args)...);
		}

	};

}