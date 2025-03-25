#pragma once

#include "../../Common/common.h"

namespace FD::Calc {
	enum class Mode : uint8_t {
		Debug,
		Normal
	};
}

//forward
namespace FD {
	class CalcWrite;
	class CalcRead;
	class ProjectWrite;
}

namespace FD::Calc::Internal {
	struct Data final {
		FluidumUtils_Class_Delete_ConDestructor(Data);
	private:
		static inline Calc::Mode mode = Calc::Mode::Debug;
		static inline bool first = true;

		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;
	private:
		friend class ::FD::CalcWrite;
		friend class ::FD::CalcRead;
		friend class ::FD::ProjectWrite;
	};
}

namespace FD {

	class CalcWrite final {
	public:
		explicit CalcWrite(Internal::PassKey) noexcept {};
		~CalcWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(CalcWrite);

	public:
		void mode(const Calc::Mode val) const;

		void setFirst(const bool val) const;

	public:
		void save() const noexcept;
	};

	class CalcRead final {
	public:
		explicit CalcRead(Internal::PassKey) noexcept {};
		~CalcRead() = default;
		FluidumUtils_Class_Delete_CopyMove(CalcRead);

	public:
		[[nodiscard]] Calc::Mode mode() const;
		[[nodiscard]] bool first() const;

	};



}