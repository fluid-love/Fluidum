#include "../FSlua.h"

using namespace FS::LuaType;

template<>
class FS::LuaCheck::Check<Draw::CLEAR> final : private Common<> {
public:
	using Common::Common;

private:
	const char* const funcName = "FDraw.Clear";

public:
	void arg() {
		const auto types = LuaAssist::getCoreTypes(L);
		const auto size = types.size();

		if (size > 0) {
			//ŠÖ”{}‚Ìˆø”‚Ì”‚ª‘½‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
			LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
			log->add<FD::LogType::ERROR>(text, funcName, types.size(), "0");
			throw std::runtime_error("");
		}

	}
};

Ret FS::Lua::clear_Draw(State L) {
	LuaCheck::Check<Draw::CLEAR> check(L, log);
	check.arg();

	commands->erase<FVK::CommandType::DRAW_INDEXED, FVK::CommandType::BIND_INDEX_BUFFER, FVK::CommandType::BIND_VERTEX_BUFFERS>();

	auto vLock = vertex->getLock();

	for (auto& x : vertex->vertexBuffers)
		vk->deleteFvkItem(x);
	for (auto& x : vertex->indexBuffers)
		vk->deleteFvkItem(x);

	vertex->vertices.clear();
	vertex->indices.clear();
	vertex->vertexBuffers.clear();
	vertex->indexBuffers.clear();

	vertex->vertices.shrink_to_fit();
	vertex->indices.shrink_to_fit();
	vertex->vertexBuffers.shrink_to_fit();
	vertex->indexBuffers.shrink_to_fit();

	LuaAssist::pop(L);
	return 0;
}
