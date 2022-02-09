#include "credit.h"

FS::Bar::Credit::Credit() {
	FluidumScene_Log_Constructor(::FS::Bar::Credit);
}

FS::Bar::Credit::~Credit() noexcept {
	FluidumScene_Log_Destructor(::FS::Bar::Credit);
}

void FS::Bar::Credit::call() {

	if (ImGui::BeginPopupModal("Credit", nullptr)) {

		//ImGui::PushTextWrapPos(ImGui::GetWindowSize().x);
		//ImGui::Text(this->creditText.c_str());
		//ImGui::Separator();
		//ImGui::PopTextWrapPos();

		ImGui::EndPopup();
	}
}


