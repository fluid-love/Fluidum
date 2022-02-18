#pragma once

#include "../Common/common.h"

//forward
namespace FD {
	class SceneRead;
	class ProjectWrite;
	class ProjectRead;
}

namespace FD::Scene {
	namespace Internal {
		class Data final {
		public://callback
			static void addSceneCallback(bool async, FU::Class::ClassCode::CodeType code);
			static void deleteSceneCallback(bool async, FU::Class::ClassCode::CodeType code);

		private:
			static inline std::vector<FU::Class::ClassCode::CodeType> codes{};
			static inline std::mutex mtx{};
			static inline std::atomic_bool save = false;
			static inline std::atomic_bool erase = false;
		private:
			friend class SceneRead;
			friend class ProjectWrite;
			friend class ProjectRead;
		};
	}

	using CallBacks = ::FD::Scene::Internal::Data;

}

namespace FD {

	class SceneRead final {
	public:
		explicit SceneRead(Internal::PassKey) noexcept {};
		~SceneRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(SceneRead);

	private:
		using CodeType = FU::Class::ClassCode::CodeType;

	public:
		template<typename Scene>
		[[nodiscard]] bool exist() const noexcept {
			using namespace ::FD::Scene::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);
			constexpr CodeType code = FU::Class::ClassCode::GetClassCode<Scene>();
			const auto itr = std::find(Data::codes.cbegin(), Data::codes.cend(), code);
			return itr != Data::codes.cend();
		}

		/*
		Note:
			[scene1 exists] [scene2 exists]   -> true
			[scene1 exists] [!scene2 exists]  -> true
			[!scene1 exists] [!scene2 exists] -> false
		*/
		template<typename... Scene>
		[[nodiscard]] bool exist_or() const noexcept {
			using namespace ::FD::Scene::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);
			constexpr std::array<CodeType, sizeof...(Scene)> codes = { FU::Class::ClassCode::GetClassCode<Scene>()... };

			for (auto x : codes) {
				const auto itr = std::find(Data::codes.crbegin(), Data::codes.crend(), x);
				if (itr != Data::codes.crend())
					return true;
			}
			return false;
		}

		/*
		Note:
			[scene1 exists] [scene2 exists]   -> true
			[scene1 exists] [!scene2 exists]  -> false
			[!scene1 exists] [!scene2 exists] -> false
		*/
		template<typename... Scene>
		[[nodiscard]] bool exist_and() const noexcept {
			using namespace ::FD::Scene::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);
			constexpr std::array<CodeType, sizeof...(Scene)> codes = { FU::Class::ClassCode::GetClassCode<Scene>()... };

			for (auto x : codes) {
				const auto itr = std::find(Data::codes.crbegin(), Data::codes.crend(), x);
				if (itr == Data::codes.crend())
					return false;
			}
			return true;
		}

		template<FU::Class::ClassCode::CodeType Code>
		[[nodiscard]] bool exist() const noexcept {
			using namespace ::FD::Scene::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);
			const auto itr = std::find(Data::codes.crbegin(), Data::codes.crend(), Code);
			return itr != Data::codes.crend();
		}

		[[nodiscard]] bool exist(const FU::Class::ClassCode::CodeType Code) const noexcept;

	public:
		/*
		Return:
			running   : true
			otherwise : false
		Exception: no-throw
		*/
		[[nodiscard]] bool running() const noexcept;

	};

}
