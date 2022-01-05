#pragma once

#include "command.h"

namespace FVK::Internal::Command {

	struct CommandBlock final {
		CommandType type = CommandType::None;
		std::shared_ptr<void> element = nullptr;
		std::function<void()> function = []() {};
	};

	class FunctionIndex final {
	private:
		UIF32 index = 0;
	public:
		//index++;
		void operator++(int) noexcept;

		[[nodiscard]] operator UIF32() const noexcept;
		void operator=(const UIF32) noexcept;
	};

}

namespace FVK::Internal::Command {

	//thread safe
	class Commands final : protected std::vector<CommandBlock> {
	private://using
		using Base = std::vector<CommandBlock>;
		using Block = Base::value_type;
		using Blocks = Base;
		using ConstIterator = Blocks::const_iterator;

	public://using
		using Pos = uint32_t;

	public:
		FluidumUtils_Class_Default_ConDestructor(Commands)

	private:
		//�`�ʏ����ȏ�̖��߂�����ƈӐ}���Ȃ����Ƃ��N����D
		//��->updateBuffer���A�v�����ŘA���œ�����up->up->up->�`��->up->up�݂����ɂȂ��Ċ��炩�ɓ����Ȃ�
		std::condition_variable condition{};

		FunctionIndex index = {};//���ǂ��̊֐����w���Ă��邩��\��


	public://main function
		void call();

	private:
		void internalCall();

	public:
		template<CommandType Type>
		void push(const Command<Type>& command) {
			Block block{ Type, command.ptr, std::bind(&Command<Type>::Element::call, command.ptr) };
			LockGuard lock(GMutex);
			Blocks::emplace_back(std::move(block));
		}

		//any command
		template<FvkType ...T>
		void push(const AnyCommand<T...>& command) {
			using Ty = AnyCommand<T...>;
			CommandBlock block{ CommandType::Any,command.ptr, std::bind(&Ty::Element::call, command.ptr) };
			LockGuard lock(GMutex);
			Blocks::emplace_back(std::move(block));
		}

	};

	//future
	//	private:
	//		vector<ui32> onceIndices = {};//��x��������������R�}���h���L�^����D�������i�����ׂ��jfunctionIndex , ����commandIndex ,...
	//

	//
	//		//Commadns���R�}���h�Ƃ��ē���邱�Ƃ��ł���悤�ɂ���
	//		//Commands�̈ʒu,Commands�̖{��
	//		vector<std::pair<Pos, Commands>> internalCommands = {};
	//

	//
	//	private:
	//		//functionIndex onetime
	//		void adjustIndexPlus(ConstIterator pos);
	//		void adjustIndexPlus(ConstIterator pos, const uint16_t size);
	//
	//		void adjustIndexMinus(ConstIterator pos);
	//
	//		ui32 getNextFunctionIndex() const;
	//
	//		vector<ui32> getIndicesInternal(const CommandType type);
	//
	//		ConstIterator toItr(const Pos pos) const noexcept;
	//
	//		void internalCall();
	//	public:
	//
	//		template<CommandTypeInternal Type>
	//		void push(const Command<Type>& command) {
	//			CommandBlock block{ Type, command.data,std::bind(&Command<Type>::Data::call, command.data) };
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//			VecType::emplace_back(std::move(block));
	//		}
	//
	//		template<typename ...T>
	//		void push(const CommandAny<T...>& command) {
	//			using Ty = CommandAny<T...>;
	//			CommandBlock block{ CommandTypeInternal::ANY,command.data,std::bind(&Ty::Data::call, command.data) };
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//			VecType::emplace_back(std::move(block));
	//		}
	//
	//		template<CommandTypeInternal Type>
	//		void pushFront(const Command<Type>& command) {
	//			CommandBlock block{ Type, command.data,std::bind(&Command<Type>::Data::call, command.data) };
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//			VecType::insert(VecType::begin(), std::move(block));
	//		}
	//
	//		template<typename ...T>
	//		void pushFront(const CommandAny<T...>& command) {
	//			using Ty = CommandAny<T...>;
	//			CommandBlock block{ CommandTypeInternal::NONE,command.data,std::bind(&Ty::Data::call, command.data) };
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//			VecType::insert(VecType::begin(), std::move(block));
	//		}
	//
	//		//type�Ŏw�肵��command�̃C���f�b�N�X���擾
	//		_NODISCARD vector<ui32> getIndices(const CommandType type);
	//
	//		template<CommandType Type>
	//		_NODISCARD auto at(const Pos pos) {
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//			auto posItr = toItr(pos);
	//			//check type
	//			if (static_cast<CommandTypeInternal>(Type) != posItr->type)
	//				throw std::runtime_error("Type Error");
	//
	//			using DataType = CommandElement<static_cast<CommandTypeInternal>(Type)>;
	//			auto shared = std::static_pointer_cast<DataType>(posItr->data);
	//			return Command<static_cast<CommandTypeInternal>(Type)>(std::move(shared));
	//		}
	//
	//		/*
	//		functionIndex = 4
	//		0 1 2 3 4
	//		a b c E d
	//
	//		insert
	//
	//		functionIndex = 4���ƕs�s�� ++1����
	//		0 1 2 3 4 5
	//		a b A c E d
	//		*/
	//		//concept issue typename -> IsCommandType
	//		template<typename... T>
	//		void insert(const Pos pos, const T&... commands) {
	//			std::array<CommandBlock, sizeof...(T)> blocks{
	//				CommandBlock{
	//				T::CType,
	//				commands.data,
	//				std::bind(&T::Data::call, commands.data)
	//			}
	//			... };
	//
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//			auto posItr = toItr(pos);
	//			adjustIndexPlus(posItr, sizeof...(T));
	//			VecType::insert(posItr, blocks.begin(), blocks.end());
	//		}
	//
	//		void insert(const Pos pos, const ::FVK::Commands& command);
	//
	//		//���̃R�}���h�͈��s��ꂽ��C�������D
	//		//template<CommandTypeInternal Type>
	//		//void insertOnce(ConstIterator pos, const Command<Type>& command) {
	//			//CommandBlock block;
	//			//block.type = Type;
	//			//block.data = command.ptr;
	//			//block.function = std::bind(&Command<Type>::Data::call, command.ptr);
	//
	//			//CommandMutex::Lock lock(CommandMutex::mtx);
	//
	//			//this->onceIndices.emplace_back(this->getNextFunctionIndex());
	//
	//			//adjustIndexPlus(pos);
	//			//std::size_t distance = std::distance(VecType::cbegin(), pos);
	//			//VecType::insert(pos, std::move(block));
	//
	//			//this->onceIndices.emplace_back(distance);
	//		//}
	//
	//		//�͂��߂��猟������BeforeType�Ɉ�v������̂�����΂��̌�ɑ}�������
	//		//���̃R�}���h�͈��s��ꂽ��C�������D
	//		template<CommandTypeInternal Type>
	//		void insertOnceAfter(const CommandType BeforeType, const Command<Type>& command) {
	//			//CommandBlock block;
	//			//block.type = Type;
	//			//block.data = command.ptr;
	//			//block.function = std::bind(&Command<Type>::Data::call, command.ptr);
	//
	//			//CommandMutex::Lock lock(CommandMutex::mtx);
	//
	//			//this->onceIndices.emplace_back(this->getNextFunctionIndex());
	//
	//			//bool find = false;
	//			//std::size_t distance;
	//			//for (auto itr = VecType::cbegin(); itr != VecType::cend(); itr++) {
	//			//	if (itr->type == static_cast<CommandTypeInternal>(BeforeType)) {
	//			//		adjustIndexPlus(itr + 1);
	//			//		distance = std::distance(VecType::cbegin(), itr);
	//			//		VecType::insert(itr + 1, std::move(block));
	//			//		this->onceIndices.emplace_back(distance + 1);
	//			//		find = true;
	//			//		break;
	//			//	}
	//			//}
	//
	//			//if (!find) {
	//			//	this->onceIndices.pop_back();
	//			//	this->onceIndices.pop_back();
	//			//	throw std::runtime_error("");
	//			//}
	//		}
	//
	//		//call�܂�wait����
	//		//call��thread����Ă΂Ȃ�����
	//		template<CommandTypeInternal Type>
	//		void insertOnceAfterCurrentPos(const CommandType beforeType, const vector<Command<Type>>& commands) {
	//			bool find = false;
	//
	//			CommandMutex::ULock lock(CommandMutex::mtx);
	//			condition.wait(lock);
	//			ui32 current = 0;
	//
	//			std::vector<ui32> functionIndices;
	//			{
	//				auto size = VecType::size();
	//				for (ui32 i = 0; i < size; i++) {
	//					if (VecType::operator[](i).type == CommandTypeInternal::NEXT) {
	//						if (i == size - 1)
	//							functionIndices.emplace_back(0);
	//						else
	//							functionIndices.emplace_back(i + 1);
	//					}
	//				}
	//				auto temp = functionIndices;
	//				if (temp.back() == 0) {
	//					temp.pop_back();
	//					temp.insert(temp.begin(), 0);
	//				}
	//				for (ui32 i = 0; auto & x : temp) {
	//					if (x == functionIndex) {
	//						current = i;
	//						break;
	//					}
	//					i++;
	//				}
	//				assert(commands.size() == functionIndices.size());
	//			}
	//
	//			auto ins = [&](ConstIterator itr) {
	//				CommandBlock block;
	//				block.type = Type;
	//				block.data = commands.at(current).data;
	//				block.function = std::bind(&Command<Type>::Data::call, commands.at(current).data);
	//				std::size_t distance = std::distance(VecType::cbegin(), itr);
	//
	//				this->onceIndices.emplace_back(functionIndices[current]);
	//				adjustIndexPlus(itr);
	//
	//				VecType::insert(itr, std::move(block));
	//				this->onceIndices.emplace_back(distance);
	//				find = true;
	//			};
	//
	//			for (auto itr = VecType::cbegin() + functionIndex; itr != VecType::cend(); itr++) {
	//				//type����������
	//				if (itr->type == static_cast<CommandTypeInternal>(beforeType))
	//				{
	//					ins(itr + 1);
	//					break;
	//				}
	//				else if (itr->type == CommandTypeInternal::NEXT) {
	//					current++;
	//					if (current >= commands.size())
	//						current = 0;
	//				}
	//			}
	//			if (find);
	//			else {
	//				for (auto itr = VecType::cbegin(); itr != VecType::cend() - functionIndex; itr++) {
	//					if (itr->type == static_cast<CommandTypeInternal>(beforeType))
	//					{
	//						ins(itr + 1);
	//						break;
	//					}
	//					else if (itr->type == CommandTypeInternal::NEXT)
	//						current++;
	//				}
	//			}
	//		}
	//
	//
	//		//���݂̈ʒu����beforeType�̌�ɃR�}���h������D
	//		//call�Ɠ���thread����Ă΂Ȃ�����->wait���I���Ȃ�
	//		//��������beforeType�̂��Ƃɓ����R�}���h������Ύ���beforeType�̌�ɓ����
	//		//�S�Ė��܂��Ă�����false��Ԃ�
	//		//���̃R�}���h�͈��s��ꂽ��C�������D
	//		//��ڂɂ����vector::at(1)...
	//		//UpdateVertexBuffer��UpdateIndexBuffer���s����
	//		template<CommandTypeInternal Type >
	//		void insertOnceAfterCurrentPosIfNotAlready(const CommandType beforeType, const vector<Command<Type>>& commands) {
	//			bool find = false;
	//
	//			CommandMutex::ULock lock(CommandMutex::mtx);
	//			condition.wait(lock);
	//			ui32 current = 0;
	//
	//			std::vector<ui32> functionIndices;
	//			{
	//				auto size = VecType::size();
	//				for (ui32 i = 0; i < size; i++) {
	//					if (VecType::operator[](i).type == CommandTypeInternal::NEXT) {
	//						if (i == size - 1)
	//							functionIndices.emplace_back(0);
	//						else
	//							functionIndices.emplace_back(i + 1);
	//					}
	//				}
	//				auto temp = functionIndices;
	//				if (temp.back() == 0) {
	//					temp.pop_back();
	//					temp.insert(temp.begin(), 0);
	//				}
	//				for (ui32 i = 0; auto & x : temp) {
	//					if (x == functionIndex) {
	//						current = i;
	//						break;
	//					}
	//					i++;
	//				}
	//				assert(commands.size() == functionIndices.size());
	//			}
	//
	//			auto ins = [&](ConstIterator itr) {
	//				CommandBlock block;
	//				block.type = Type;
	//				block.data = commands.at(current).data;
	//				block.function = std::bind(&Command<Type>::Data::call, commands.at(current).data);
	//				std::size_t distance = std::distance(VecType::cbegin(), itr);
	//
	//				this->onceIndices.emplace_back(functionIndices[current]);
	//				adjustIndexPlus(itr);
	//
	//				VecType::insert(itr, std::move(block));
	//				this->onceIndices.emplace_back(distance);
	//				find = true;
	//			};
	//
	//			for (auto itr = VecType::cbegin() + functionIndex; itr != VecType::cend(); itr++) {
	//				//type���������� &&  ���ɂȂ�
	//				if (itr->type == static_cast<CommandTypeInternal>(beforeType) &&
	//					((itr == VecType::cend() - 1 ? VecType::cbegin()->type : (itr + 1)->type) != Type))//�ŏI�̗v�f�̓I�[�o�[����̂ł͂��߂̂Ɣ�r
	//				{
	//					ins(itr + 1);
	//					break;
	//				}
	//				else if (itr->type == CommandTypeInternal::NEXT) {
	//					current++;
	//					if (current >= commands.size())
	//						current = 0;
	//				}
	//			}
	//			if (find);
	//			else {
	//				for (auto itr = VecType::cbegin(); itr != VecType::cend() - functionIndex; itr++) {
	//					if (itr->type == static_cast<CommandTypeInternal>(beforeType) &&
	//						((itr == VecType::cend() - 1 ? VecType::cbegin()->type : (itr + 1)->type) != Type))//�ŏI�̗v�f�̓I�[�o�[����̂ł͂��߂̂Ɣ�r
	//					{
	//						ins(itr + 1);
	//						break;
	//					}
	//					else if (itr->type == CommandTypeInternal::NEXT)
	//						current++;
	//				}
	//			}
	//		}
	//
	//
	//
	//		ui32 size();
	//
	//		void erase(const Pos pos);
	//
	//		//type�̃R�}���h��S�ď���
	//		template<CommandType... Types>
	//		void erase() {
	//			CommandMutex::Lock lock(CommandMutex::mtx);
	//
	//			for (auto& x : { Types... }) {
	//				auto eraseIndices = this->getIndicesInternal(x);
	//
	//				std::reverse(eraseIndices.begin(), eraseIndices.end());
	//
	//				for (auto& y : eraseIndices) {
	//					adjustIndexMinus(VecType::begin() + y);
	//					VecType::erase(VecType::begin() + y);
	//				}
	//			}
	//		}
	//
	//		void pop_back();
	//
	//		bool emptyOneTime();
	//
	//		Pos getCurrentPos();
	//
	//		Pos getNextIndex();
	//

	//
	//	};
}