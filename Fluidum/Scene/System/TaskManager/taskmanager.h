#pragma once

#include "../../Common/common.h"

#include "../../../Data/Gui/Texts/taskmanager.h"

#ifdef FluidumUtils_Type_OS_Windows
#include <pdh.h>
#endif 

namespace FS::System {

	class TaskManager final : public Scene {
	public:
		explicit TaskManager();
		void Constructor();

		~TaskManager() noexcept;

		FluidumUtils_Class_Delete_CopyMove(TaskManager);

	public:
		virtual void call() override;

	private:

		FD::Text::TaskManager text{};
		struct {
			FD::Text::Common unexpected{ FD::Text::CommonText::UnexpectedError };
		}text_;

	private:
		ImPlotContext* imPlotContext{};

		struct {
			float leftWidth{};
			const float iconFontscale = 1.3f;
			ImVec2 minSizeLimit{};
			float intervalComboWidth{};
		}style;

		struct {
			bool window = true;
			bool plotHovered = false;
		}flag;

		struct {
			std::chrono::system_clock::time_point now{};

			[[nodiscard]] bool over(const std::chrono::system_clock::time_point& point, const Size milliseconds) const {
				const auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - point);
				return (milli > std::chrono::milliseconds(milliseconds));
			}
		}timer;

		struct {
#ifdef FluidumUtils_Type_OS_Windows
			PDH_HQUERY query{};
			PDH_HCOUNTER total{};
#endif
			const char* processorArchiitecture{};
			ISize numOfPhysicalProcesser{};
			ISize numOfLogicalProcesser{};

			Size intervalMilliSeconds = 1000;//1 second

			std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
			std::vector<float> val{};
			std::vector<float> x{};
		}cpuInfo;

		struct {
			std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
			std::vector<float> val{};
			std::vector<float> x{};

			Size max{};
		}memInfo;


		enum class Type : UIF8 {
			Overview,
			Cpu,
			Gpu,
			Memory,
			Storage
		};

		struct {
			Type type = Type::Overview;
		}select;

		std::function<void()> draw = std::bind(&TaskManager::overview, this);

		struct {
			const std::vector<std::pair<UI16, const char*>> interval{
				std::pair{ 500  , "0.5" },
				std::pair{ 1000 , "1"   },
				std::pair{ 2000 , "2"   },
				std::pair{ 5000 , "5"   }
			};

			const char* current = interval.at(1).second;
		}interval;


	private:
		void constructCpuInfo();

	private:
		void update();
		void catchWindowFlags();

	private:
		void left();
		void icons();

	private:
		void right();

	private:
		void overview();

	private:
		void cpu();
		void cpu_title();
		void cpu_interval();
		void cpu_plot();
		void cpu_info();

	private:
		void gpu();

	private:
		void memory();
		void memory_plot();

	private:
		void storage();

	};

}