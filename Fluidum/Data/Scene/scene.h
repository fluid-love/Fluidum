#pragma once

#include "../Common/common.h"

//forward
namespace FD {
	class SceneRead;
	class ProjectWrite;
	class ProjectRead;
}

namespace FD {
	namespace Internal::Scene {
		class Data final {
		public://callback
			static void addSceneCallback(bool async, FU::Class::ClassCode::CodeType code);
			static void deleteSceneCallback(bool async, FU::Class::ClassCode::CodeType code);

		private:
			static inline std::vector<FU::Class::ClassCode::CodeType> codes{};
			static inline std::mutex mtx{};
			static inline std::atomic_bool save = false;
		private:
			friend class SceneRead;
			friend class ProjectWrite;
			friend class ProjectRead;
		};
	}
	namespace Scene {
		using CallBacks = ::FD::Internal::Scene::Data;
	}

}

namespace FD {

	class SceneRead final {
	public:
		explicit SceneRead(Internal::PassKey) noexcept {};
		~SceneRead() = default;
		FluidumUtils_Class_Delete_CopyMove(SceneRead)

	private:
		using CodeType = FU::Class::ClassCode::CodeType;

	public:

		//ÉVÅ[ÉìÇ™ë∂ç›Ç∑ÇÍÇŒtrue
		template<typename Scene>
		_NODISCARD bool isExist() const noexcept {
			using namespace Internal::Scene;
			std::lock_guard<std::mutex> lock(Data::mtx);
			constexpr CodeType code = FU::Class::ClassCode::GetClassCode<Scene>();
			const auto itr = std::find(Data::codes.cbegin(), Data::codes.cend(), code);
			return itr != Data::codes.cend();
		}

		template<FU::Class::ClassCode::CodeType Code>
		_NODISCARD bool isExist() const noexcept {
			using namespace Internal::Scene;
			std::lock_guard<std::mutex> lock(Data::mtx);
			const auto itr = std::find(Data::codes.cbegin(), Data::codes.cend(), Code);
			return itr != Data::codes.cend();
		}

		_NODISCARD bool isExist(const FU::Class::ClassCode::CodeType Code) const noexcept;
	

	};

}
