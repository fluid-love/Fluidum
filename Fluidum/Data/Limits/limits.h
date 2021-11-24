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

}

namespace FD::Calc::Array::Limits {

	constexpr inline uint32_t MaxSize = 100000;
	
}