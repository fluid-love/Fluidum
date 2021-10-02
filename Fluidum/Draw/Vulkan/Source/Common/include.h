#pragma once

//FluidumVK
#include "FVKtype.h"

//FluidumKeyManager
#include <FKMinclude.h>

//disable warning push
FluidumUtils_Debug_BeginDisableAllWarning

//glfw
#include <GLFW/glfw3.h>

//imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
 
//glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include "../../library/glm/glm/glm.hpp"
#include "../../library/glm/glm/gtc/matrix_transform.hpp"
#include "../../library/glm/glm/gtx/hash.hpp"

//disable warning pop
FluidumUtils_Debug_EndDisableAllWarning

//FluidumVK
#include "FVKdefine.h"
#include "../Exception/FVKexception.h"
#include "FVKpasskey.h"
#include "../Data/FVKdata.h"
#include "FVKinfo.h"
#include "../Key/FVKinclude.h"
#include "FVKmutex.h"
#include "FVKlog.h"