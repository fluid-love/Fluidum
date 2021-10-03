#pragma once

#include <iostream>
#include <map>
#include <CppGA.h>

#include "property/FDproperty.h"
#include "../common/FDcommon.h"

#include <FluidumTuple/FluidumTuple.h>
#include <FluidumConcept/FluidumConcept.h>
#include <FluidumClass/FluidumClass.h>

//前方宣言
namespace FD {
	class GenomeRead;
}

#define FluidumData_Genome_CallFunctions(Name)\
Name<::FD::Genome::BinType>,Name<::FD::Genome::OneByteType>,Name<::FD::Genome::Int32Type>,Name<::FD::Genome::UInt32Type>,Name<::FD::Genome::FloatType>,Name<::FD::Genome::DblType>


namespace FD {

	namespace Genome {
		enum class Type : uint16_t {
			NONE,
			BINARY,
			ONE_BYTE,
			INT32,
			UINT32,
			FLOAT,
			DOUBLE
		};

		using BinType = bool;
		using OneByteType = uint8_t;
		using Int32Type = int32_t;
		using UInt32Type = uint32_t;
		using FloatType = float;
		using DblType = double;


		template<typename T>
		using Genomes = std::vector<CppGA::Genome<T>>;

		template<typename T>
		using TempGenomes = std::map<std::size_t, CppGA::Genome<T>>;

		template<typename T>
		concept IsGenomeType =
			std::same_as<T, BinType> ||
			std::same_as<T, OneByteType> ||
			std::same_as<T, Int32Type> ||
			std::same_as<T, UInt32Type> ||
			std::same_as<T, FloatType> ||
			std::same_as<T, DblType>;

		namespace Internal {

			//Funcsが関数で第一引数にGenome::Genomes<T>であること
			template<auto...Funcs>
			struct CheckFunctionFirstArgGenomes final {
			private:

				template<auto Func, std::size_t Index>
				static consteval bool isSuitable() {
					using namespace Fluidum::Utils;
					if constexpr (Index == 0) {
						return std::same_as<
							Concept::remove_all_t<
							std::tuple_element_t<0, Tuple::FunctionArgsToTupleType<Func>>
							>, Genome::Genomes<BinType>
						>;
					}
					else if constexpr (Index == 1) {
						return std::same_as<
							Concept::remove_all_t<
							std::tuple_element_t<0, Tuple::FunctionArgsToTupleType<Func>>
							>, Genome::Genomes<OneByteType>
						>;
					}
					else if constexpr (Index == 2) {
						return std::same_as<
							Concept::remove_all_t<
							std::tuple_element_t<0, Tuple::FunctionArgsToTupleType<Func>>
							>, Genome::Genomes<Int32Type>
						>;
					}
					else if constexpr (Index == 3) {
						return std::same_as<
							Concept::remove_all_t<
							std::tuple_element_t<0, Tuple::FunctionArgsToTupleType<Func>>
							>, Genome::Genomes<UInt32Type>
						>;
					}
					else if constexpr (Index == 4) {
						return std::same_as<
							Concept::remove_all_t<
							std::tuple_element_t<0, Tuple::FunctionArgsToTupleType<Func>>
							>, Genome::Genomes<FloatType>
						>;
					}
					else if constexpr (Index == 5) {
						return std::same_as<
							Concept::remove_all_t<
							std::tuple_element_t<0, Tuple::FunctionArgsToTupleType<Func>>
							>, Genome::Genomes<DblType>
						>;
					}

					return false;
				}

				template<typename T, T... Index>
				static consteval bool isSuitableFunctions(std::integer_sequence<T, Index...>) {
					std::array<bool, sizeof...(Funcs)> results = { isSuitable<Funcs,Index>()... };
					const auto itr = std::find(results.begin(), results.end(), false);
					return (itr == results.end());
				}

			public:
				static constexpr inline bool value = isSuitableFunctions(std::make_integer_sequence<std::size_t, sizeof...(Funcs)>());
			};

			template<auto... Funcs>
			concept IsFunctionFirstArgGenomes = CheckFunctionFirstArgGenomes<Funcs...>::value;
		}
	}

	class GenomeWrite final : protected Fluidum::Utils::Class::Lock {
	public:
		explicit GenomeWrite(Internal::ManagerKey) {};
#ifdef FluidumData_Genome_Debug//debug
		GenomeWrite() {};
#endif
		~GenomeWrite() = default;
		FluidumUtils_Class_NoCopyNoMove(GenomeWrite)

	public:
		using Lock::getLock;
	private:
		static inline Genome::Type type = Genome::Type::NONE;
	public:
		void initType(Genome::Type type) {
			if (this->type != Genome::Type::NONE || type == Genome::Type::NONE)
				throw std::runtime_error("");

			this->type = type;
		}

		Genome::Type genomeType() {
			return this->type;
		}



		//メンバ関数
		template<auto FuncBin, auto FuncOneByte, auto FuncInt32, auto FuncUInt32, auto FuncFloat, auto FuncDbl, typename Class, typename... UserData>
		requires Genome::Internal::IsFunctionFirstArgGenomes<FuncBin, FuncOneByte, FuncInt32, FuncUInt32, FuncFloat, FuncDbl>
			void call(Class* ptr, UserData&&... data) const
		{
			if (type == Genome::Type::NONE)
				throw std::runtime_error("GenomeType == NONE");

			else if (type == Genome::Type::BINARY)
				(ptr->*FuncBin)(&GGenome<Genome::BinType>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::ONE_BYTE)
				(ptr->*FuncOneByte)(&GGenome<Genome::OneByteType>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::INT32)
				(ptr->*FuncInt32)(&GGenome<Genome::Int32Type>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::UINT32)
				(ptr->*FuncUInt32)(&GGenome<Genome::UInt32Type>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::FLOAT)
				(ptr->*FuncFloat)(&GGenome<Genome::FloatType>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::DOUBLE)
				(ptr->*FuncDbl)(&GGenome<Genome::DblType>, std::forward<UserData>(data)...);
		}

		template<auto FuncBin, auto FuncOneByte, auto FuncInt32, auto FuncUInt32, auto FuncFloat, auto FuncDbl, typename Class, typename... UserData>
		requires (!Genome::Internal::IsFunctionFirstArgGenomes<FuncBin, FuncOneByte, FuncInt32, FuncUInt32, FuncFloat, FuncDbl>)
			void call(Class* ptr, UserData&&... data) const
		{
			if (type == Genome::Type::NONE)
				throw std::runtime_error("GenomeType == NONE");

			else if (type == Genome::Type::BINARY)
				(ptr->*FuncBin)(std::forward<UserData>(data)...);
			else if (type == Genome::Type::ONE_BYTE)
				(ptr->*FuncOneByte)(std::forward<UserData>(data)...);
			else if (type == Genome::Type::INT32)
				(ptr->*FuncInt32)(std::forward<UserData>(data)...);
			else if (type == Genome::Type::UINT32)
				(ptr->*FuncUInt32)(std::forward<UserData>(data)...);
			else if (type == Genome::Type::FLOAT)
				(ptr->*FuncFloat)(std::forward<UserData>(data)...);
			else if (type == Genome::Type::DOUBLE)
				(ptr->*FuncDbl)(std::forward<UserData>(data)...);
		}


		//非メンバ関数
		//第1引数にgenomes
		template<auto FuncBin, auto FuncOneByte, auto FuncInt32, auto FuncUInt32, auto FuncFloat, auto FuncDbl, typename... UserData>
		requires Genome::Internal::IsFunctionFirstArgGenomes<FuncBin, FuncOneByte, FuncInt32, FuncUInt32, FuncFloat, FuncDbl>
			void call(UserData&&...data) const
		{
			if (type == Genome::Type::NONE)
				throw std::runtime_error("GenomeType == NONE");

			else if (type == Genome::Type::BINARY)
				FuncBin(&GGenome<Genome::BinType>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::ONE_BYTE)
				FuncOneByte(&GGenome<Genome::OneByteType>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::INT32)
				FuncInt32(&GGenome<Genome::Int32Type>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::UINT32)
				FuncUInt32(&GGenome<Genome::UInt32Type>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::FLOAT)
				FuncFloat(&GGenome<Genome::FloatType>, std::forward<UserData>(data)...);
			else if (type == Genome::Type::DOUBLE)
				FuncDbl(&GGenome<Genome::DblType>, std::forward<UserData>(data)...);
		}

		//引数は何でもok
		template<auto FuncBin, auto FuncOneByte, auto FuncInt32, auto FuncUInt32, auto FuncFloat, auto FuncDbl, typename... UserData>
		requires (!Genome::Internal::IsFunctionFirstArgGenomes<FuncBin, FuncOneByte, FuncInt32, FuncUInt32, FuncFloat, FuncDbl>)
			void call(UserData&&...data) const
		{
			if (type == Genome::Type::NONE)
				throw std::runtime_error("GenomeType == NONE");

			else if (type == Genome::Type::BINARY)
				FuncBin(std::forward<UserData>(data)...);
			else if (type == Genome::Type::ONE_BYTE)
				FuncOneByte(std::forward<UserData>(data)...);
			else if (type == Genome::Type::INT32)
				FuncInt32(std::forward<UserData>(data)...);
			else if (type == Genome::Type::UINT32)
				FuncUInt32(std::forward<UserData>(data)...);
			else if (type == Genome::Type::FLOAT)
				FuncFloat(std::forward<UserData>(data)...);
			else if (type == Genome::Type::DOUBLE)
				FuncDbl(std::forward<UserData>(data)...);
		}


		template<typename T>
		_NODISCARD Genome::Genomes<T>* get() {
			return &GGenome<T>;
		}

	private:
		template<typename T>
		static inline Genome::Genomes<T> GGenome = {};

	private:
		struct {
			std::size_t get() {
				return tempIndex++;
			}
		private:
			std::size_t tempIndex = 0;
		}tempIndex;
		template<typename T>
		static inline Genome::TempGenomes<T> tempGenome = {};//一時的なゲノムを保持

	public:
		template<Genome::IsGenomeType T>
		std::size_t pushTemp(const CppGA::Genome<T>& genome) {
			const auto index = tempIndex.get();
			auto pair = std::make_pair(index, genome);
			tempGenome<T>.insert(std::move(pair));
			return index;
		}
		template<Genome::IsGenomeType T>
		std::size_t pushTemp(CppGA::Genome<T>&& genome) {
			const auto index = tempIndex.get();
			auto pair = std::make_pair(index, std::move(genome));
			tempGenome<T>.insert(std::move(pair));
			return index;
		}

		//tempから本体へpush_back 要素は消さない
		template<Genome::IsGenomeType T>
		void pushBackToData(const typename Genome::TempGenomes<T>::key_type key) {
			GGenome<T>.emplace_back(tempGenome<T>.at(key));
		}
		void pushBackToData(const typename Genome::TempGenomes<Genome::BinType>::key_type key) {
			if (type == Genome::Type::NONE)
				throw std::runtime_error("GenomeType == NONE");

			else if (type == Genome::Type::BINARY)
				GGenome<Genome::BinType>.emplace_back(tempGenome<Genome::BinType>.at(key));
			else if (type == Genome::Type::DOUBLE)
				GGenome<Genome::DblType>.emplace_back(tempGenome<Genome::DblType>.at(key));
		}



	private:
		friend GenomeRead;
	};



	class GenomeRead final {
	public:
		explicit GenomeRead(Internal::ManagerKey) {};
		~GenomeRead() = default;
		FluidumUtils_Class_NoCopyNoMove(GenomeRead)

	public:
		template<Genome::IsGenomeType T>
		const Genome::Genomes<T>* get() const {
			return &GenomeWrite::GGenome<T>;
		}

		Genome::Type currentType() const noexcept {
			return GenomeWrite::type;
		}

		_NODISCARD std::size_t populationSize() const {
			if (GenomeWrite::type == Genome::Type::BINARY)
				return GenomeWrite::GGenome<bool>.size();
			else if (GenomeWrite::type == Genome::Type::DOUBLE)
				return GenomeWrite::GGenome<double>.size();

			throw std::runtime_error("");
		}

		_NODISCARD std::size_t genomeSize(const Genome::PopulationSize populationIndex) const {
			if (GenomeWrite::type == Genome::Type::BINARY)
				return GenomeWrite::GGenome<bool>[populationIndex].size();
			else if (GenomeWrite::type == Genome::Type::DOUBLE)
				return GenomeWrite::GGenome<double>[populationIndex].size();

			throw std::runtime_error("");
		}



	};

}