#pragma	once

#include "../Common/common.h"

namespace FD::Project::Limits {

	struct UserFiles final {
		FluidumUtils_Class_Delete_ConDestructor(UserFiles)
	public:
		//num of directories(virtual folder)
		static constexpr uint16_t DirectorySizeMax = 500;
	};

	struct ProjectFiles final {
		FluidumUtils_Class_Delete_ConDestructor(ProjectFiles)
	public:
		//num of directories
		static constexpr uint16_t DirectorySizeMax = 500;
	};


}

namespace FD::Plot::Limits {

	//ImPlot
	struct Plot final {
		FluidumUtils_Class_Delete_ConDestructor(Plot)
	public:
		static constexpr uint8_t PlotMax = 20;
		static constexpr uint8_t FigureMax = 20;
	};



}

namespace FD::Layout::Limits {
	
	struct Window final {
		FluidumUtils_Class_Delete_ConDestructor(Window)
	public:
		static constexpr uint8_t SeparatorMax = 8;
	};

	constexpr inline uint8_t UnRedo = 100;

}

namespace FD::Calc::Array::Limits {

	constexpr inline uint32_t MaxSize = 100'000;
	
}

namespace FD::Console::Limits {

	constexpr inline uint16_t Characters = 10'000;
	constexpr inline uint16_t Lines = 10'000;

}