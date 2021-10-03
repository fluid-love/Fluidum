#pragma once

#include "../../common/FSusing.h"
#include "../Text/FSguitext.h"

namespace FS {

	class Analysis final :public Scene {
	public:
		explicit Analysis(Manager* manager,FD::PlotRead* plot);
		void Constructor(FD::PlotRead);

		~Analysis();

	public:
		virtual void update() override;
		virtual void draw() override;

	private://data
		FD::PlotRead* plot = nullptr;

		Text::Analysis text = {};
	

	private:
		void plotGui();
		void functionGui();


	};
}