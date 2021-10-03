#include "../FSlua.h"

/*
1:index
2:BufferKey
3:changedValueIndices
4:table
*/

using namespace FS::LuaType;

template<>
class FS::LuaCheck::Check<Draw::UPDATE_VERTEX> final : private Common<> {
public:
	using Common::Common;

private:
	const char* const funcName = "FDraw.UpdateVertex";

public:
	void arg() {
		const auto types = LuaAssist::getCoreTypes(L);

		if (types.size() > 4) {
			//ŠÖ”{}‚Ìˆø”‚Ì”‚ª‘½‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
			LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
			log->add<FD::LogType::ERROR>(text, funcName, types.size(), "4");
			throw std::runtime_error("");
		}

		if (types[0] != CoreType::INTEGER) {
			//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
			throw std::runtime_error("");
		}

		if (types[1] != CoreType::STRING) {
			//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 2, LuaAssist::getCoreTypeName(types[1]), LuaAssist::getCoreTypeName(CoreType::STRING));
			throw std::runtime_error("");
		}

		if (types[2] != CoreType::TABLE) {
			//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 3, LuaAssist::getCoreTypeName(types[2]), LuaAssist::getCoreTypeName(CoreType::TABLE));
			throw std::runtime_error("");
		}

		if (types[3] != CoreType::TABLE) {
			//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 4, LuaAssist::getCoreTypeName(types[3]), LuaAssist::getCoreTypeName(CoreType::TABLE));
			throw std::runtime_error("");
		}
	}
};

Ret FS::Lua::updateVertex(State L) {
	LuaCheck::Check<Draw::UPDATE_VERTEX> check(L, log);
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

	Fluidum::Utils::Container::eraseDuplicateAscendingSort(changedIndices);

	//table‚¾‚¯‚É‚·‚é
	lua_remove(L, 1);
	lua_remove(L, 1);
	lua_remove(L, 1);

	auto lock = fvkKey->getLock();
	auto vLock = vertex->getLock();

	LuaAssist::setPosColorVertices(L, &vertex->vertices.at(dataIndex), changedIndices);

	const FVK::VertexBufferKey<const char*> key(bufferKey);

	uint32_t nextSize = static_cast<uint32_t>(commands->getIndices(FVK::CommandType::NEXT).size());

	std::vector<FVK::UpdateVertexBufferCommand> updateVertexBufferCommands;

	//commandbufferkey‚ª‚Í‚¶‚ß‚É‚ ‚é‚Ì‚Í•ÛØ‚·‚é
	for (uint32_t i = 0; i < nextSize; i++) {
		updateVertexBufferCommands.emplace_back(vk->makeUpdateVertexBufferCommand(fvkKey->commandBuffers.at(i), key));
		updateVertexBufferCommands[i]->setDataSize(sizeof(FD::Object::Vertex) * vertex->vertices[dataIndex].size());
		updateVertexBufferCommands[i]->setVerticesPtr(vertex->vertices[dataIndex].data());
	}
	
	lock.unlock();
	vLock.unlock();

	//call‚³‚ê‚é‚Ü‚Åwait‚³‚ê‚é->ŠmÀ‚Éƒ^ƒCƒ~ƒ“ƒO‚æ‚­“ü‚ê‚ç‚ê‚é
	commands->insertOnceAfterCurrentPos(FVK::CommandType::BEGIN_COMMAND_BUFFER, updateVertexBufferCommands);

	LuaAssist::pop(L);
	return 0;
}
