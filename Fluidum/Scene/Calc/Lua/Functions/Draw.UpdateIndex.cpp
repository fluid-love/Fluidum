#include "../FSlua.h"

/*
1:index
2:BufferKey
3:changedValueIndices
4:table
*/

using namespace FS::LuaType;

template<>
class FS::LuaCheck::Check<Draw::UPDATE_INDEX> final : private Common<> {
public:
	using Common::Common;

private:
	const char* const funcName = "FDraw.UpdateIndex";

public:
	void arg() {
		const auto types = LuaAssist::getCoreTypes(L);

		if (types.size() > 4) {
			//関数{}の引数の数が多すぎます．渡された引数の数:{}．正しい引数の数:{}．
			LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
			log->add<FD::LogType::ERROR>(text, funcName, types.size(), "4");
			throw std::runtime_error("");
		}

		if (types[0] != CoreType::INTEGER) {
			//関数{}の{}番目の引数に誤った型が渡されました．渡された引数の型:{}．正しい型:{}．
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
			throw std::runtime_error("");
		}

		if (types[1] != CoreType::STRING) {
			//関数{}の{}番目の引数に誤った型が渡されました．渡された引数の型:{}．正しい型:{}．
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 2, LuaAssist::getCoreTypeName(types[1]), LuaAssist::getCoreTypeName(CoreType::STRING));
			throw std::runtime_error("");
		}

		if (types[2] != CoreType::TABLE) {
			//関数{}の{}番目の引数に誤った型が渡されました．渡された引数の型:{}．正しい型:{}．
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 3, LuaAssist::getCoreTypeName(types[2]), LuaAssist::getCoreTypeName(CoreType::TABLE));
			throw std::runtime_error("");
		}

		if (types[3] != CoreType::TABLE) {
			//関数{}の{}番目の引数に誤った型が渡されました．渡された引数の型:{}．正しい型:{}．
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 4, LuaAssist::getCoreTypeName(types[3]), LuaAssist::getCoreTypeName(CoreType::TABLE));
			throw std::runtime_error("");
		}
	}
};

Ret FS::Lua::updateIndex(State L) {
	LuaCheck::Check<Draw::UPDATE_INDEX> check(L, log);
	check.arg();

	const Val dataIndex = lua_tointeger(L, 1);

	String bufferKey = lua_tostring(L, 2);

	std::vector<uint32_t> changedIndices;
	for (uint32_t i = 1; i < std::numeric_limits<uint32_t>::max(); i++) {
		if (lua_rawgeti(L, 3, i) != LUA_TNIL) {
			changedIndices.emplace_back(static_cast<uint32_t>(lua_tointeger(L, -1)));
			lua_pop(L, 1);
		}
		else {
			lua_pop(L, 1);
			break;
		}
	}

	if (changedIndices.empty()) {
		LuaAssist::pop(L);
		return 0;
	}

	//tableだけにする
	lua_remove(L, 1);
	lua_remove(L, 1);
	lua_remove(L, 1);

	LuaAssist::setIndices(L, &vertex->indices.at(dataIndex), changedIndices);

	const FVK::IndexBufferKey<const char*> key(bufferKey);

	auto lock = fvkKey->getLock();
	auto vLock = vertex->getLock();


	std::vector<FVK::UpdateIndexBufferCommand> updateIndexBufferCommands;

	uint32_t nextSize = static_cast<uint32_t>(commands->getIndices(FVK::CommandType::NEXT).size());

	for (uint32_t i = 0; i < nextSize; i++) {
		updateIndexBufferCommands.emplace_back(vk->makeUpdateIndexBufferCommand(fvkKey->commandBuffers.at(i), key));
		updateIndexBufferCommands[i]->setDataSize(sizeof(FD::Object::Vertex) * vertex->indices[dataIndex].size());
		updateIndexBufferCommands[i]->setIndicesPtr(vertex->indices[dataIndex].data());
	}

	lock.unlock();

	commands->insertOnceAfterCurrentPos(FVK::CommandType::BEGIN_COMMAND_BUFFER, updateIndexBufferCommands);

	auto drawIndexedIndices = commands->getIndices(FVK::CommandType::DRAW_INDEXED);
	//この間に削除などをされるとまずいが，仕様上このスレッドが動いている間は他のスレッドから削除させない
	for (std::size_t i = dataIndex; i < drawIndexedIndices.size(); i += vertex->indices.size()) {
		auto data = commands->at<FVK::CommandType::DRAW_INDEXED>(drawIndexedIndices[i]);
		data->setDrawSize(static_cast<uint32_t>(vertex->indices[dataIndex].size()));
	}


	LuaAssist::pop(L);
	return 0;
}
