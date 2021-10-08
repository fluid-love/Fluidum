#pragma once

#include "lua.h"
#include "../../Libraries/lua-5.4.3/src/lua.hpp"

namespace FD::Calc {
	enum class Language : uint8_t {
		Lua,
		Python,
		AngelScript
	};

}

//forward
namespace FD {
	template<Calc::Language>
	class FunctionWrite;
	template<Calc::Language>
	class FunctionRead;
}

namespace FD::Calc::Internal {

	class FunctionData final {
	private:
		static inline std::mutex mtx{};

	private:
		template<::FD::Calc::Language>
		friend class FunctionWrite;
		template<::FD::Calc::Language>
		friend class FunctionRead;
	};

}

namespace FD {

	template<Calc::Language>
	class FunctionWrite final {
		FluidumUtils_Class_Delete_ConDestructor(FunctionWrite)
	};
	template<Calc::Language>
	class FunctionRead final {
		FluidumUtils_Class_Delete_ConDestructor(FunctionRead)
	};

	//一定回数のpushでファイルへ出力
	template<>
	class FunctionWrite<Calc::Language::Lua> final {
	public:
		explicit FunctionWrite(Internal::PassKey) noexcept {};
		~FunctionWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(FunctionWrite)


	private:
		using Type = Calc::Lua::FunctionType;
		using FData = Calc::Internal::FunctionData;


		static inline Calc::Internal::FunctionContainer data{};

	public:
		//puah_back
		template<Calc::Lua::FunctionType FType, Calc::Lua::IsArgType... Arg>
		void push(const Calc::Lua::FunctionType type, Arg&&...args) {
			std::lock_guard<std::mutex> lock(FData::mtx);
			data.push(Calc::Lua::Internal::RetArgInfo<FType>{std::forward<Arg>(args)...});			
		}




	private:
		template<::FD::Calc::Language>
		friend class FunctionRead;
	};

	template<>
	class FunctionRead<FD::Calc::Language::Lua> final {
	public:
		explicit FunctionRead(Internal::PassKey) noexcept {};
		~FunctionRead() = default;
		FluidumUtils_Class_Delete_CopyMove(FunctionRead)

	public:

	};




}
