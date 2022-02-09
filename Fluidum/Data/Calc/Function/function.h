#pragma once

#include "lua.h"
#include "../../External/lua-5.4.4/src/lua.hpp"

namespace FD::Calc {
	enum class Language : uint8_t {
		None,
		Lua,
		Python,
		AngelScript
	};

	//.lua .py .as 
	inline Language pathToLanguageType(const std::string& path) {
		std::filesystem::path p = path;
		const std::string extension = p.extension().string();
		if (extension == ".lua")
			return Language::Lua;
		else if (extension == ".py")
			return Language::Python;
		else if (extension == ".as" || extension == ".AS" )
			return Language::AngelScript;
		return Language::None;
	}
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
		FluidumUtils_Class_Delete_ConDestructor(FunctionWrite);
	};
	template<Calc::Language>
	class FunctionRead final {
		FluidumUtils_Class_Delete_ConDestructor(FunctionRead);
	};

	//一定回数のpushでファイルへ出力
	template<>
	class FunctionWrite<Calc::Language::Lua> final {
	public:
		explicit FunctionWrite(Internal::PassKey) noexcept {};
		~FunctionWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(FunctionWrite);


	private:
		using Type = Calc::Lua::FunctionType;
		using FData = Calc::Internal::FunctionData;


		static inline Calc::Internal::FunctionContainer<FluidumData_Lua_ContainerTemplateArgs> data{};

	public:
		//puah_back
		template<Calc::Lua::FunctionType FType, Calc::Lua::IsArgType... Arg>
		void push(Arg&&...args) {
			std::lock_guard<std::mutex> lock(FData::mtx);
			data.push(Calc::Lua::Internal::RetArgInfo<FType>{std::forward<Arg>(args)...});
		}

		_NODISCARD bool empty() {
			std::lock_guard<std::mutex> lock(FData::mtx);
			return data.empty();
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
		FluidumUtils_Class_Delete_CopyMove(FunctionRead);

	public:

	};




}
