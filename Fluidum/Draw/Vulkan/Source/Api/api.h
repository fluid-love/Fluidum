/*
ThreadSafe

Key:�e�A�C�e���ɂ͕������key�����蓖�Ă�D

	key�Ɋ��蓖�Ă镶�����'__'�Ŏn�߂Ȃ��D


Exception
	*���̊֐��̎�ړI�����s�����Ƃ�
		FailedToCreate
		using FailedToInitialize = FailedToAllocate = FailedToCreate

	*FluidumVK������������Ă��Ȃ�
		NotInitialized

	*�g�p�����݂�key�����蓖�Ă���Ă��Ȃ�
		NotFound

	*���蓖�Ă�(�o�^����)key�����ɑ��̃A�C�e���ł����Ă���
		CollisionOfKeys

	*�폜�Ɏ��s
		FailedToErase;


*/

#pragma once

#include "using.h"
#include "assist.h"

#define FluidumVK_API

//initialize terminate
namespace FVK {

	//����������
	/*
	�S�Ă�FluidumAPI�֐����ĂԑO�ɌĂяo���K�v������

	catch�𐄏������O:
		FailedToInitialize
			->���ɏ���������Ă���
			->glfwInit()�Ɏ��s�DGLFW�̏������Ɏ��s
		NotSupported
			->Vulkan�ɖ��Ή�
	*/
	FluidumVK_API void initialize();


	//����������Ă��邩
	/*
	catch�𐄏������O:
		�����Ȃ�
	*/
	FluidumVK_API _NODISCARD bool isFluidumVKInitialized() noexcept;


	//�I������
	/*
	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
	*/
	FluidumVK_API void terminate();


}

//Debug
namespace FVK {

	//Message�����������Ƃ��Ɉ��Ăяo�����
	/*
	������const std::string&�ł���CTime��LogType�t�̃��b�Z�[�W�������Ă���D

	����:
		callback �Ăяo�����֐�

	*/
	FluidumVK_API void setMessengerCallbackFunction(const MessengerCallbackType callback);

}

//Window
namespace FVK {

	//Window�̒ǉ��ƍ쐬
	/*
		WindowParameter��Window�̏����w��ł���D
		��{�I�ɂ�1�쐬����Ηǂ����C���̃E�B���h�E�ւ̕`�ʂ�����Ƃ��͓�ڂ��쐬����K�v������.

		����:
			key       �쐬����Window�Ɋ��蓖�Ă�key
			parameter �쐬����Window�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			FailedToCreate
				->GLFWwindow�̍쐬�Ɏ��s
	*/
	template<Internal::IsWindowParameter Parameter>
	FluidumVK_API void createWindow(const char* key, const Parameter& parameter) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->add<FvkType::Window>(key, parameter);
	}


	//Window�̔j��
	/*
		����:
			key �j�󂷂�Window��key

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key���݂���Ȃ�
			FailedToErase
				->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyWindow(const WindowKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Window>(key);
	}


	//glfwWindowShouldClose == true�ɂȂ�܂ň���func�����[�v����
	/*
		����:
			key  Window��key
			func ���[�v���Ŏ��s����֐��̃|�C���^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key���݂���Ȃ�
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


	//FrameBuffer�̃T�C�Y���ύX������true�ƂȂ��Ă���ϐ���Ԃ��Cfalse�ɂ���
	/*
		����:
			key Window��key

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key���݂���Ȃ�
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


	//Window���̕ύX�E�擾
	/*
		����:
			key Window��key

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key���݂���Ȃ�
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


	//�ŏ����E�ő剻����
	/*
		����:
			key Window��key

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key���݂���Ȃ�
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

	//Instance�̒ǉ��ƍ쐬
	/*
		Parameter��InstanceMessengerParameter�ɂ����Messenger��o�^�ł���D

		����:
			key       �쐬����Instance�Ɋ��蓖�Ă�key
			parameter �쐬����Instance�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			FailedToCreate
				->ValidationLayer���T�|�[�g����Ă��Ȃ�
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createInstance(const char* key, const InstanceParameter& parameter);
	FluidumVK_API void createInstance(const char* key, const InstanceMessengerParameter& parameter);


	//Instance�̔j��
	/*
	Messenger�t�Ȃ�ꏏ�ɍ폜

	����:
		key �폜����Instance��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyInstance(const InstanceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		//Messenger����̏ꍇ
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

	//Surface�̒ǉ��ƍ쐬
	/*
		parameter��key�͑S�Ă���邱��

		����:
			key         �쐬����Surface�Ɋ��蓖�Ă�key
			parameter   �쐬����Surface�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createSurface(const char* key, const SurfaceParameter& parameter);


	//Surface�̍폜
	/*
	����:
		key �폜����Surface��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//PhysicalDevice�̌����ƑI��
	/*
		����:
			key         �쐬����PhysicalDevice�Ɋ��蓖�Ă�key
			parameter   �쐬����PhysicalDevice�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->�v����������̕����f�o�C�X���݂���Ȃ�
	*/
	FluidumVK_API void pickPhysicalDevice(const char* key, const PhysicalDeviceParameter& parameter);


	//Swapchain���T�|�[�g���镨���f�o�C�X��������
	FluidumVK_API void pickPhysicalDevice(const char* key, const PhysicalDeviceSwapchainParameter& parameter);


	//���p�\�ȍő��sample���擾
	/*
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::SampleCountFlagBits getMaxUsableSampleCount(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.usableMaxMsaaSamples;
	}


	//�Ή����Ă���SurfaceFormat���擾
	/*
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�

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
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�

	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD bool isSurfaceSupportedFormat(const PhysicalDeviceKey<T>& key, const vk::Format format, const vk::ColorSpaceKHR colorSpace) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return Swapchain::isSurfaceFormatSupport(format, colorSpace, data.formats);
	}

	//�w�肵�������ɂ���vk::Format��T��
	/*
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�
			NotSupported
				->�T�|�[�g����Ă��Ȃ�

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
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD bool isPresentModeSupport(const PhysicalDeviceKey<T>& key, const vk::PresentModeKHR presentMode) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return Swapchain::isPresentModeSupport(presentMode, data.presentModes);
	}


	//vk::SurfaceCapabilitiesKHR���擾
	/*
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::SurfaceCapabilitiesKHR getSurfaceCapabilities(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.capabilities;
	}


	//property���擾
	/*
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API _NODISCARD vk::PhysicalDeviceProperties getPhysicalDeviceProperties(const PhysicalDeviceKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		const Data::PhysicalDeviceInfo& data = GManager->refInfo<FvkType::PhysicalDevice>(key);
		return data.physicalDevice.getProperties();
	}


	//QueueFamily���擾
	/*
		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->key��������Ȃ�
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
	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key��������Ȃ�
		NotSupported
			->�T�|�[�g����Ă��Ȃ�
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


	//PhysicalDevice�̍폜
	/*
	Vulkan���ł͉������Ȃ���FluidumVK�ɂ�����key�̊Ǘ����O��

	����:
		key �폜����PhysicalDevice��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//LogicalDevice�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����LogicalDevice�Ɋ��蓖�Ă�key
			parameter   �쐬����LogicalDevice�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	//PhysicalDevice�ŗv������extensions��S�ėL���ɂ���D
	FluidumVK_API void createLogicalDevice(const char* key, const LogicalDeviceParameter& parameter);


	//LogicalDevice�̍폜
	/*
	����:
		key �폜����LogicalDevice��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//Queue�̎擾
	/*
		����:
			key         �쐬����Queue�Ɋ��蓖�Ă�key
			parameter   �쐬����Queue�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void getQueue(const char* key, const QueueParameter& parameter);


	//Queue�̍폜
	/*
	Vulkan���ł͉������Ȃ���FluidumVK�ɂ�����key�̊Ǘ����O��

	����:
		key �폜����Queue��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//Swapchain�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����Swapchain�Ɋ��蓖�Ă�key
			parameter   �쐬����Swapchain�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createSwapchain(const char* key, SwapchainParameter& parameter);


	//Swapchain�̍폜
	/*
	����:
		key �폜����Swapchain��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroySwapchain(const SwapchainKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::Swapchain>(key);
	}


	//������SwapchainExtent���擾
	/*
		catch�𐄏������O:
			NotFound
				->key��������Ȃ�
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


	//imageFormat���擾
	/*
		catch�𐄏������O:
			NotFound
				->key��������Ȃ�
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


	//Extent���擾
	/*
		catch�𐄏������O:
			NotFound
				->key��������Ȃ�
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


	//MinImageCount���擾
	/*
		catch�𐄏������O:
			NotFound
				->key��������Ȃ�
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

	//RenderPass�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����RenderPass�Ɋ��蓖�Ă�key
			parameter   �쐬����RenderPass�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createRenderPass(const char* key, const RenderPassParameter& parameter);


	//RenderPass�̍폜
	/*
	����:
		key �폜����RenderPass��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//DescriptorSetLayout�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����DescriptorSetLayout�Ɋ��蓖�Ă�key
			parameter   �쐬����DescriptorSetLayout�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createDescriptorSetLayout(const char* key, const DescriptorSetLayoutParameter& parameter);


	//UpdateDescriptorSet
	/*
		����:
			parameter   �p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			NotFound
				->Parameter��key������Ă���C���̊֐����ĂԂ܂łɐݒ肵��key��ύX�E�폜����
	*/
	FluidumVK_API void updateDescriptorSet(UpdateDescriptorSetParameter& parameter);


	//DescriptorSetLayout�̍폜
	/*
	����:
		key �폜����DescriptorSetLayout��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//ShaderModule�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����ShaderModule�Ɋ��蓖�Ă�key
			parameter   �쐬����ShaderModule�̃p�����[�^

		catch�𐄏������O:
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			NotInitialized
				->���������Ă��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createShaderModule(const char* key, const ShaderModuleParameter& parameter);


	//ShaderModule�̍폜
	/*
	����:
		key �폜����ShaderModule��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyShaderModule(const ShaderModuleKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ShaderModule>(key);
	}


	//ShaderFile�̒��g��ǂݍ���
	/*
		����:
			filePath �ǂݍ��ރt�@�C���̃p�X

		catch�𐄏������O:
			runtime_error
				->�t�@�C���̓ǂݍ��݂Ɏ��s
	*/
	FluidumVK_API _NODISCARD std::vector<char> readShaderFile(const char* filePath);

}

//GraphicsPipelineLayout
namespace FVK {

	//GraphicsPipelineLayout�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����GraphicsPipelineLayout�Ɋ��蓖�Ă�key
			parameter   �쐬����GraphicsPipelineLayout�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createGraphicsPipelineLayout(const char* key, GraphicsPipelineLayoutParameter& parameter);


	//GraphicsPipelineLayout�̍폜
	/*
	����:
		key �폜����GraphicsPipelineLayout��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//GraphicsPipeline�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����GraphicsPipeline�Ɋ��蓖�Ă�key
			parameter   �쐬����GraphicsPipeline�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createGraphicsPipeline(const char* key, GraphicsPipelineParameter& parameter);


	//GraphicsPipeline�̍폜
	/*
	����:
		key �폜����GraphicsPipeline��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//Image�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����Image�Ɋ��蓖�Ă�key
			parameter   �쐬����Image�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createImage(const char* key, const ImageParameter& parameter);


	//Image�̍폜
	/*
	����:
		key �폜����Image��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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
	catch�𐄏������O:
		NotFound
			->key��������Ȃ�
		FailedToCreate
			->bind�Ɏ��s
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

	//ImageView�̒ǉ��ƍ쐬
	/*
		����:
			key         �쐬����ImageView�Ɋ��蓖�Ă�key
			parameter   �쐬����ImageView�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createImageView(const char* key, ImageViewParameter& parameter);
	FluidumVK_API void createImageView(const char* key, const SwapchainImageViewParameter& parameter);


	//ImageView�̍폜
	/*
	����:
		key �폜����ImageView��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//DeviceMemory�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����DeviceMemory�Ɋ��蓖�Ă�key
			parameter   �쐬����DeviceMemory�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->���������蓖�ĂɎ��s
	*/
	FluidumVK_API void allocateDeviceMemory(const char* key, const DeviceMemoryParameter& parameter);


	//DeviceMemory�̉��
	/*
	����:
		key �폜����DeviceMemory��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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
	catch�𐄏������O:
		NotFound
			->key��������Ȃ�
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

	//CommandPool�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����CommandPool�Ɋ��蓖�Ă�key
			parameter   �쐬����CommandPool�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɐݒ肵��key��������Ȃ��D���蓖�ĂĂȂ��C�������́C�ύX�폜�����\��������D
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createCommandPool(const char* key, const CommandPoolParameter& parameter);


	//CommandPool�̔j��
	/*
	����:
		key �폜����CommandPool��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//VertexBuffer�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����VertexBuffer�Ɋ��蓖�Ă�key
			parameter   �쐬����VertexBuffer�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->Parameter��key������Ă���C���̊֐����ĂԂ܂łɐݒ肵��key��ύX�E�폜����
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createVertexBuffer(const char* key, const VertexBufferParameter& parameter);


	//VertexBuffer�̔j��
	/*
	����:
		key �폜����VertexBuffer��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//IndexBuffer�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����IndexBuffer�Ɋ��蓖�Ă�key
			parameter   �쐬����IndexBuffer�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->Parameter��key������Ă���C���̊֐����ĂԂ܂łɐݒ肵��key��ύX�E�폜����
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createIndexBuffer(const char* key, const IndexBufferParameter& parameter);


	//IndexBuffer�̔j��
	/*
	����:
		key �폜����IndexBuffer��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//UniformBuffer�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����UniformBuffer�Ɋ��蓖�Ă�key
			parameter   �쐬����UniformBuffer�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->Parameter��key������Ă���C���̊֐����ĂԂ܂łɐݒ肵��key��ύX�E�폜����
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createUniformBuffer(const char* key, const UniformBufferParameter& parameter);


	//UniformBuffer�̔j��
	/*
	����:
		key �폜����UniformBuffer��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
		FailedToErase
			->���̃A�C�e���Ŏg�p���Ă���̂ɍ폜���悤�Ƃ���
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

	//DescriptorPool�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����DescriptorPool�Ɋ��蓖�Ă�key
			parameter   �쐬����DescriptorPool�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->Parameter��key������Ă���C���̊֐����ĂԂ܂łɐݒ肵��key��ύX�E�폜����
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createDescriptorPool(const char* key, const DescriptorPoolParameter& parameter);


	//DescriptorPool�̔j��
	/*
	����:
		key �폜����DescriptorPool��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//DescriptorSet�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����DescriptorSet�Ɋ��蓖�Ă�key
			parameter   �쐬����DescriptorSet�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void allocateDescriptorSet(const char* key, DescriptorSetParameter& parameter);


	//DescriptorSet�̉��
	/*
	����:
		key �폜����DescriptorSet��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//CommandBuffer�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����CommandBuffer�Ɋ��蓖�Ă�key
			parameter   �쐬����CommandBuffer�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			std::bad_optional_access
				->parameter�ɕK�v��key���Z�b�g����Ă��Ȃ�
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void allocateCommandBuffers(const char* key, CommandBufferParameter& parameter);


	//CommandBuffer�̉��
	/*
	����:
		key �폜����CommandBuffer��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//FrameBuffer�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����FrameBuffer�Ɋ��蓖�Ă�key
			parameter   �쐬����FrameBuffer�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createFrameBuffer(const char* key, FrameBufferParameter& parameter);


	//FrameBuffer�̔j��
	/*
	����:
		key �폜����FrameBuffer��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//Semaphore�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����Semaphore�Ɋ��蓖�Ă�key
			parameter   �쐬����Semaphore�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			std::bad_optional_access
				->parameter�ɕK�v��key���Z�b�g����Ă��Ȃ�
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createSemaphore(const char* key, const SemaphoreParameter& parameter);


	//Semaphore�̔j��
	/*
	����:
		key �폜����Semaphore��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//Fence�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����Fence�Ɋ��蓖�Ă�key
			parameter   �쐬����Fence�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createFence(const char* key, const FenceParameter& parameter);


	//Fence�̔j��
	/*
	����:
		key �폜����Fence��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//Sampler�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����Sampler�Ɋ��蓖�Ă�key
			parameter   �쐬����Sampler�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createSampler(const char* key, const SamplerParameter& parameter);


	//Sampler�̔j��
	/*
	����:
		key �폜����Sampler��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//Texture�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����Sampler�Ɋ��蓖�Ă�key
			parameter   �쐬����Sampler�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createTexture(const char* key, const TextureParameter& parameter);


	//Texture�̔j��
	/*
	����:
		key �폜����Texture��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//ImGui�̒ǉ��Ɗ��蓖��
	/*
		����:
			key         �쐬����ImGui�Ɋ��蓖�Ă�key
			parameter   �쐬����ImGui�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API void createImGui(const char* key, const ImGuiParameter& parameter);


	//ImGui�̔j��
	/*
	����:
		key �폜����ImGui��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImGui(const ImGuiKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ImGui>(key);
	}


	//ImTextureID���쐬
	/*
		ImGui::Image()�̑������ɓn���Ɖ摜�̕`�ʂ��\

		����:
			key         �쐬����ImGui�Ɋ��蓖�Ă�key
			parameter   �쐬����ImGui�̃p�����[�^

		catch�𐄏������O:
			NotInitialized
				->���������Ă��Ȃ�
			CollisionOfKeys
				->key������key�Ɣ���Ă���
			NotFound
				->parameter�ɃZ�b�g����key�����݂��Ȃ�
			FailedToCreate
				->�쐬�Ɏ��s
	*/
	FluidumVK_API _NODISCARD ImTextureID createImGuiImage(const char* key, const ImGuiImageParameter& parameter);


	//ImGuiImage�̔j��
	/*
	����:
		key �폜����ImGuImage��key

	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
	*/
	template<Internal::Key::IsKeyType T>
	FluidumVK_API void destroyImGuiImage(const ImGuiImageKey<T>& key) {
		using namespace Internal;
		LockGuard lock(GMutex);
		API::checkManagerEmpty();
		GManager->erase<FvkType::ImGuiImage>(key);
	}


	//createImGui�Œǉ������t�H���g���擾
	/*
	����:
		key �擾����ImGui��key
	
	catch�𐄏������O:
		NotInitialized
			->���������Ă��Ȃ�
		NotFound
			->key���݂���Ȃ�
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

	//template<typename /*Internal::Key::IsKey �o�O*/ ...T>
	//FluidumVK_API _NODISCARD auto makeAnyCommand(const T&... keys) {
	//	using namespace Internal;
	//	LockGuard lock(GMutex);
	//	API::checkManagerEmpty();
	//	std::array<std::vector<IndexKey>, sizeof...(T)> indices
	//		= { std::vector<IndexKey>{Key::Converter::toIndexKey(keys)}... };

	//	return GManager->makeAnyCommand<T...>(indices);
	//}

	template<typename /*Internal::Key::IsKey �o�O*/ ...T>
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