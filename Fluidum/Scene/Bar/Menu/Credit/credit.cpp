#include "credit.h"

FS::Internal::Bar::Credit::Credit() {
	Internal::GLog.add<FD::Log::Type::None>("Construct CreditScene.");
}

FS::Internal::Bar::Credit::~Credit() noexcept {
	try {
		Internal::GLog.add<FD::Log::Type::None>("Destruct CreditScene.");
	}
	catch (const std::exception& e) {
		try {
			std::cerr << e.what() << std::endl;
			abort();
		}
		catch (...) {
			abort();
		}
	}
	catch (...) {
		abort();
	}
}

void FS::Internal::Bar::Credit::call() {

	if (ImGui::BeginPopupModal("Credit", nullptr)) {

		//ImGui::PushTextWrapPos(ImGui::GetWindowSize().x);
		//ImGui::Text(this->creditText.c_str());
		//ImGui::Separator();
		//ImGui::PopTextWrapPos();

		ImGui::EndPopup();
	}
}


