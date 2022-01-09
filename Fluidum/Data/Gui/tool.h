#pragma once

#include "../Common/common.h"

//forward
namespace FD {
	class ToolBarWrite;
	class ToolBarRead;
}

namespace FD {
	namespace TopBar::Internal {
		struct Info final {
			std::function<void()> func = nullptr;
			FU::Class::ClassCode::CodeType code{};
			std::string sceneName{};
			bool select = false;
		};
		class Data final {
			static inline std::vector<Info> funcs{};
			static inline std::vector<UIF16> indices{};
		private:
			friend class ::FD::ToolBarWrite;
			friend class ::FD::ToolBarRead;
		};
	}
	namespace TopBar {
		using Internal::Info;
	}

	//single thread
	class ToolBarWrite final {
	public:
		ToolBarWrite(Internal::PassKey) {}
		~ToolBarWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ToolBarWrite);

	public:
		template<typename Scene>
		void add(void(Scene::* func)(), Scene* ptr, const std::string& name) const {
			using namespace TopBar::Internal;
			constexpr auto code = FU::Class::ClassCode::GetClassCode<Scene>();

			//already
			if (this->find<Scene>() != Data::funcs.end())
				return;

			Info info{
				.func = std::bind(func, ptr),
				.code = code,
				.sceneName = name
			};
			Data::funcs.emplace_back(std::move(info));
		}

		template<typename Scene>
		void remove() {
			using namespace TopBar::Internal;
			const auto itr = this->find<Scene>();
			const auto distance = std::distance(Data::funcs.begin(), itr);
			Data::funcs.erase(itr);

			auto [erase, eraseItr] = std::pair{ false ,Data::indices.end() };
			for (auto it = Data::indices.begin(), end = Data::indices.end(); it != end; it++) {
				if (*it == distance) {
					erase = true;
					eraseItr = it;
				}
				else if (*it > distance)
					(*it)--;
			}
			if (!erase)
				return;

			Data::indices.erase(eraseItr);
		}

		template<typename Scene>
		void lock() {
			using namespace TopBar::Internal;
			auto itr = this->find<Scene>();
			assert(itr != Data::funcs.end());
			itr->select = true;
			Data::indices.emplace_back(std::distance(Data::funcs.begin(), itr));
		}

		void lock(const FU::Class::ClassCode::CodeType code);

		template<typename Scene>
		void unlock() {
			using namespace TopBar::Internal;
			auto itr = this->find<Scene>();
			itr->select = false;
			assert(itr != Data::funcs.end());
			auto erase = std::find(Data::indices.begin(), Data::indices.end(), std::distance(Data::funcs.begin(), itr));
			Data::indices.erase(erase);
		}

		void unlock(const FU::Class::ClassCode::CodeType code);
	

	private:

		template<typename Scene>
		auto find() const {
			using namespace TopBar::Internal;
			return std::find_if(Data::funcs.begin(), Data::funcs.end(), [&](auto& x) {
				constexpr auto code = FU::Class::ClassCode::GetClassCode<Scene>();
				return x.code == code;
				}
			);
		}

	};

	class ToolBarRead final {
	public:
		ToolBarRead(Internal::PassKey) {}
		~ToolBarRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ToolBarRead);

	public:
		[[nodiscard]] const std::vector<TopBar::Info>* getInfo() const noexcept;
		[[nodiscard]] const std::vector<UIF16>* getIndices() const noexcept;
		void call(const UIF16 index) const;

	};

}