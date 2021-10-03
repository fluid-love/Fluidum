#pragma once

#include "../../common/FSusing.h"

namespace FS {

	class Console final :public Scene {
	public:
		explicit Console(
			Manager* manager,
			FD::LogWrite* log,
			FD::ConsoleLogRead* console
		);
		void Constructor(FD::LogWrite, FD::ConsoleLogRead);

		~Console();

	public:
		virtual void update() override;
		virtual void draw() override;

	private://data

		FD::LogWrite* const log = nullptr;
		const FD::ConsoleLogRead* const console = nullptr;


	};
}
