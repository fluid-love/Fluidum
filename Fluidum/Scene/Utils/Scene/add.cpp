#include "add.h"

#include "../../Analysis/Overview/overview.h"

#include "../../Coding/Tab/tab.h"
#include "../../Coding/TextEditor/texteditor.h"

FS::Utils::AddScenes::AddScenes(
	const std::vector<FU::Class::ClassCode::CodeType>& codes
) : codes(codes)
{
	GLog.add<FD::Log::Type::None>("Construct Utils::AddScenesScene.");
}

FS::Utils::AddScenes::~AddScenes() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Utils::AddScenesScene.");
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

void FS::Utils::AddScenes::call() {
	using namespace FU::Class;

	for (const auto x : this->codes) {


		if (x == ClassCode::GetClassCode<::FS::Analysis::Overview>()) {
			GLog.add<FD::Log::Type::None>("Request add AnalysisOverviewScene.");
			Scene::addScene<::FS::Analysis::Overview>();
		}
		else if (x == ClassCode::GetClassCode<::FS::TextEditor>()) {
			GLog.add<FD::Log::Type::None>("Request add TextEditorScene.");
			Scene::addScene<::FS::TextEditor>();
		}
		else if (x == ClassCode::GetClassCode<::FS::Coding::Tab>()) {
			GLog.add<FD::Log::Type::None>("Request add Coding::TabScene.");
			Scene::addScene<::FS::Coding::Tab>();
		}
	

	}

	GLog.add<FD::Log::Type::None>("Request delete Utils::AddScenesScene.");
	Scene::deleteScene<AddScenes>();
}
