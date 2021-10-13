#include "layout.h"

FS::LeftLayout::LeftLayout(const FD::LayoutRead* const layoutRead, FD::LayoutWrite* const layoutWrite, const FD::GuiRead* const guiRead, FD::GuiWrite* const guiWrite)
	: layoutRead(layoutRead), layoutWrite(layoutWrite), guiRead(guiRead), guiWrite(guiWrite)
{
	GLog.add<FD::Log::Type::None>("Construct LeftLayuotScene.");

	style.windowPos = { guiRead->leftBarWidth() ,guiRead->menuBarHeight() + guiRead->topBarHeight() };
	style.windowSize = { guiRead->windowSize().x / 4.0f,guiRead->windowSize().y - style.windowPos.y };

	layoutWrite->leftLayoutPos(style.windowPos);

}

FS::LeftLayout::~LeftLayout() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct LeftLayuotScene.");
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

void FS::LeftLayout::call() {

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.006f, 0.005f, 0.005f, 1.000f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.266f, 0.200f, 0.200f, 1.000f));

	this->dockGui();

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);

	//if (style.isChanged)
	//	this->updateScissorViewport();
}

void FS::LeftLayout::dockGui() {
	ImGui::SetNextWindowPos(style.windowPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(style.windowSize, ImGuiCond_Always);

	constexpr auto windowFlag =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar;

	if (!flag.limitLeft)
		ImGui::Begin("LeftLayout", nullptr, windowFlag);
	else
		ImGui::Begin("LeftLayout", nullptr, windowFlag | ImGuiWindowFlags_NoResize);
	

	id = ImGui::GetID("LeftLayout");
	ImGui::DockSpace(id, ImVec2());

	auto currentSizeX = ImGui::GetWindowSize().x;
	if (static_cast<int32_t>(currentSizeX) != static_cast<int32_t>(style.windowSize.x)) {
		style.windowSize.x = currentSizeX;
		flag.isChanged = true;
	}

	if (style.windowSize.x > guiRead->windowSize().x * 0.8f) {
		style.windowSize.x = guiRead->windowSize().x * 0.8f;
		flag.limitLeft = true;
	}
	else if (style.windowSize.x < guiRead->windowSize().x * 0.15f) {
		style.windowSize.x = guiRead->windowSize().x * 0.15f;
		flag.limitLeft = true;
	}
	else {
		flag.limitLeft = false;
	}

	layoutWrite->leftLayoutSize(style.windowSize);


	ImGui::End();
}

void FS::LeftLayout::updateScissorViewport() {
	//scissors
	//for (auto& x : sharedObject->scissors) {
	//	x->setScissorExtentWidth(gui->windowSize().x - static_cast<uint32_t>(style.windowSize.x));
	//}

	//style.isChanged = false;
}
