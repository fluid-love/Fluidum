#pragma once

#include "../../Utils/include.h"

//Fluidum DataManager
#include "../Data/manager.h"

#include "exception.h"

//forward declaration 
namespace FS {

	template<typename ...Data>
	class FluidumScene;

	namespace Internal {
		template<typename... Data>
		class SceneBase;
	}
}

namespace FS::Internal {

	template<typename SceneClass, typename ...Data>
	concept IsSceneAble = std::derived_from<SceneClass, SceneBase<Data...>>;

}

namespace FS {

	//add and delete
	namespace Internal {
		using CallBackType = void(*)(bool, FU::Class::ClassCode::CodeType);
	}
	using Internal::CallBackType;

}