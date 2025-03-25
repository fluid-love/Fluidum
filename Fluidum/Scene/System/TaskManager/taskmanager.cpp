#include "taskmanager.h"

#pragma comment(lib, "pdh.lib")

FS::System::TaskManager::TaskManager() {
	FluidumScene_Log_Constructor(::FS::System::TaskManager);

	style.leftWidth =
		(ImGui::CalcTextSize(ICON_MD_MEMORY).x * style.iconFontscale) +
		(ImGui::GetStyle().FramePadding.x * 2.0f) +
		(ImGui::GetStyle().FrameBorderSize * 1.75f * 2.0f);

	this->imPlotContext = ImPlot::CreateContext();
	ImPlot::StyleColorsDark();
	ImPlot::GetStyle().Colors[ImPlotCol_AxisBg] = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	ImPlot::GetStyle().Colors[ImPlotCol_FrameBg] = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);

	GetPhysicallyInstalledSystemMemory(&memInfo.max);

	this->constructCpuInfo();

	//style.minSizeLimit = { style.leftWidth, 200.0f };
	style.intervalComboWidth = ImGui::CalcTextSize("000").x;
}

FS::System::TaskManager::~TaskManager() noexcept {

	ImPlot::DestroyContext();

	//cpu
	{
		PdhCloseQuery(cpuInfo.query);
	}

	FluidumScene_Log_Destructor(::FS::System::TaskManager);
}

void FS::System::TaskManager::call() {
	ImPlot::SetCurrentContext(this->imPlotContext);

	this->update();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

	const ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("Taskbar", &flag.window, flags);

	this->left();

	ImGui::SameLine();

	this->right();

	ImGui::PopStyleVar();

	ImGui::End();

}

void FS::System::TaskManager::constructCpuInfo() {
	PdhOpenQuery(NULL, NULL, &cpuInfo.query);
	PdhAddCounter(cpuInfo.query, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuInfo.total);

	SYSTEM_INFO systemInfo{};
	GetSystemInfo(&systemInfo);

	{
		if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			cpuInfo.processorArchiitecture = "x64";
		else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			cpuInfo.processorArchiitecture = "x86";
		else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64)
			cpuInfo.processorArchiitecture = "ARM64";
		else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
			cpuInfo.processorArchiitecture = "ARM";
		else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
			cpuInfo.processorArchiitecture = "Intel Itanium-based";
		else if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN)
			cpuInfo.processorArchiitecture = "Unknown";
		else
			cpuInfo.processorArchiitecture = "Error";
	}

	{
		SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info{};
		DWORD val = sizeof(info);
		const BOOL result = GetLogicalProcessorInformationEx(RelationProcessorPackage, &info, &val);
		if (result == FALSE) {
			cpuInfo.numOfPhysicalProcesser = -1;
		}
		else {
			const bool smt = (info.Processor.Flags == LTP_PC_SMT);
			cpuInfo.numOfPhysicalProcesser = static_cast<ISize>(systemInfo.dwNumberOfProcessors / (smt ? 2 : 1));
		}
		cpuInfo.numOfLogicalProcesser = static_cast<ISize>(systemInfo.dwNumberOfProcessors);
	}

}

void FS::System::TaskManager::update() {
	timer.now = std::chrono::system_clock::now();
}

void FS::System::TaskManager::catchWindowFlags() {

}

void FS::System::TaskManager::left() {
	constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding;


	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, ImGui::GetStyle().FrameBorderSize * 1.6f);
	const float borderSize = ImGui::GetStyle().FrameBorderSize;
	const float windowBorderSize = ImGui::GetStyle().WindowBorderSize;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { borderSize + windowBorderSize, borderSize + windowBorderSize });
	ImGui::BeginChild("##TaskBarLeft", { style.leftWidth, 0.0f }, false, flags);
	ImGui::PopStyleVar();
	ImGui::SetWindowFontScale(style.iconFontscale);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, borderSize * 1.8f });

	this->icons();

	ImGui::PopStyleVar(2);

	ImGui::EndChild();
}

void FS::System::TaskManager::icons() {
	struct Info final {
		Type type{};
		void(TaskManager::* func)() {};
		const char* icon{};
		ImU32 color{};
	};

	constexpr std::array<Info, magic_enum::enum_count<Type>()> items{
		Info{Type::Overview, &TaskManager::overview, ICON_FA_BORDER_ALL,  FU::ImGui::ConvertImVec4ToImU32(0.2f, 0.4f, 0.95f, 1.0f)},
		Info{Type::Cpu, &TaskManager::cpu, ICON_MD_MEMORY, FU::ImGui::ConvertImVec4ToImU32(0.2f, 0.7f, 0.95f, 1.0f)},
		Info{Type::Gpu, &TaskManager::gpu, ICON_FA_PAINT_BRUSH, FU::ImGui::ConvertImVec4ToImU32(0.5f, 0.4f, 0.35f, 1.0f)},
		Info{Type::Memory, &TaskManager::memory, ICON_FA_MEMORY, FU::ImGui::ConvertImVec4ToImU32(0.7f, 0.4f, 0.45f, 1.0f)},
		Info{Type::Storage, &TaskManager::storage, ICON_MD_STORAGE, FU::ImGui::ConvertImVec4ToImU32(0.4f, 0.4f, 0.95f, 1.0f)},
	};

	constexpr ImU32 colorIfSelected = FU::ImGui::ConvertImVec4ToImU32(0.3f, 0.84f, 0.67f, 1.0f);


	for (auto& x : items) {
		ImGui::PushStyleColor(ImGuiCol_Text, x.color);

		const bool pushCol = (x.type == select.type);
		if (pushCol)
			ImGui::PushStyleColor(ImGuiCol_Border, colorIfSelected);

		if (ImGui::Button(x.icon)) {
			select.type = x.type;
			this->draw = std::bind(x.func, this);
		}

		if (pushCol)
			ImGui::PopStyleColor();

		ImGui::PopStyleColor();
	}
}

void FS::System::TaskManager::right() {
	const ImGuiWindowFlags flags =
		(flag.plotHovered ? ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar :
			ImGuiWindowFlags_NoScrollbar);

	ImGui::BeginChild("##TaskBarRight", ImVec2(), true, flags);

	assert(this->draw.operator bool());
	this->draw();

	ImGui::EndChild();
}

void FS::System::TaskManager::overview() {

}

void FS::System::TaskManager::cpu() {
	if (timer.over(cpuInfo.time, cpuInfo.intervalMilliSeconds)) {
		cpuInfo.time = std::chrono::system_clock::now();

		PDH_FMT_COUNTERVALUE counterVal{};
		PDH_STATUS result = PdhCollectQueryData(cpuInfo.query);
		if (result != ERROR_SUCCESS)
			return;

		PdhGetFormattedCounterValue(cpuInfo.total, PDH_FMT_DOUBLE, NULL, &counterVal);

		const float xVal = static_cast<float>(cpuInfo.val.size()) * static_cast<float>(cpuInfo.intervalMilliSeconds) / 1000.0f;
		cpuInfo.x.emplace_back(xVal);
		const float val = static_cast<float>(counterVal.doubleValue);
		cpuInfo.val.emplace_back(val);

	}

	this->cpu_plot();
	this->cpu_info();
}

void FS::System::TaskManager::cpu_title() {
	constexpr UIF8 scale = 1.4f;
	ImGui::BeginChild("##TMCpuTitle", { 0.0f, scale * ImGui::CalcTextSize("CPU").y });
	ImGui::SetWindowFontScale(scale);
	ImGui::BulletText("CPU");
	ImGui::EndChild();
}

void FS::System::TaskManager::cpu_interval() {

	ImGui::SetNextItemWidth(style.intervalComboWidth);

	const bool begin = ImGui::BeginCombo(
		"(s)##TMCpuInterval",
		interval.current,
		ImGuiComboFlags_NoArrowButton
	);

	if (!begin)
		return;

	for (auto& x : this->interval.interval) {
		const bool cliked = ImGui::Selectable(x.second);
		if (cliked) {
			cpuInfo.intervalMilliSeconds = x.first;
			interval.current = x.second;
		}
	}

	ImGui::EndCombo();

}

void FS::System::TaskManager::cpu_plot() {
	ImGui::Spacing();

	this->cpu_title();

	ImGui::Spacing();

	this->cpu_interval();

	ImPlot::SetNextAxisLimits(ImAxis_Y1, -0.5, 100.0, ImPlotCond_Always);

	constexpr ImPlotFlags plotFlags =
		ImPlotFlags_NoMouseText |
		ImPlotFlags_NoChild |
		ImPlotFlags_NoTitle |
		ImPlotFlags_NoFrame |
		ImPlotFlags_NoLegend;

	if (!ImPlot::BeginPlot("cpu", { -1.0f, 0.0f }, plotFlags))
		return;

	ImPlot::SetupAxes("(s)", "(%)");

	ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 3.0f);
	ImPlot::PlotLine(
		"",
		cpuInfo.x.data(),
		cpuInfo.val.data(),
		static_cast<int>(cpuInfo.val.size())
	);
	ImPlot::PopStyleVar();

	flag.plotHovered = ImPlot::IsPlotHovered();

	ImPlot::EndPlot();
}

void FS::System::TaskManager::cpu_info() {
	ImGui::Spacing();

	if (!ImGui::BeginTable("##CpuInfo", 2))
		return;

	//Archiitecture
	ImGui::TableNextColumn();
	ImGui::BulletText(text.cpuArchitecture);

	ImGui::TableNextColumn();
	ImGui::Text(cpuInfo.processorArchiitecture);

	//PhysicalProcessor
	ImGui::TableNextColumn();
	ImGui::BulletText(text.numOfPhysicalProcessor);

	ImGui::TableNextColumn();
	ImGui::Text("%.d", cpuInfo.numOfPhysicalProcesser);

	//LogicalProcessor
	ImGui::TableNextColumn();
	ImGui::BulletText(text.numOfLogicalProcessor);

	ImGui::TableNextColumn();
	ImGui::Text("%.d", cpuInfo.numOfLogicalProcesser);

	ImGui::EndTable();
}

void FS::System::TaskManager::gpu() {

}

void FS::System::TaskManager::memory() {
	MEMORYSTATUSEX memStatus{};
	memStatus.dwLength = sizeof(MEMORYSTATUSEX);

	if (timer.over(memInfo.time, 1000)) {
		memInfo.time = std::chrono::system_clock::now();


		const BOOL result = GlobalMemoryStatusEx(&memStatus);

		if (result == FALSE) {
			ImGui::Text("sss");
		}

		//memInfo.x.emplace_back(memInfo.val.size());
		//const float val = static_cast<float>(memStatus.dwMemoryLoad);
		//memInfo.val.emplace_back(val);
	}

	this->memory_plot();
}

void FS::System::TaskManager::memory_plot() {
	ImGui::BulletText("memory");

	ImPlot::SetNextAxisLimits(ImAxis_Y1, -0.5, 100.0, ImPlotCond_Always);

	if (!ImPlot::BeginPlot("mem", { -1.0f, 0.0f }, ImPlotFlags_NoChild | ImPlotFlags_CanvasOnly))
		return;

	ImPlot::SetupAxes("time", "load");

	ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 3.0f);
	ImPlot::PlotLine(
		"",
		memInfo.x.data(),
		memInfo.val.data(),
		static_cast<int>(memInfo.val.size())
	);
	ImPlot::PopStyleVar();

	ImPlot::EndPlot();
}

void FS::System::TaskManager::storage()
{
}

