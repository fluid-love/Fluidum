#pragma once

#include "../../Utils/include.h"

//Fluidum DataManager
#include "../Data/manager.h"

#include "exception.h"
#include <list>

//前方宣言
namespace FS {

	template<typename ...Data>
	class FluidumScene;

	namespace Internal {

		template<typename... Data>
		class SceneBase;
	}
}

namespace FS::Internal {

	//SceneBaseを継承している必要がある && Constructor関数が必要
	template<typename SceneClass, typename ...Data>
	concept IsSceneAble = std::derived_from<SceneClass, SceneBase<Data...>>;


}

namespace FS {
	namespace Internal {
		using CallBackType = void(*)(bool, FU::Class::ClassCode::CodeType);
	}
	using Internal::CallBackType;
}