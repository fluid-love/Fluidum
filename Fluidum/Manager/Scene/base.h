#pragma once

#include "manager.h"

namespace FS::Internal {

	template<typename...Data>
	std::unique_ptr<Manager<Data...>> GManager = nullptr;

}

namespace FS::Internal {

	//���̃N���X��public�p������
	template<typename... Data>
	class SceneBase {
	public:
		SceneBase() = default;//���ۃN���X�Ȃ̂Ōp�������ɂ͍쐬�s��
		virtual ~SceneBase() = default;
		FluidumUtils_Class_Delete_CopyMove(SceneBase)

	public:
		virtual void call() = 0;

	public:

		//�V�[���̒ǉ�
		//�����V�[�������������ǉ������D�܂莟�̃��[�v����Ă΂�邱�ƂɂȂ�D
		template<IsSceneAble<Data...> Scene, typename... Args>
		void addScene(Args&&... args) {
			GManager<Data...>->addScene<Scene>(std::forward<Args>(args)...);
		}

		//�V�[���̒ǉ������݂�
		//���ɍ쐬����Ă���΁C�쐬���Ȃ�->return false
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

		//�񓯊��V�[���̒ǉ�
		//�����ɔ񓯊��Ƃ��Ċ֐����Ă΂��
		template<IsSceneAble<Data...> Scene, typename... Args>
		void addAsyncScene(Args&&... args) {
			GManager<Data...>->addAsyncScene<Scene>(std::forward<Args>(args)...);
		}

		//�V�[���̍폜
		//�����V�[���̈�����I��������Ƃɏ������
		//�����Ɏ��̏����ւƈڂ�
		template<IsSceneAble<Data...> Scene>
		void deleteScene() {
			GManager<Data...>->addDeleteCode<Scene>();
		}

		//�V�[���̍폜�����݂�
		//���łɍ폜�i�폜����Ă��Ȃ��j�ꍇ�́C�폜���Ȃ�->return false
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

		//�񓯊��V�[���̍폜
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

		//const�ȃf�[�^�|�C���^��Ԃ�
		//�ύX�������ꍇ�̓R���X�g���N�^�Ńf�[�^���擾���邱��
		template<FD::IsDataAble T>
		_NODISCARD const auto* getData() {
			static_assert(std::is_pointer_v<T>);
			//pointer�ɂ���unique_ptr���m�F�D�Ⴄ�̂ł���Ζ߂�
			constexpr auto index = FU::Tuple::getSameTypeIndex<std::tuple<Data...>, T>();
			return GManager<Data...>->data.get<index>();
		}

		//data���쐬����Ă��邩
		template<FD::IsDataAble T>
		_NODISCARD bool isDataCreated() const {
			static_assert(std::is_pointer_v<T>);
			constexpr auto index = FU::Tuple::getSameTypeIndex<std::tuple<Data...>, T>();
			return GManager<Data...>->data.isDataCreated<index>();
		}

	};

}