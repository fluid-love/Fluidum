/*
ThreadSafe

Key:各アイテムには文字列のkeyを割り当てる．

	keyに割り当てる文字列は'__'で始めない．


Exception
	*その関数の主目的が失敗したとき
		FailedToCreate
		using FailedToInitialize = FailedToAllocate = FailedToCreate

	*FluidumVKが初期化されていない
		NotInitialized

	*使用を試みたkeyが割り当てされていない
		NotFound

	*割り当てる(登録する)keyが既に他のアイテムでつかわれている
		CollisionOfKeys

	*削除に失敗
		FailedToErase;


*/

#pragma once

#include "using.h"
#include "assist.h"

#define FluidumVK_API

//initialize terminate
namespace FVK {

	//初期化する
	/*
	全てのFluidumAPI関数を呼ぶ前に呼び出す必要がある

	catchを推奨する例外:
		FailedToInitialize
			->既に初期化されている
			->glfwInit()に失敗．GLFWの初期化に失敗
		NotSupported
			->Vulkanに未対応
	*/
	FluidumVK_API void initialize();


	//初期化されているか
	/*
	catchを推奨する例外:
		投げない
	*/
	FluidumVK_API _NODISCARD bool isFluidumVKInitialized() noexcept;


	//終了する
	/*
	catchを推奨する例外:
		NotInitialized
			->初期化していない
	*/
	FluidumVK_API void terminate();


}

//Debug
namespace FVK {

	//Messageが発生したときに一回呼び出される
	/*
	引数はconst std::string&であり，TimeとLogType付のメッセージが入ってくる．

	引数:
		callback 呼び出される関数

	*/
	FluidumVK_API void setMessengerCallbackFunction(const MessengerCallbackType callback);

}

//Window
namespace FVK {

	//Windowの追加と作成
	/*
		WindowParameterでWindowの情報を指定できる．
		基本的には1つ作成すれば良いが，他のウィンドウへの描写をするときは二個目を作成する必要がある.

		引数:
			key       作成するWindowに割り当てるkey
			parameter 作成するWindowのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			FailedToCreate
				->GLFWwindowの作成に失敗
	*/
	template<Internal::IsWindowParameter Parameter>
	FluidumVK_API void createWindow(const char* key, const Parameter& parameter) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->add<FvkType::Window>(key, parameter);
	}


	//Windowの破壊
	/*
		引数:
			key 破壊するWindowのkey

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyがみつからない
			FailedToErase
				->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Window>(key);
	}


	//glfwWindowShouldClose == trueになるまで引数funcをループする
	/*
		引数:
			key  Windowのkey
			func ループ内で実行する関数のポインタ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T, typename... Args>
	FluidumVK_API void loopUntilWindowShouldClose(const WindowKey<T>& key, void(*func)(Args...), Args&&...args) {
		using namespace Internal;
		GLFWwindow* window = nullptr;
		{
			LockGuard lock(GMutex);
			API::checkManagerEmpty();
			const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
			window = info.window;
		}
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			func(std::forward<Args>(args)...);
		}
	}


	//FrameBufferのサイズが変更されるとtrueとなっている変数を返し，falseにする
	/*
		引数:
			key Windowのkey

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD bool isFrameBufferResized(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		bool result = info.framebufferResized;
		info.framebufferResized = false;
		return result;
	}


	//Window情報の変更・取得
	/*
		引数:
			key Windowのkey

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD std::pair<int32_t, int32_t> getWindowSize(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Window& item = GManager->refItem<FvkType::Window>(key);
		return item.getWindowSize();
	}
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD std::pair<int32_t, int32_t> getWindowPos(const WindowKey<T>& key) {
		using namespace Internal;
		int32_t x, y;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwGetWindowPos(info.window, &x, &y);
		return { x,y };
	}
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void setWindowSize(const WindowKey<T>& key, const int32_t width, const int32_t height) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwSetWindowSize(info.window, width, height);
	}
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void setWindowPos(const WindowKey<T>& key, const int32_t x, const int32_t y) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwSetWindowPos(info.window, x, y);
	}


	//最小化・最大化する
	/*
		引数:
			key Windowのkey

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void minimizeWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwIconifyWindow(info.window);
	}
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void maximizeWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::WindowInfo& info = GManager->refInfo<FvkType::Window>(key);
		glfwMaximizeWindow(info.window);
	}

}

//Instance
namespace FVK {

	//Instanceの追加と作成
	/*
		ParameterをInstanceMessengerParameterにするとMessengerを登録できる．

		引数:
			key       作成するInstanceに割り当てるkey
			parameter 作成するInstanceのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			FailedToCreate
				->ValidationLayerがサポートされていない
				->作成に失敗
	*/
	FluidumVK_API void createInstance(const char* key, const InstanceParameter& parameter);
	FluidumVK_API void createInstance(const char* key, const InstanceMessengerParameter& parameter);


	//Instanceの破壊
	/*
	Messenger付なら一緒に削除

	引数:
		key 削除するInstanceのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyInstance(const InstanceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		//Messengerありの場合
		const StringKeyType instanceStrKey = Key::Converter::keyToStringKeyType(key);
		StringKeyType messengerStrKey = "__Messenger_";
		messengerStrKey += instanceStrKey;

		const Data::InstanceInfo& info = GManager->refInfo<FvkType::Instance>(key);
		if (info.isMessengerCreated)
			GManager->erase<FvkType::Messenger>(Key::MessengerKey<const char*>{messengerStrKey.c_str()});

		GManager->erase<FvkType::Instance>(key);
	}


}

//Surface
namespace FVK {

	//Surfaceの追加と作成
	/*
		parameterのkeyは全ていれること

		引数:
			key         作成するSurfaceに割り当てるkey
			parameter   作成するSurfaceのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createSurface(const char* key, const SurfaceParameter& parameter);


	//Surfaceの削除
	/*
	引数:
		key 削除するSurfaceのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySurface(const SurfaceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Surface>(key);
	}


}

//PhysicalDevice
namespace FVK {

	//PhysicalDeviceの検索と選択
	/*
		引数:
			key         作成するPhysicalDeviceに割り当てるkey
			parameter   作成するPhysicalDeviceのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->要求する条件の物理デバイスがみつからない
	*/
	FluidumVK_API void pickPhysicalDevice(const char* key, const PhysicalDeviceParameter& parameter);


	//Swapchainをサポートする物理デバイスを見つける
	FluidumVK_API void pickPhysicalDevice(const char* key, const PhysicalDeviceSwapchainParameter& parameter);


	//利用可能な最大のsampleを取得
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::SampleCountFlagBits getMaxUsableSampleCount(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.usableMaxMsaaSamples;
	}


	//対応しているSurfaceFormatを取得
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない

	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD std::vector<vk::SurfaceFormatKHR> getSupportedSurfaceFormats(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.formats;
	}


	//SurfaceFormatKHR
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない

	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD bool isSurfaceSupportedFormat(const PhysicalDeviceKey<T>& key, const vk::Format format, const vk::ColorSpaceKHR colorSpace) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return Swapchain::isSurfaceFormatSupport(format, colorSpace, data.formats);
	}

	//指定した条件にあうvk::Formatを探す
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない
			NotSupported
				->サポートされていない

	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::Format findSupportedFormat(const PhysicalDeviceKey<T>& key, const std::vector<vk::Format>& candidates, const vk::ImageTiling tiling, const vk::FormatFeatureFlags features) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& info = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return PhysicalDevice::findSupportedFormat(info.physicalDevice, candidates, tiling, features);
	}

	//PresentMode
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD bool isPresentModeSupport(const PhysicalDeviceKey<T>& key, const vk::PresentModeKHR presentMode) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return Swapchain::isPresentModeSupport(presentMode, data.presentModes);
	}


	//vk::SurfaceCapabilitiesKHRを取得
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.capabilities;
	}


	//propertyを取得
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::PhysicalDeviceProperties getPhysicalDeviceProperties(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.physicalDevice.getProperties();
	}


	//QueueFamilyを取得
	/*
		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD uint32_t getGraphicsQueueFmily(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.graphicsFamily;
	}
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD uint32_t getPresentQueueFmily(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.presentFamily;
	}


	//find memory type
	/*
	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyが見つからない
		NotSupported
			->サポートされていない
	*/
	template<Internal::Key::IsKeyType T1>
	FluidumVK_API _NODISCARD uint32_t findMemoryType(
		const PhysicalDeviceKey<T1>& key,
		const uint32_t typeFilter,
		const vk::MemoryPropertyFlags properties
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& info = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return PhysicalDevice::findMemoryType(info.physicalDevice, typeFilter, properties);
	}


	//PhysicalDeviceの削除
	/*
	Vulkan側では何もしないがFluidumVKにおいてkeyの管理を外す

	引数:
		key 削除するPhysicalDeviceのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyPhysicalDevice(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::PhysicalDevice>(key);
	}


}

//LogicalDevice
namespace FVK {

	//LogicalDeviceの追加と作成
	/*
		引数:
			key         作成するLogicalDeviceに割り当てるkey
			parameter   作成するLogicalDeviceのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	//PhysicalDeviceで要求したextensionsを全て有効にする．
	FluidumVK_API void createLogicalDevice(const char* key, const LogicalDeviceParameter& parameter);


	//LogicalDeviceの削除
	/*
	引数:
		key 削除するLogicalDeviceのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyLogicalDevice(const LogicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::LogicalDevice>(key);
	}


}

//Queue
namespace FVK {

	//Queueの取得
	/*
		引数:
			key         作成するQueueに割り当てるkey
			parameter   作成するQueueのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void getQueue(const char* key, const QueueParameter& parameter);


	//Queueの削除
	/*
	Vulkan側では何もしないがFluidumVKにおいてkeyの管理を外す

	引数:
		key 削除するQueueのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyQueue(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::PhysicalDevice>(key);
	}


}

//Swapchain
namespace FVK {

	//Swapchainの追加と作成
	/*
		引数:
			key         作成するSwapchainに割り当てるkey
			parameter   作成するSwapchainのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createSwapchain(const char* key, SwapchainParameter& parameter);


	//Swapchainの削除
	/*
	引数:
		key 削除するSwapchainのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySwapchain(const SwapchainKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Swapchain>(key);
	}


	//正しいSwapchainExtentを取得
	/*
		catchを推奨する例外:
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API _NODISCARD vk::Extent2D getCorrectSwapchainExtent(
		const WindowKey<T1>& window,
		const PhysicalDeviceKey<T2>& physicalDevice
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& physicalDeviceInfo = GManager->refInfo<FvkType::PhysicalDevice>(physicalDevice);
		const Data::WindowInfo& windowInfo = GManager->refInfo<FvkType::Window>(window);
		return Swapchain::getCorrectSwapchainExtent(windowInfo.window, physicalDeviceInfo.capabilities);
	}


	//imageFormatを取得
	/*
		catchを推奨する例外:
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T1>
	FluidumVK_API _NODISCARD vk::Format getSwapchainImageFormat(
		const SwapchainKey<T1>& swapchain
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::SwapchainInfo& info = GManager->refInfo<FvkType::Swapchain>(swapchain);
		return info.format;
	}


	//Extentを取得
	/*
		catchを推奨する例外:
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T1>
	FluidumVK_API _NODISCARD vk::Extent2D getSwapchainExtent(
		const SwapchainKey<T1>& swapchain
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::SwapchainInfo& info = GManager->refInfo<FvkType::Swapchain>(swapchain);
		return info.extent;
	}


	//MinImageCountを取得
	/*
		catchを推奨する例外:
			NotFound
				->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T1>
	FluidumVK_API _NODISCARD uint32_t getSwapchainMinImageCount(
		const SwapchainKey<T1>& swapchain
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::SwapchainInfo& info = GManager->refInfo<FvkType::Swapchain>(swapchain);
		return info.minImageCount;
	}

}

//RenderPass
namespace FVK {

	//RenderPassの追加と作成
	/*
		引数:
			key         作成するRenderPassに割り当てるkey
			parameter   作成するRenderPassのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createRenderPass(const char* key, const RenderPassParameter& parameter);


	//RenderPassの削除
	/*
	引数:
		key 削除するRenderPassのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyRenderPass(const RenderPassKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::RenderPass>(key);
	}


}

//DescriptorSetLayout
namespace FVK {

	//DescriptorSetLayoutの追加と作成
	/*
		引数:
			key         作成するDescriptorSetLayoutに割り当てるkey
			parameter   作成するDescriptorSetLayoutのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createDescriptorSetLayout(const char* key, const DescriptorSetLayoutParameter& parameter);


	//UpdateDescriptorSet
	/*
		引数:
			parameter   パラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			NotFound
				->Parameterにkeyをいれてから，この関数を呼ぶまでに設定したkeyを変更・削除した
	*/
	FluidumVK_API void updateDescriptorSet(UpdateDescriptorSetParameter& parameter);


	//DescriptorSetLayoutの削除
	/*
	引数:
		key 削除するDescriptorSetLayoutのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyDescriptorSetLayout(const DescriptorSetLayoutKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::DescriptorSetLayout>(key);
	}


}

//ShaderModule
namespace FVK {

	//ShaderModuleの追加と作成
	/*
		引数:
			key         作成するShaderModuleに割り当てるkey
			parameter   作成するShaderModuleのパラメータ

		catchを推奨する例外:
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			NotInitialized
				->初期化していない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createShaderModule(const char* key, const ShaderModuleParameter& parameter);


	//ShaderModuleの削除
	/*
	引数:
		key 削除するShaderModuleのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyShaderModule(const ShaderModuleKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ShaderModule>(key);
	}


	//ShaderFileの中身を読み込む
	/*
		引数:
			filePath 読み込むファイルのパス

		catchを推奨する例外:
			runtime_error
				->ファイルの読み込みに失敗
	*/
	FluidumVK_API _NODISCARD std::vector<char> readShaderFile(const char* filePath);

}

//GraphicsPipelineLayout
namespace FVK {

	//GraphicsPipelineLayoutの追加と作成
	/*
		引数:
			key         作成するGraphicsPipelineLayoutに割り当てるkey
			parameter   作成するGraphicsPipelineLayoutのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createGraphicsPipelineLayout(const char* key, GraphicsPipelineLayoutParameter& parameter);


	//GraphicsPipelineLayoutの削除
	/*
	引数:
		key 削除するGraphicsPipelineLayoutのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyGraphicsPipelineLayout(const GraphicsPipelineLayoutKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::GraphicsPipelineLayout>(key);
	}


}


//GraphicsPipeline
namespace FVK {

	//GraphicsPipelineの追加と作成
	/*
		引数:
			key         作成するGraphicsPipelineに割り当てるkey
			parameter   作成するGraphicsPipelineのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createGraphicsPipeline(const char* key, GraphicsPipelineParameter& parameter);


	//GraphicsPipelineの削除
	/*
	引数:
		key 削除するGraphicsPipelineのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyGraphicsPipeline(const GraphicsPipelineKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::GraphicsPipeline>(key);
	}


}

//Image
namespace FVK {

	//Imageの追加と作成
	/*
		引数:
			key         作成するImageに割り当てるkey
			parameter   作成するImageのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createImage(const char* key, const ImageParameter& parameter);


	//Imageの削除
	/*
	引数:
		key 削除するImageのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImage(const ImageKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Image>(key);
	}


	//bindImageMemory
	/*
	catchを推奨する例外:
		NotFound
			->keyが見つからない
		FailedToCreate
			->bindに失敗
	*/
	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2, Internal::Key::IsKeyType T3>
	FluidumVK_API _NODISCARD void bindImageMemory(
		const LogicalDeviceKey<T3>& logicalDeviceKey,
		const ImageKey<T1>& imageKey,
		const DeviceMemoryKey<T2>& deviceMemoryKey,
		const vk::DeviceSize offset = 0
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();

		const Data::LogicalDeviceInfo& logicalDeviceInfo = GManager->refInfo<FvkType::LogicalDevice>(logicalDeviceKey);
		const Data::ImageInfo& imageInfo = GManager->refInfo<FvkType::Image>(imageKey);
		const Data::DeviceMemoryInfo& deviceMemoryInfo = GManager->refInfo<FvkType::DeviceMemory>(deviceMemoryKey);

		auto result = logicalDeviceInfo.device.bindImageMemory(imageInfo.image, deviceMemoryInfo.deviceMemory, offset);

		if (result != vk::Result::eSuccess)
			::FVK::Internal::Exception::throwFailedToCreate("Failed to bind image memory");
	}



}

//ImageView
namespace FVK {

	//ImageViewの追加と作成
	/*
		引数:
			key         作成するImageViewに割り当てるkey
			parameter   作成するImageViewのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createImageView(const char* key, ImageViewParameter& parameter);
	FluidumVK_API void createImageView(const char* key, const SwapchainImageViewParameter& parameter);


	//ImageViewの削除
	/*
	引数:
		key 削除するImageViewのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImageView(const ImageViewKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ImageView>(key);
	}


}

//DeviceMemory
namespace FVK {

	//DeviceMemoryの追加と割り当て
	/*
		引数:
			key         作成するDeviceMemoryに割り当てるkey
			parameter   作成するDeviceMemoryのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->メモリ割り当てに失敗
	*/
	FluidumVK_API void allocateDeviceMemory(const char* key, const DeviceMemoryParameter& parameter);


	//DeviceMemoryの解放
	/*
	引数:
		key 削除するDeviceMemoryのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void freeDeviceMemory(const DeviceMemoryKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::DeviceMemory>(key);
	}


	//getImageMemoryRequirements
	/*
	catchを推奨する例外:
		NotFound
			->keyが見つからない
	*/
	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API _NODISCARD vk::MemoryRequirements getImageMemoryRequirements(
		const LogicalDeviceKey<T1>& logicalDeviceKey,
		const ImageKey<T2>& imageKey
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();

		const Data::LogicalDeviceInfo& logicalDeviceInfo = GManager->refInfo<FvkType::LogicalDevice>(logicalDeviceKey);
		const Data::ImageInfo& imageInfo = GManager->refInfo<FvkType::Image>(imageKey);

		return logicalDeviceInfo.device.getImageMemoryRequirements(imageInfo.image);
	}

}

//CommandPool
namespace FVK {

	//CommandPoolの追加と割り当て
	/*
		引数:
			key         作成するCommandPoolに割り当てるkey
			parameter   作成するCommandPoolのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterに設定したkeyが見つからない．割り当ててない，もしくは，変更削除した可能性がある．
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createCommandPool(const char* key, const CommandPoolParameter& parameter);


	//CommandPoolの破壊
	/*
	引数:
		key 削除するCommandPoolのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyCommandPool(const CommandPoolKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::CommandPool>(key);
	}


}

//VertexBuffer
namespace FVK {

	//VertexBufferの追加と割り当て
	/*
		引数:
			key         作成するVertexBufferに割り当てるkey
			parameter   作成するVertexBufferのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->Parameterにkeyをいれてから，この関数を呼ぶまでに設定したkeyを変更・削除した
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createVertexBuffer(const char* key, const VertexBufferParameter& parameter);


	//VertexBufferの破壊
	/*
	引数:
		key 削除するVertexBufferのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyVertexBuffer(const VertexBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::VertexBuffer>(key);
	}


}

//IndexBuffer
namespace FVK {

	//IndexBufferの追加と割り当て
	/*
		引数:
			key         作成するIndexBufferに割り当てるkey
			parameter   作成するIndexBufferのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->Parameterにkeyをいれてから，この関数を呼ぶまでに設定したkeyを変更・削除した
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createIndexBuffer(const char* key, const IndexBufferParameter& parameter);


	//IndexBufferの破壊
	/*
	引数:
		key 削除するIndexBufferのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyIndexBuffer(const IndexBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::IndexBuffer>(key);
	}


}

//UniformBuffer
namespace FVK {

	//UniformBufferの追加と割り当て
	/*
		引数:
			key         作成するUniformBufferに割り当てるkey
			parameter   作成するUniformBufferのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->Parameterにkeyをいれてから，この関数を呼ぶまでに設定したkeyを変更・削除した
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createUniformBuffer(const char* key, const UniformBufferParameter& parameter);


	//UniformBufferの破壊
	/*
	引数:
		key 削除するUniformBufferのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
		FailedToErase
			->他のアイテムで使用しているのに削除しようとした
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyUniformBuffer(const UniformBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::UniformBuffer>(key);
	}


}

//DescriptorPool
namespace FVK {

	//DescriptorPoolの追加と割り当て
	/*
		引数:
			key         作成するDescriptorPoolに割り当てるkey
			parameter   作成するDescriptorPoolのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->Parameterにkeyをいれてから，この関数を呼ぶまでに設定したkeyを変更・削除した
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createDescriptorPool(const char* key, const DescriptorPoolParameter& parameter);


	//DescriptorPoolの破壊
	/*
	引数:
		key 削除するDescriptorPoolのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyDescriptorPool(const DescriptorPoolKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::DescriptorPool>(key);
	}


}

//DescriptorSet
namespace FVK {

	//DescriptorSetの追加と割り当て
	/*
		引数:
			key         作成するDescriptorSetに割り当てるkey
			parameter   作成するDescriptorSetのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void allocateDescriptorSet(const char* key, DescriptorSetParameter& parameter);


	//DescriptorSetの解放
	/*
	引数:
		key 削除するDescriptorSetのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void freeDescriptorSet(const DescriptorSetKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::DescriptorSet>(key);
	}

}

//CommandBuffer
namespace FVK {

	//CommandBufferの追加と割り当て
	/*
		引数:
			key         作成するCommandBufferに割り当てるkey
			parameter   作成するCommandBufferのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			std::bad_optional_access
				->parameterに必要なkeyがセットされていない
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void allocateCommandBuffers(const char* key, CommandBufferParameter& parameter);


	//CommandBufferの解放
	/*
	引数:
		key 削除するCommandBufferのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void freeCommandBuffer(const CommandBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::CommandBuffer>(key);
	}


}

//FrameBuffer
namespace FVK {

	//FrameBufferの追加と割り当て
	/*
		引数:
			key         作成するFrameBufferに割り当てるkey
			parameter   作成するFrameBufferのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createFrameBuffer(const char* key, FrameBufferParameter& parameter);


	//FrameBufferの破壊
	/*
	引数:
		key 削除するFrameBufferのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyFrameBuffer(const FrameBufferKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::FrameBuffer>(key);
	}


}

//Semaphore
namespace FVK {

	//Semaphoreの追加と割り当て
	/*
		引数:
			key         作成するSemaphoreに割り当てるkey
			parameter   作成するSemaphoreのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			std::bad_optional_access
				->parameterに必要なkeyがセットされていない
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createSemaphore(const char* key, const SemaphoreParameter& parameter);


	//Semaphoreの破壊
	/*
	引数:
		key 削除するSemaphoreのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySemaphore(const SemaphoreKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Semaphore>(key);
	}


}

//Fence
namespace FVK {

	//Fenceの追加と割り当て
	/*
		引数:
			key         作成するFenceに割り当てるkey
			parameter   作成するFenceのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createFence(const char* key, const FenceParameter& parameter);


	//Fenceの破壊
	/*
	引数:
		key 削除するFenceのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyFence(const FenceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Fence>(key);
	}


}

//Sampler
namespace FVK {

	//Samplerの追加と割り当て
	/*
		引数:
			key         作成するSamplerに割り当てるkey
			parameter   作成するSamplerのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createSampler(const char* key, const SamplerParameter& parameter);


	//Samplerの破壊
	/*
	引数:
		key 削除するSamplerのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySampler(const SamplerKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Sampler>(key);
	}


}

//Texture
namespace FVK {

	//Textureの追加と割り当て
	/*
		引数:
			key         作成するSamplerに割り当てるkey
			parameter   作成するSamplerのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createTexture(const char* key, const TextureParameter& parameter);


	//Textureの破壊
	/*
	引数:
		key 削除するTextureのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyTexture(const TextureKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Texture>(key);
	}

}

//ImGui
namespace FVK {

	//ImGuiの追加と割り当て
	/*
		引数:
			key         作成するImGuiに割り当てるkey
			parameter   作成するImGuiのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API void createImGui(const char* key, const ImGuiParameter& parameter);


	//ImGuiの破壊
	/*
	引数:
		key 削除するImGuiのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImGui(const ImGuiKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ImGui>(key);
	}


	//ImTextureIDを作成
	/*
		ImGui::Image()の第一引数に渡すと画像の描写が可能

		引数:
			key         作成するImGuiに割り当てるkey
			parameter   作成するImGuiのパラメータ

		catchを推奨する例外:
			NotInitialized
				->初期化していない
			CollisionOfKeys
				->keyが他のkeyと被っている
			NotFound
				->parameterにセットしたkeyが存在しない
			FailedToCreate
				->作成に失敗
	*/
	FluidumVK_API _NODISCARD ImTextureID createImGuiImage(const char* key, const ImGuiImageParameter& parameter);


	//ImGuiImageの破壊
	/*
	引数:
		key 削除するImGuImageのkey

	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImGuiImage(const ImGuiImageKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ImGuiImage>(key);
	}


	//createImGuiで追加したフォントを取得
	/*
	引数:
		key 取得するImGuiのkey
	
	catchを推奨する例外:
		NotInitialized
			->初期化していない
		NotFound
			->keyがみつからない
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API std::vector<ImFont*> getImGuiFonts(const ImGuiKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::ImGuiInfo& info = GManager->refInfo<FvkType::ImGui>(key);
		return info.fonts;
	}


}

//Command
namespace FVK {


	template<Internal::Key::IsKeyType T1>
	FluidumVK_API _NODISCARD BeginCommandBufferCommand makeBeginCommandBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BeginCommandBufferConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::BeginCommandBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType T1>
	FluidumVK_API _NODISCARD EndCommandBufferCommand makeEndCommandBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::EndCommandBufferConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::EndCommandBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API _NODISCARD BindVertexBuffersCommand makeBindVertexBuffersCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const VertexBufferKey<T2>& vertexBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BindVertexbuffersConnectionKeys(commandBuffer, vertexBuffer);
		return GManager->makeCommand<CommandType::BindVertexBuffers>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2, Internal::Key::IsKeyType  T3>
	FluidumVK_API _NODISCARD BeginRenderPassCommand makeBeginRenderPassCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const RenderPassKey<T2>& renderPass,
		const FrameBufferKey<T3>& frameBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BeginRenderPassConnectionKeys(commandBuffer, renderPass, frameBuffer);
		return GManager->makeCommand<CommandType::BeginRenderPass>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2, Internal::Key::IsKeyType  T3>
	FluidumVK_API _NODISCARD BindDescriptorSetCommand makeBindDescriptorSetCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const GraphicsPipelineLayoutKey<T2>& graphicsPipelineLayout,
		const DescriptorSetKey<T3>& descriptorSet,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BindDescriptorSetConnectionKeys(commandBuffer, graphicsPipelineLayout, descriptorSet);
		return GManager->makeCommand<CommandType::BindDescriptorSet>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API _NODISCARD BindGraphicsPipelineCommand makeBindGraphicsPipelineCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const GraphicsPipelineKey<T2>& graphicsPipeline,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BindGraphicsPipelineConnectionKeys(commandBuffer, graphicsPipeline);
		return GManager->makeCommand<CommandType::BindGraphicsPipeline>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API _NODISCARD BindIndexBufferCommand makeBindIndexBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const IndexBufferKey<T2>& indexBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BindIndexBufferConnectionKeys(commandBuffer, indexBuffer);
		return GManager->makeCommand<CommandType::BindIndexBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API _NODISCARD DrawIndexedCommand makeDrawIndexedCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::DrawIndexedConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::DrawIndexed>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API _NODISCARD EndRenderPassCommand makeEndRenderPassCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::EndRenderPassConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::EndRenderPass>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API _NODISCARD ImGuiRenderDrawDataCommand makeImGuiRenderDrawDataCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::ImGuiRenderDrawDataConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::ImGuiRenderDrawData>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType T1, Internal::Key::IsKeyType T2>
	FluidumVK_API _NODISCARD BindImGuiImageCommand makeBindImGuiImageCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const ImGuiImageKey<T2>& imguiImage,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::BindImGuiImageConnectionKeys(commandBuffer, imguiImage);
		return GManager->makeCommand<CommandType::BindImGuiImage>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API _NODISCARD SetScissorCommand makeSetScissorCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::SetScissorConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::SetScissor>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1>
	FluidumVK_API _NODISCARD SetViewportCommand makeSetViewportCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::SetViewportConnectionKeys(commandBuffer);
		return GManager->makeCommand<CommandType::SetViewport>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API _NODISCARD UpdateVertexBufferCommand makeUpdateVertexBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const VertexBufferKey<T2>& vertexBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::UpdateVertexBufferConnectionKeys(commandBuffer, vertexBuffer);
		return GManager->makeCommand<CommandType::UpdateVertexBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API _NODISCARD UpdateIndexBufferCommand makeUpdateIndexBufferCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const IndexBufferKey<T2>& indexBuffer,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::UpdateIndexBufferConnectionKeys(commandBuffer, indexBuffer);
		return GManager->makeCommand<CommandType::UpdateIndexBuffer>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API _NODISCARD TransitionImageLayoutCommand makeTransitionImageLayout(
		const CommandBufferKey<T1>& commandBuffer,
		const ImageKey<T2>& image,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::TransitionImageLayoutConnectionKeys(commandBuffer, image);
		return GManager->makeCommand<CommandType::TransitionImageLayout>(connections, commandBufferIndex);
	}

	template<Internal::Key::IsKeyType  T1, Internal::Key::IsKeyType  T2>
	FluidumVK_API _NODISCARD QueueSubmitCommand makeQueueSubmitCommand(
		const CommandBufferKey<T1>& commandBuffer,
		const QueueKey<T2>& queue,
		const uint32_t commandBufferIndex
	) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const auto connections = Key::QueueSubmitConnectionKeys(commandBuffer, queue);
		return GManager->makeCommand<CommandType::QueueSubmit>(connections, commandBufferIndex);
	}

	FluidumVK_API _NODISCARD NextCommand makeNextCommand();

	//template<typename /*Internal::Key::IsKey バグ*/ ...T>
	//FluidumVK_API _NODISCARD auto makeAnyCommand(const T&... keys) {
	//	using namespace Internal;
	//	LockGuard lock(GMutex);
	//	API::checkManagerEmpty();
	//	std::array<std::vector<IndexKey>, sizeof...(T)> indices
	//		= { std::vector<IndexKey>{Key::Converter::toIndexKey(keys)}... };

	//	return GManager->makeAnyCommand<T...>(indices);
	//}

	template<typename /*Internal::Key::IsKey バグ*/ ...T>
	FluidumVK_API _NODISCARD auto makeAnyCommand(const T&... keys) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		std::array<std::vector<IndexKey>, sizeof...(T)> indices
			= { Key::Converter::toIndexKeyVector(keys)... };

		return GManager->makeAnyCommand<T...>(indices);
	}

	void  exeCommands(Commands& commands);

}