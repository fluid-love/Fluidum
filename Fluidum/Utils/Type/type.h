#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <list>

#define FluidumUtils_Type_Internal_Namespace(name) \
namespace name {								   \
												   \
	using I8 = std::int8_t;						   \
	using I16 = std::int16_t;					   \
	using I32 = std::int32_t;					   \
	using I64 = std::int64_t;					   \
	using UI8 = std::uint8_t;					   \
	using UI16 = std::uint16_t;					   \
	using UI32 = std::uint32_t;					   \
	using UI64 = std::uint64_t;					   \
												   \
	using IF8 = std::int_fast8_t;				   \
	using IF16 = std::int_fast16_t;				   \
	using IF32 = std::int_fast32_t;				   \
	using IF64 = std::int_fast64_t;				   \
	using UIF8 = std::uint_fast8_t;				   \
	using UIF16 = std::uint_fast16_t;			   \
	using UIF32 = std::uint_fast32_t;			   \
	using UIF64 = std::uint_fast64_t;			   \
												   \
	using Size = std::size_t;	       		       \
	using ISize = std::make_signed_t<Size>;		   \
												   \
	using IMax = std::intmax_t;					   \
	using UImax = std::uintmax_t;				   \
}												   
												   
namespace FU::Type {							   
	template<typename T>						   
	using VecSize = std::vector<T>::size_type;	   
												   
	/*https://eel.is/c++draft/array*/			   
	/*std::array<T, N>::size_type = size_t*/	   
	using ArrSize = std::size_t;				   
												   
	using StrSize = std::string::size_type;		   
}

FluidumUtils_Type_Internal_Namespace(FS)
FluidumUtils_Type_Internal_Namespace(FD)
FluidumUtils_Type_Internal_Namespace(FU)
FluidumUtils_Type_Internal_Namespace(FDR)
FluidumUtils_Type_Internal_Namespace(FVK)
FluidumUtils_Type_Internal_Namespace(FKM)