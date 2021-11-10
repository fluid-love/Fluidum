#include "topbar.h"
#include "../../Calc/Lua/lua.h"
#include "../../Utils/Popup/message.h"
#include <imgui_internal.h>

using namespace FU::ImGui::Operators;

FS::TopBar::TopBar(
	const FD::ProjectRead* const projectRead,
	const FD::FluidumFilesRead* const fluidumFilesRead,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	const FD::SceneRead* const sceneRead,
	const FD::TopBarRead* const topBarRead,
	FD::TopBarWrite* const topBarWrite
)
	: projectRead(projectRead), fluidumFilesRead(fluidumFilesRead), guiRead(guiRead), guiWrite(guiWrite), sceneRead(sceneRead), topBarRead(topBarRead), topBarWrite(topBarWrite)
{
	GLog.add<FD::Log::Type::None>("Construct TopBarScene.");

	style.windowPos = ImVec2(0.0f, guiRead->menuBarHeight());
	const float windowHeight = guiRead->menuBarHeight();
	style.windowSize = ImVec2(guiRead->windowSize().x, windowHeight);

	if (windowHeight <= ImGui::GetStyle().WindowMinSize.y)
		guiWrite->topBarHeight(ImGui::GetStyle().WindowMinSize.y);
	else
		guiWrite->topBarHeight(windowHeight);


	style.packageWindowPos = { guiRead->windowSize().x * 0.3f,0.0f };
	style.packageWindowSize = { guiRead->windowSize().x * 0.6f,guiRead->menuBarHeight() };

}

FS::TopBar::~TopBar() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct TopBarScene.");
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

namespace FS::Internal::Bar {
	constexpr auto commonWindowFlag =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;
}

void FS::TopBar::call() {

	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	//角をとる
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//borderを細く
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("TopBar", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImGui::End();


	this->rightGui();
	this->calc();
	this->scene();


	ImGui::PopStyleVar(2);

	////////////////////////////////////////////////////////////////////////////////////////////


	ImGui::SetNextWindowPos(style.packageWindowPos);
	ImGui::SetNextWindowSize(style.packageWindowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	ImGui::Begin("TopBar2", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBackground);

	ImGui::Dummy(ImGui::GetStyle().FramePadding * 1.17f);

	this->packageGui(); ImGui::SameLine(); ImGui::Dummy(ImVec2(guiRead->windowSize().x * 0.02f, 0.0f)); ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.05f, 0.05f, 0.05f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.03f, 0.03f, 0.03f, 1.0f));
	ImGui::PushItemWidth(guiRead->windowSize().x / 13.0f);

	this->layoutGui(); ImGui::SameLine(); ImGui::Dummy(ImVec2(guiRead->windowSize().x * 0.02f, 0.0f)); ImGui::SameLine();
	this->templateGui(); ImGui::SameLine(); ImGui::Dummy(ImVec2(guiRead->windowSize().x * 0.1f, 0.0f)); ImGui::SameLine();
	ImGui::PopItemWidth();

	this->projectNameGui();

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	ImGui::End();
}

void FS::TopBar::packageGui() {
	ImGui::AlignTextToFramePadding();
	ImGui::Text(text.package); ImGui::SameLine();

	ImGui::Button(ICON_MD_ARCHIVE);
}

void FS::TopBar::layoutGui() {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Layout"); ImGui::SameLine();

	int32_t layoutIndex = 0;
	const char* layoutTemplateNames[1] = { ICON_MD_DASHBOARD " Standard" };
	//	const char* layoutTemplateNames[4] = { ICON_MD_DASHBOARD " Standard",ICON_MD_SCHEDULE " Animation",ICON_MD_SHARE " Node" ,ICON_MD_VISIBILITY " View" };

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.01f, 0.01f, 0.01f, 1.0f));
	ImGui::Combo("##Layout", &layoutIndex, layoutTemplateNames, IM_ARRAYSIZE(layoutTemplateNames));
	ImGui::PopStyleColor();
}

void FS::TopBar::templateGui() {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Template"); ImGui::SameLine();

	int32_t layoutIndex = 0;
	const char* layoutTemplateNames[2] = {
		ICON_MD_DASHBOARD " Clear",
		ICON_MD_CODE " Coding"
	};

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.01f, 0.01f, 0.01f, 1.0f));
	if (ImGui::BeginCombo("##Template", "##TemplateP", ImGuiComboFlags_NoPreview)) {
		for (uint16_t i = 0; i < std::extent_v<decltype(layoutTemplateNames), 0>; i++) {
			if (ImGui::Selectable(layoutTemplateNames[i]))
				this->setTemplate(i);
		}
		ImGui::EndCombo();
	}
	ImGui::PopStyleColor();
}

void FS::TopBar::setTemplate(const uint16_t index) {
	if (index == 0) {

	}
}

void FS::TopBar::projectNameGui() {
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

	if (projectRead->isDefaultProject())
		ImGui::Button(text.tempProject);
	else
		ImGui::Button(projectRead->getProjectName().c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void FS::TopBar::rightGui() {
	ImGui::SetNextWindowPos(ImVec2(style.windowSize.x * 0.6f, style.windowPos.y));
	ImGui::SetNextWindowSize(ImVec2(style.windowSize.x * 0.4f, style.windowSize.y));

	ImGui::Begin("Right", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBackground);

	this->backColorGui();
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	this->areaGui();

	ImGui::End();
}

void FS::TopBar::backColorGui() {
	ImGui::SetNextItemWidth(style.windowSize.x * 0.1f);
	static float col[3] = {};
	if (ImGui::ColorEdit3("##Back", col)) {
		//auto ind = commands->getIndices(FVK::CommandType::BEGIN_RENDERPASS);
		//for (auto& x : ind) {
		//	FVK::BeginRenderPassCommand command = commands->at<FVK::CommandType::BEGIN_RENDERPASS>(x);

		//	command->setBackColor(std::array<float, 4>{col[0], col[1], col[2], 1.0f});
		//}

	}

}

void FS::TopBar::areaGui() {
	//static bool temp = false;
	//ImGui::Button(ICON_MD_BRANDING_WATERMARK);
}

void FS::TopBar::calc() {
	ImGui::SetNextWindowPos(ImVec2(style.windowSize.x * 0.4f, style.windowPos.y));
	ImGui::SetNextWindowSize(ImVec2(style.windowSize.x * 0.3f, style.windowSize.y));

	ImGui::Begin("Run", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBackground);
	this->separator(ImGui::GetWindowPos().x + 2.0f, { 0.3f,0.2f,0.2f,1.0f });
	ImGui::Spacing(); ImGui::SameLine();

	this->mode();
	ImGui::SameLine();

	bool isRunning = sceneRead->isExist<Lua::Calc>();

	//前のステップまで戻る
	//一時停止中以外は選択不可
	if (isRunning) {
		ImGui::Button(ICON_MD_SKIP_PREVIOUS);
	}
	else {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.1f);
		ImGui::Button(ICON_MD_SKIP_PREVIOUS);
		ImGui::PopStyleVar();
	}

	ImGui::SameLine();

	//実行中
	if (isRunning) {

		//一時停止を要求
		bool pause = ImGui::Button(ICON_MD_STOP);

		ImGui::SameLine();
	}
	else {

		bool run = ImGui::Button(ICON_MD_PLAY_ARROW);
		if (run) {
			pos.run = ImGui::GetItemRectMax();
			this->run();
		}
	}

	ImGui::SameLine();

	//次のステップまで進む
	if (isRunning) {
		ImGui::Button(ICON_MD_SKIP_NEXT);

	}
	else {
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.1f);

		//停止を要求
		bool stop = ImGui::Button(ICON_MD_PAUSE);
		ImGui::SameLine();

		//次のステップ
		ImGui::Button(ICON_MD_SKIP_NEXT);

		ImGui::PopStyleVar();
	}


	ImGui::End();

	this->playCheck();
}

void FS::TopBar::mode() {
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth()*0.275f);
	if (!ImGui::BeginCombo("##Mode", "Debug"))
		return;

	if (ImGui::Selectable("Standard"))
		std::cout << "a";


	ImGui::EndCombo();
}

void FS::TopBar::run() {
	const FD::Project::CodeType type = fluidumFilesRead->getCurrentMainCodeType();
	using enum FD::Project::CodeType;

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (type == Empty) {
		GLog.add<FD::Log::Type::None>("Request add Utils::MessageScene.");
		Scene::addScene<Utils::Message>(text.error_mainfile,pos.run);
	}
	else if (type == Error) {
		GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__);
		abort();
	}
	else if (type == Lua) {
		GLog.add<FD::Log::Type::None>("Request add LuaCalcScene(Async).");
		Scene::addAsyncScene<Lua::Calc>();
	}
	else if (type == Python) {

	}
	else if (type == AngelScript) {

	}
}

void FS::TopBar::playCheck() {
	//if (!wait.isPlayButtonClicked || !loadingParam.finishFlag.load())
	//	return;



	////計算を開始する命令が飛んできた

	////保存が速く終わっていても待機
	//const auto currentTime = std::chrono::system_clock::now();
	//if (currentTime > wait.time) {
	//	std::cout << "a\n\n\n\n";
	//	wait.reset();
	//	//Scene::addAsyncScene<FS::Lua>();
	//}



}

void FS::TopBar::scene() {
	ImGui::SetNextWindowPos(ImVec2(7.0f, style.windowPos.y - 2.0f));
	ImGui::SetNextWindowSize(ImVec2(style.windowSize.x * 0.4f, style.windowSize.y));

	ImGui::Begin("SceneFunc", nullptr, Internal::Bar::commonWindowFlag | ImGuiWindowFlags_NoBackground);

	//combo button color
	if (!topBarRead->getIndices()->empty())
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.01f, 0.01f, 1.0f));
	else
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.02f, 0.02f, 0.02f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.01f, 0.01f, 0.01f, 1.0f));

	this->combo();	ImGui::SameLine();
	ImGui::PopStyleColor(2);

	this->func();

	ImGui::End();
}

void FS::TopBar::combo() {
	const auto* info = topBarRead->getInfo();

	if (!ImGui::BeginCombo("##SceneFuncCombo", "##None", ImGuiComboFlags_NoPreview))
		return;

	for (auto& x : *info) {

		if (ImGui::Selectable(x.sceneName.c_str(), x.select)) {
			if (!x.select)
				topBarWrite->lock(x.code);
			else
				topBarWrite->unlock(x.code);
		}
	}


	ImGui::EndCombo();

}

void FS::TopBar::func() {

	const auto* indices = topBarRead->getIndices();

	for (auto x : *indices) {
		ImGui::Spacing(); ImGui::SameLine();
		topBarRead->call(x);
		ImGui::SameLine(); ImGui::Spacing();
		this->separator(ImGui::GetItemRectMax().x + 10.0f);
		ImGui::SameLine();
	}

}

void FS::TopBar::separator(const float posX, const ImVec4& col4) {

	ImDrawList* list = ImGui::GetWindowDrawList();
	const ImU32 col = ImGui::ColorConvertFloat4ToU32(col4);
	list->AddLine({ posX,style.windowPos.y + 2.0f }, { posX,style.windowPos.y + style.windowSize.y - 2.0f }, col);

}

