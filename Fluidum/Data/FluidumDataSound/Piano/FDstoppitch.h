#pragma once

#include "../common/FDtype.h"
#include "FDpiano.h"

namespace FD::Piano::Internal {

	namespace Internal {
		template<auto Function, typename Arg>
		consteval bool isAppropriateStopPitchLoopFunction() {
			//メンバ関数であること
			bool result = Fluidum::Utils::Concept::IsMemberFunctionPointer<Function>;
			//第一引数はArgであること
			using T = std::tuple_element_t<0, Fluidum::Utils::Tuple::FunctionArgsToTupleType<Function>>;
			result = std::same_as<Arg, T>;
			return result;
		}
	}


	using StopPitchStackType = std::vector<std::pair<Pitch, std::chrono::system_clock::time_point>>;


	//class Playないに置く
	//同期は気にしなくてよい
	template<auto Function>
	struct StopPitch final {
	public:
		template<typename Class>
		StopPitch(Class* const ptr) {
			assert(ptr);
			th = std::thread(&StopPitch::loop<Class>, this, ptr);
		}
		~StopPitch() {
			finishFlag = true;
			th.join();
		}
	public:
		using StackType = StopPitchStackType;
		static_assert(Internal::isAppropriateStopPitchLoopFunction<Function, const StackType&>(), "Functionが適切ではありません．");

	private:
		template<typename Class>
		void loop(Class* const ptr) {
			while (!finishFlag) {
				std::unique_lock<std::mutex> lock(mtx);
				(ptr->*Function)(stack);
				lock.unlock();

				this->call();
			}
		}

		void call() {
			for (auto& x : callFunctions) {
				x();
			}
			callFunctions.clear();
		}

	private:
		std::thread th;
		std::atomic_bool finishFlag = false;

		std::mutex mtx{};

		std::vector<std::function<void()>> callFunctions;

		StackType stack{};

	public:

		//いつ消すかを指定
		void add(const Pitch pitch, const std::chrono::system_clock::time_point& time) {
			std::lock_guard<std::mutex> lock(mtx);
			stack.emplace_back(std::make_pair(pitch, time));
		}

		//ピアノは同じピッチが重なることがない
		void erase(const Pitch pitch) {
			std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
			//lockできない場合
			if (!lock.try_lock()) {
				callFunctions.emplace_back(std::bind(&StopPitch::erase, this, pitch));
				return;
			}

			for (auto itr = stack.begin(), end = stack.end(); itr != end; itr++) {
				if ((*itr).first == pitch) {
					stack.erase(itr);
					break;
				}
			}
		}



	};



}