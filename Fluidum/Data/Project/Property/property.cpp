#include "property.h"

void FD::Project::PropertyWrite::save() noexcept {
	using namespace Property::Internal;
	Data::save.store(true);
}

void FD::Project::PropertyWrite::currentProjectType(const Property::ProjectType type) noexcept {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//system_error
	Data::currentProjectType = type;
}

FD::Project::Property::ProjectType FD::Project::PropertyRead::currentProjectType() const noexcept {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//system_error
	return Data::currentProjectType;
}

bool FD::Project::PropertyRead::isLua() const noexcept {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//system_error
	return Data::currentProjectType == Property::ProjectType::Lua;
}

bool FD::Project::PropertyRead::isPython() const noexcept {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//system_error
	return Data::currentProjectType == Property::ProjectType::Python;
}

bool FD::Project::PropertyRead::isCpp() const noexcept {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//system_error
	return Data::currentProjectType == Property::ProjectType::Cpp;
}

