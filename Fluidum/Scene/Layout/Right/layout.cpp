#include "layout.h"

FS::RightLayout::RightLayout(const FD::LayoutRead* const layoutRead, FD::LayoutWrite* const layoutWrite, const FD::GuiRead* const guiRead, FD::GuiWrite* const guiWrite)
	: layoutRead(layoutRead), layoutWrite(layoutWrite), guiRead(guiRead), guiWrite(guiWrite)
{
	GLog.add<FD::Log::Type::None>("Construct RightLayuotScene.");

	style.windowPos = { layoutRead->leftLayoutPos().x + layoutRead->leftLayoutSize().x,guiRead->menuBarHeight() + guiRead->topBarHeight() };
	style.windowSize = { guiRead->windowSize().x - style.windowPos.x, guiRead->windowSize().y - style.windowPos.y };

	//ちょっとオーバーにして隠す
	style.windowSize.x += 5.0f;
}

FS::RightLayout::~RightLayout() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct RightLayuotScene.");
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

void FS::RightLayout::call() {

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.005f, 0.005f, 0.006f, 1.000f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.266f, 0.200f, 0.200f, 1.000f));

	this->update();
	this->dockGui();

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);

	//if (style.isChanged)
	//	this->updateScissorViewport();
}

void FS::RightLayout::dockGui() {

	ImGui::SetNextWindowPos(style.windowPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(style.windowSize, ImGuiCond_Always);

	constexpr auto windowFlag =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoMove
		;

	if (!flag.limitLeft)
		ImGui::Begin("RightLayout", nullptr, windowFlag);
	else
		ImGui::Begin("RightLayout", nullptr, windowFlag | ImGuiWindowFlags_NoResize);


	id = ImGui::GetID("RightLayout");
	ImGui::DockSpace(id, ImVec2());

	//auto currentSizeX = ImGui::GetWindowSize().x;
	//if (static_cast<int32_t>(currentSizeX) != static_cast<int32_t>(style.windowSize.x)) {
	//	style.windowSize.x = currentSizeX;
	//	flag.isChanged = true;
	//}

	//if (style.windowSize.x > guiRead->windowSize().x * 0.5f) {
	//	style.windowSize.x = guiRead->windowSize().x * 0.5f;
	//	flag.limitLeft = true;
	//}
	//else if (style.windowSize.x < guiRead->windowSize().x * 0.15f) {
	//	style.windowSize.x = guiRead->windowSize().x * 0.15f;
	//	flag.limitLeft = true;
	//}
	//else {
	//	flag.limitLeft = false;
	//}


	ImGui::End();
}

void FS::RightLayout::update() {
	style.windowPos.x = layoutRead->leftLayoutPos().x + layoutRead->leftLayoutSize().x;
	style.windowSize.x = guiRead->windowSize().x - style.windowPos.x + 5.0f;

}
