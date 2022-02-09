#include "property.h"

#include "../../Utils/Popup/backwindow.h"

FS::Project::Property::Property(
	const FD::GuiRead* const guiRead,
	const FD::Style::ColorRead* const colorRead,
	const FD::Style::VarRead* const varRead,
	FD::Project::PropertyWrite* const propertyWrite,
	const FD::Project::PropertyRead* const propertyRead,
	FD::Project::PropertyLuaWrite* const propertyLuaWrite,
	const FD::Project::PropertyLuaRead* const propertyLuaRead,
	const FD::ProjectFilesRead* const projectFilesRead,
	const FD::UserFilesRead* const userFilesRead
) :
	varRead(varRead),
	colorRead(colorRead),
	propertyWrite(propertyWrite),
	propertyRead(propertyRead),
	propertyLuaWrite(propertyLuaWrite),
	propertyLuaRead(propertyLuaRead),
	projectFilesRead(projectFilesRead),
	userFilesRead(userFilesRead)
{
	FluidumScene_Log_Constructor(::FS::Project::Property);

	style.windowPos = varRead->popupWindowPos(guiRead->centerPos(), guiRead->windowSize());
	style.windowSize = varRead->popupWindowSize(style.windowPos, guiRead->windowSize());
	style.tabHeight = ImGui::GetFontSize() * 2.0f;
	style.infomationAreaHeight = ImGui::GetFontSize() * 13.0f;

	style.bottomHeight = ImGui::GetFontSize() * 5.3f;
	style.leftWindowSize = { style.windowSize.y * 0.3f, style.windowSize.y - style.tabHeight - style.bottomHeight };
	style.rightWindowSize = { 0.0f, style.leftWindowSize.y - style.infomationAreaHeight };
	style.informationWindowSize = { 0.0f, style.infomationAreaHeight };

	style.windowWidth = style.windowSize.x;
	style.innerHeight = ImGui::GetFontSize() * 1.6f;
	style.dummy = (style.innerHeight - ImGui::CalcTextSize("a").y - ImGui::GetStyle().FramePadding.y) / 2.0f;

	this->init();

	FluidumScene_Log_RequestAddScene(::FS::Utils::PopupBackWindow);
	Scene::addScene<Utils::PopupBackWindow>();
}

FS::Project::Property::~Property() noexcept {
	FluidumScene_Log_RequestDeleteScene(::FS::Utils::PopupBackWindow);
	Scene::deleteScene<Utils::PopupBackWindow>();

	FluidumScene_Log_Destructor(::FS::Project::Property);
}

void FS::Project::Property::call() {

	ImGui::SetNextWindowFocus();//popup backwindow
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	//animation
	ImAnime::PushStyleVar(anime.window, 0.3f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	//style
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, varRead->popupWindowBorderSize());
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.043f,0.043f,0.043f,1.0f });

	constexpr ImGuiWindowFlags windowFlags =
		FD::Style::VarRead::PopupWindowFlags() ^
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin(text.projectProperty, &flag.window, windowFlags);

	ImGui::Spacing();

	this->tab();

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.012f,0.012f,0.012f,1.0f });

	this->left();

	ImGui::SameLine();
	ImGui::BeginChild("##RightWindow", { 0.0f, style.leftWindowSize.y }, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	this->right();

	ImGui::PopStyleColor();

	this->information();

	ImGui::EndChild();

	this->bottom();

	ImGui::End();

	ImAnime::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(3);

	this->closeWindow();
	this->catchProjectType();
}

void FS::Project::Property::closeWindow() {
	if (flag.window)
		return;

	//not saved
	if (!this->confirmNotSaved())
		return;

	FluidumScene_Log_RequestDeleteScene(::FS::Project::Property);
	Scene::deleteScene<Property>();
}

void FS::Project::Property::catchProjectType() {
	if (!type.changeFlag)
		return;

	if (!this->confirmNotSaved()) {
		//cancel or internal error
		type.changeFlag = false;
		return;
	}

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Change current project type. {} -> {}.", magic_enum::enum_name(type.current), magic_enum::enum_name(type.change));

	type.current = type.change;

	type.changeFlag = false;

	propertyWrite->currentProjectType(type.current);
	propertyWrite->save();
}

void FS::Project::Property::tab() {
	ImGui::BeginChild("##PropTab", { 0.0f, style.tabHeight }, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (!ImGui::BeginTabBar("##ProjectProTab")) {
		ImGui::EndChild();
		return;
	}

	ImGui::PushStyleColor(ImGuiCol_TabActive, { 0.15f,0.5f,0.75f,0.25f });

	if (ImGui::BeginTabItem(text.tab_main)) {
		select.tab = Tab::Main;
		ImGui::EndTabItem();
	}

	if (type.isNone()) {
		const char* label = "Error";
		if (type.isLua())
			label = "Lua";
		else if (type.isPython())
			label = "Python";
		else if (type.isCpp())
			label = "C++";
		if (ImGui::BeginTabItem(label)) {
			select.tab = Tab::Language;
			ImGui::EndTabItem();
		}

	}

	if (ImGui::BeginTabItem("Fluidum")) {
		select.tab = Tab::Fluidum;
		ImGui::EndTabItem();
	}

	ImGui::PopStyleColor();

	ImGui::EndTabBar();

	ImGui::EndChild();
}

void FS::Project::Property::left() {
	ImGui::BeginChild("##PropLeft", style.leftWindowSize);

	if (type.isNone())
		this->left_none();
	else if (type.isLua())
		this->left_lua();

	ImGui::EndChild();

}

void FS::Project::Property::leftLoop(const std::vector<Node>& nodes) {
	using namespace FU::ImGui::Operators;

	constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanFullWidth;

	//hovered
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4());

	for (auto& x : nodes) {
		const bool selected = select.list == x.list;

		if (ImGui::TreeNodeEx(x.label, nodeFlags)) {
			ImGui::TreePop();
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			select.list = x.list;
			info.title = x.label;
			info.text = x.infoText;
			select.listFunction = std::bind(x.function, this);
		}

		const ImVec2 min = ImGui::GetItemRectMin();
		const ImVec2 max = ImGui::GetItemRectMax();

		if (selected) {
			constexpr ImU32 col = FU::ImGui::ConvertImVec4ToImU32(0.1f, 0.8f, 0.96f, 0.7f);
			ImGui::GetForegroundDrawList()->AddRect(min, max, col, 0.0f, 0, 1.4f);
		}
		if (ImGui::IsMouseHoveringRect(min, max)) {
			constexpr ImU32 col = FU::ImGui::ConvertImVec4ToImU32(0.1f, 0.5f, 0.96f, 0.2f);
			ImGui::GetWindowDrawList()->AddRectFilled(min, max, col, 0.0f, 0);
		}
	}

	ImGui::PopStyleColor();
}

void FS::Project::Property::left_none() {
	constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanFullWidth;

	if (select.tab == Tab::Main) {
		this->leftLoop(node.none_main);
	}

}

void FS::Project::Property::left_lua() {

	if (select.tab == Tab::Main) {
		this->leftLoop(node.lua_main);
	}
	else if (select.tab == Tab::Language) {
		this->leftLoop(node.lua_language);
	}

}

void FS::Project::Property::right() {
	using namespace FU::ImGui::Operators;

	ImGui::BeginChild("##PropRight", style.rightWindowSize);

	if (select.listFunction)
		select.listFunction();

	ImGui::EndChild();
}

void FS::Project::Property::right_currentProjectType() {
	ImGui::Spacing();

	ImGui::BulletText(text.currentType);
	varRead->bulletDummy();

	const char* label = "Error";
	if (type.isLua())
		ImGui::TextColored(colorRead->noerror(), "Lua");
	else if (type.isNone())
		ImGui::TextColored(colorRead->noerror(), "None");

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::BulletText(text.change);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
	ImGui::Bullet();
	ImGui::PopStyleVar();
	ImGui::SameLine();


	if (ImGui::Checkbox("Lua", &type.lua)) {
		if (!type.lua) { //Lua -> Lua
			type.lua ^= 1;
		}
		else {
			type.python = false, type.cpp = false;
			const bool ok = this->changeProjectType(FD::Project::Property::ProjectType::Lua);
			if (!ok) {
				type.lua ^= 1;
			}
		}
	}

	//future
	//ImGui::SameLine(); ImGui::Spacing();
	//if (ImGui::Checkbox("Python", &type.python)) {
	//	type.python = false;
	//	//type.lua = false, type.cpp = false;
	//	//this->changeProjectType(FD::Project::Property::ProjectType::Lua);
	//}
	//
	//ImGui::SameLine(); ImGui::Spacing();
	//if (ImGui::Checkbox("Cpp", &type.cpp)) {
	//	type.cpp = false;
	//	//type.lua = false, type.python = false;
	//}

}

void FS::Project::Property::right_entryFilePath() {
	assert(!type.isCpp());

	std::string path{};
	bool exists;
	if (type.isLua()) {
		path = propertyLuaRead->entryFilePath();
		exists = path.empty();
	}
	else if (type.isPython()) {

	}
	else {

	}

	using namespace FU::ImGui::Operators;

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg) / 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

	ImGui::Spacing();

	ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
	if (ImGui::InputText("##EntryPath", temp.entryFilePath.data(), temp.entryFilePath.capacity())) {
		save.tryPush(List::EntryFilePath);
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void FS::Project::Property::right_luaVersion() {
	using namespace FU::ImGui::Operators;

	ImGui::Spacing();
	ImGui::BulletText(text.currentVersion);

	//5.4.3
	static char version[] = "5.4.3";

	varRead->bulletDummy();

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg) / 2.0f);
	ImGui::InputText("##LuaVersion", version, std::strlen(version), ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleColor();
}

bool FS::Project::Property::changeProjectType(const FD::Project::Property::ProjectType type) {
	//confirm
	const auto index = FU::MB::ok_cancel(text.confirm_changeProjectType);
	if (index == 0) {//ok
		this->type.change = type;
		this->type.changeFlag = true;
		return true;
	}
	else if (index == 1) {//cancel
		return false;
	}
	else {
		FluidumScene_Log_InternalWarning();
		FD::Text::Common errText(FD::Text::CommonText::UnexpectedError);
		FU::MB::error(errText.c_str());
		return false;
	}
}

void FS::Project::Property::information() {
	using namespace FU::ImGui::Operators;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImGui::GetStyle().WindowPadding * 2.0f);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.01f,0.01f,0.01f,1.0f });

	ImGui::BeginChild("##PropInfo", style.informationWindowSize);

	if (info.text) {
		ImGui::BulletText(info.title);
		varRead->bulletDummy();
		ImGui::Spacing();
		ImGui::Text(info.text);
	}


	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void FS::Project::Property::bottom() {
	ImGui::BeginChild("##PropBottom");

	const ImVec2 size = { varRead->popupWindowBottomButtonWidth(style.windowWidth) ,0.0f };

	//close
	ImGui::PushStyleColor(ImGuiCol_Button, colorRead->backButton());
	if (ImGui::Button(text.bottom_close, size)) {
		flag.window = false;
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	//cancel
	ImGui::PushStyleColor(ImGuiCol_Button, colorRead->cancelButton());
	if (ImGui::Button(text.bottom_cancel, size)) {
		this->init();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	//save
	ImGui::PushStyleColor(ImGuiCol_Button, colorRead->saveButton());
	if (ImGui::Button(text.bottom_save, size)) {
		this->saveAllChanges();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	//OK
	ImGui::PushStyleColor(ImGuiCol_Button, colorRead->createButton());
	if (ImGui::Button("OK", size)) {
		this->saveAllChanges();
		FluidumScene_Log_RequestDeleteScene(::FS::Project::Property);
		Scene::deleteScene<Property>();
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();
}

void FS::Project::Property::saveAllChanges() {
	for (auto x : save.changed) {

		if (x == List::CurrentProjectType) {//force save
			FluidumScene_Log_InternalWarning();
		}
		else if (x == List::EntryFilePath) {
			this->save_entryFilePath();
		}
		else {
			FluidumScene_Log_InternalWarning();
		}

	}

	save.clear();

	//save
	propertyWrite->save();
}

bool FS::Project::Property::confirmNotSaved() {
	if (save.isPropertyChanged()) {
		const auto index = FU::MB::button_button_cancel(
			FU::MB::Icon::Warning,
			text.confirm_changeProjectType,
			text.confirm_save,
			text.confirm_ignore
		);

		if (index == 0) //save
			this->saveAllChanges();
		else if (index == 1) //ignore
			;
		else if (index == 2) //cancel
			return false;
		else {
			FluidumScene_Log_InternalWarning();
			FD::Text::Common errText(FD::Text::CommonText::UnexpectedError);
			FU::MB::error(errText.c_str());
			return false;
		}
	}
	return true;
}

void FS::Project::Property::save_entryFilePath() {
	std::string newPath = temp.entryFilePath.c_str();

	if (type.isLua()) {
		propertyLuaWrite->entryFilePath(newPath);
	}
}

void FS::Project::Property::dummy() {
	ImGui::Dummy({ 0.0f, style.dummy });
}

void FS::Project::Property::dummyComboArrowButton(const float textWidth) {
	const float width = ImGui::GetColumnWidth();
	ImGui::Dummy({ width - textWidth - ImGui::GetFrameHeight(), 0.0f });
	ImGui::SameLine();
}

std::vector<FS::Project::Property::Node> FS::Project::Property::makeNode_none_main() const {
	std::vector<Node> result{};

	Node info{
		.label = text.currentType,
		.infoText = text.info_currentType,
		.function = &Property::right_currentProjectType,
		.list = List::CurrentProjectType
	};

	result.emplace_back(info);

	return result;
}

std::vector<FS::Project::Property::Node> FS::Project::Property::makeNode_lua_main() const {
	std::vector<Node> result{};

	Node info{
		.label = text.currentType,
		.infoText = text.info_currentType,
		.function = &Property::right_currentProjectType,
		.list = List::CurrentProjectType
	};

	result.emplace_back(info);

	info.label = text.entryFilePath;
	info.infoText = text.info_entryFilePath;
	info.function = &Property::right_entryFilePath;
	info.list = List::EntryFilePath;

	result.emplace_back(info);

	info.label = text.luaVersion;
	info.infoText = text.info_luaVersion;
	info.function = &Property::right_luaVersion;
	info.list = List::Version;

	result.emplace_back(info);

	return result;
}

std::vector<FS::Project::Property::Node> FS::Project::Property::makeNode_lua_language() const {
	std::vector<Node> result{};

	Node info{
		.label = text.luaVersion,
		.infoText = text.info_luaVersion,
		.function = &Property::right_luaVersion,
		.list = List::Version
	};

	result.emplace_back(info);

	return result;
}

void FS::Project::Property::init() {
	//project type
	type.current = propertyRead->currentProjectType();
	if (type.isLua())
		type.lua = true;
	else if (type.isPython())
		type.python = true;
	else if (type.isCpp())
		type.cpp = true;


	//entry file
	if (type.isLua())
		temp.entryFilePath = propertyLuaRead->entryFilePath();
	temp.entryFilePath.reserve(FU::File::maxPathSize());

	//node
	{
		node.none_main = this->makeNode_none_main();
		node.lua_main = this->makeNode_lua_main();
		node.lua_language = this->makeNode_lua_language();
	}

}


























