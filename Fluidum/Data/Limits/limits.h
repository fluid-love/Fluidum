#pragma	once

#include "../Common/common.h"

namespace FD::Log::Limits {
	//output to a file.
	constexpr inline UIF8 LogArraySize = 100;
}

namespace FD::Project::Limits {

	namespace UserFiles {
		//num of directories(virtual folder)
		constexpr inline UIF16 DirectorySizeMax = 500;
		constexpr inline UIF16 FileSizeMax = 500;
	}

}

namespace FD::Coding::Limits {
	//1 charactor 1byte
	constexpr inline UIF32 FileSizeMax = 500000;

}

namespace FD::Coding::Tab::Limits {

	constexpr inline UIF8 FileSizeMax = 100;
	constexpr inline UIF8 DisplaySizeMax = 10;

}

namespace FD::Plot::Limits {

	//ImPlot
	struct Plot final {
		FluidumUtils_Class_Delete_ConDestructor(Plot);
	public:
		static constexpr UIF8 PlotMax = 20;
		static constexpr UIF8 FigureMax = 20;
	};

}

namespace FD::Layout::Limits {

	namespace Window {
		constexpr inline UIF8 SeparatorMax = 8;
	}

	constexpr inline UIF8 UnRedo = 100;

}

namespace FD::Calc::Array::Limits {

	constexpr inline UIF32 MaxSize = 100'000;

}

namespace FD::Console::Limits {

	constexpr inline UIF16 Characters = 10'000;
	constexpr inline UIF16 Lines = 10'000;

}

namespace FD::History::Limits {

	constexpr inline UIF8 ProjectLogMax = 50;
	constexpr inline UIF8 FileLogMax = 50;

}