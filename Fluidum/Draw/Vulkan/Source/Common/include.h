#pragma once

//Fluidum Vulkan
#include "type.h"

//KeyManager
#include "../../KeyManager/include.h"

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
#include "../../../Libraries/glm/glm/glm.hpp"
#include "../../../Libraries/glm/glm/gtc/matrix_transform.hpp"
#include "../../../Libraries/glm/glm/gtx/hash.hpp"

//disable warning pop
FluidumUtils_Debug_EndDisableAllWarning

//Fluidum Vulkan
#include "../Exception/exception.h"
#include "passkey.h"
#include "../Data/data.h"
#include "info.h"
#include "../Key/include.h"
#include "mutex.h"
#include "log.h"