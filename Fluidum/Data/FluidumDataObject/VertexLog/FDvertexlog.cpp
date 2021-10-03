#include "FDvertexlog.h"

using namespace FD::Internal;

FD::VertexLogWrite::VertexLogWrite(Internal::ManagerKey) {

}

void FD::VertexLogWrite::add(const std::vector<uint32_t>& changedIndices, const Object::Vertices* const data) {
	auto target = LogData::keys.at(LogData::currentKey);

	LogData::changed[target].emplace_back(changedIndices);
	for (auto& x : changedIndices) {
		LogData::tempVertexData[target].emplace_back((*data)[x]);
	}
	LogData::indexNum[target].emplace_back(false);
	LogData::num[target].emplace_back(changedIndices.size());

	if (shouldOutFile(target))
		outFile();
}

void FD::VertexLogWrite::add(const std::vector<uint32_t>& changedIndices, const Object::Indices* const data) {
	auto target = LogData::keys.at(LogData::currentKey);

	LogData::changed[target].emplace_back(changedIndices);
	for (auto& x : changedIndices) {
		LogData::tempIndexData[target].emplace_back((*data)[x]);
	}
	LogData::indexNum[target].emplace_back(true);
	LogData::num[target].emplace_back(changedIndices.size());

	if (shouldOutFile(target))
		outFile();
}

void FD::VertexLogWrite::setTargetKey(const FVK::VertexBufferKey<std::string>& key) {
	LogData::currentKey = key;
	if (!LogData::keys.contains(LogData::currentKey)) {
		auto size = LogData::keys.size() + 1;
		LogData::keys.insert(std::make_pair(key, size - 1));
		LogData::num.resize(size);
		LogData::indexNum.resize(size);
		LogData::tempIndexData.resize(size);
		LogData::tempVertexData.resize(size);
		LogData::changed.resize(size);
	}
}

bool FD::VertexLogWrite::shouldOutFile(const std::size_t target) const noexcept {
	if (LogData::num[target].size() == 500)
		return true;
	return false;
}

void FD::VertexLogWrite::outFile() const {
	VertexLogOutFile out(LogData::currentKey);
	out.out();
}

FD::Internal::VertexLogOutFile::VertexLogOutFile(const std::string& key) : target(LogData::keys.at(key)) {
	auto path = TempLogDirectory + std::string("VertexLog" + key + ".vlog");
	this->ofs.open(path, std::ios::app | std::ios::binary);
}

void FD::Internal::VertexLogOutFile::out() {

	if (!ofs)
		throw std::runtime_error("");

	for (auto i = 0; i < LogData::num[target].size(); i++) {
		this->write(i);
	}

	this->reset();
}

void FD::Internal::VertexLogOutFile::write(const uint32_t i) {

	const bool isIndex = LogData::indexNum[target][i];

	//1byte:indexかvertexか
	{
		char val;
		if (isIndex)
			val = 1;//true
		else
			val = 0;//false
		ofs.write(&val, 1);
	}

	//4byte:変更箇所は何個あるか
	auto changedSize = LogData::changed[target][i].size();
	ofs.write(reinterpret_cast<char*>(&changedSize), 4);

	//4byte:変更のあるindex
	ofs.write(reinterpret_cast<char*>(LogData::changed[target][i].data()), sizeof(uint32_t) * changedSize);

	//本体
	if (isIndex) {//i番目の変更がindex		
		ofs.write(reinterpret_cast<char*>(&LogData::tempIndexData[target].at(iPos)), sizeof(Object::IndexSize) * LogData::num[target][i]);
		iPos += LogData::num[target][i];
	}
	else {
		ofs.write(reinterpret_cast<char*>(&LogData::tempVertexData[target].at(vPos)), sizeof(Object::Vertex) * LogData::num[target][i]);
		vPos += LogData::num[target][i];
	}
}

void FD::Internal::VertexLogOutFile::reset() {
	LogData::num[target].clear();
	LogData::indexNum[target].clear();
	LogData::tempIndexData[target].clear();
	LogData::tempVertexData[target].clear();
	LogData::changed[target].clear();
}

//std::pair<FD::VertexLogRead::VertexLogType, FD::VertexLogRead::IndexLogType> FD::VertexLogRead::get(const FVK::VertexBufferKey<std::string>& key, const std::size_t at) {
//
//
//	return std::pair<VertexLogType, IndexLogType>();
//}

std::pair<FD::Object::Vertices, FD::Object::Indices> FD::VertexLogRead::create(const FVK::VertexBufferKey<std::string>& key, const std::size_t at) const {
	//残りを書き込む
	{
		std::string k = key.operator const char* ();
		Internal::VertexLogOutFile out(k);
		out.out();
	}

	auto path = TempLogDirectory + std::string("VertexLog" + std::string(key) + ".vlog");

	std::ifstream ifs(path, std::ios::in | std::ios::binary);

	if (!ifs)
		throw std::runtime_error("Failed to open " + path + " file.");

	Object::Vertices vRes;
	Object::Indices iRes;

	Object::Vertices vTemp;
	Object::Indices iTemp;

	//4byte:何個あるか
	uint32_t num = 0, vPos = 0, iPos = 0;

	std::size_t val = 0;

	while (val != at) {
		val++;

		//1byte:index か vertex
		char isIndex;
		ifs.read(&isIndex, 1);

		if (ifs.eof())
			break;

		ifs.read(reinterpret_cast<char*>(&num), 4);

		std::vector<uint32_t> changed(num);
		ifs.read(reinterpret_cast<char*>(changed.data()), 4 * num);

		if (isIndex) {
			iTemp.resize(num);
			ifs.read(reinterpret_cast<char*>(iTemp.data()), sizeof(Object::IndexSize) * num);

			iRes.resize(num + iPos);
			iPos += num;

			for (auto i = 0; i < changed.size(); i++) {
				iRes[changed[i]] = iTemp[i];
			}
		}
		else {
			vTemp.resize(num);
			ifs.read(reinterpret_cast<char*>(vTemp.data()), sizeof(Object::Vertex) * num);

			vRes.resize(num + vPos);
			vPos += num;

			for (auto i = 0; i < changed.size(); i++) {
				vRes[changed[i]] = vTemp[i];
			}

		}
	}

	return std::pair<Object::Vertices, Object::Indices>(std::move(vRes), std::move(iRes));
}
