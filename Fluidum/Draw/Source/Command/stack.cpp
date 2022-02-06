#include "stack.h"
#include "../Api/api.h"

namespace FDR::Command::Internal {

	struct Pos final {
		IF32 x{};
		IF32 y{};
		void call() {
			::FDR::setWindowPos(this->x, this->y);
		}
	};

	struct Size final {
		IF32 width{};
		IF32 height{};
		void call() {
			::FDR::setWindowSize(this->width, this->height);
		}
	};

	struct PosSize final {
		IF32 x{};
		IF32 y{};
		IF32 width{};
		IF32 height{};
		void call() {
			::FDR::setWindowPosSize(this->x, this->y, this->width, this->height);
		}
	};

	template<typename... T>
	class StackCommand final {
	public:
		FluidumUtils_Class_Default_ConDestructor_Noexcept(StackCommand);
		FluidumUtils_Class_Delete_CopyMove(StackCommand);

	public:
		void call() {
			if (this->none)
				return;
			this->none = true;

			FU::Tuple::for_each(this->stack, [](auto& x)
				{
					if (!x.has_value())
						return;

					x->call();
					x = std::nullopt;
				}
			);
		}

	public:
		template<typename U>
		void set(const U& elm) {
			static_assert(FU::Concept::IsSameOr<U, T...>);
			std::get<std::optional<U>>(this->stack) = elm;
			this->none = false;
		}

	private:
		std::tuple<std::optional<T>...> stack{};
		bool none = true;

	};
	StackCommand<Pos, Size, PosSize> GStackCommand{};

}

void FDR::Command::Internal::call() {
	GStackCommand.call();
}

void FDR::Command::Internal::setSetPos(const IF32 x, const IF32 y) noexcept {
	const Pos pos{ x, y };
	GStackCommand.set(pos);
}

void FDR::Command::Internal::setSetSize(const IF32 width, const IF32 height) noexcept {
	const Size size{ width, height };
	GStackCommand.set(size);
}

void FDR::Command::Internal::setSetPosSize(const IF32 x, const IF32 y, const IF32 width, const IF32 height) noexcept {
	const PosSize posSize{ x, y, width, height };
	GStackCommand.set(posSize);
}


