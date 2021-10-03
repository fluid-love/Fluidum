#include "../FSlua.h"

using namespace FS::LuaType;

template<>
class FS::LuaCheck::Check<Draw::CREATE> final : private Common<> {
public:
	using Common::Common;

private:
	const char* const funcName = "FDraw.Create";

public:
	_NODISCARD std::size_t arg() {
		const auto types = LuaAssist::getCoreTypes(L);
		const auto size = types.size();

		if (size > 1) {
			//ŠÖ”{}‚Ìˆø”‚Ì”‚ª‘½‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
			LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
			log->add<FD::LogType::ERROR>(text, funcName, types.size(), "0,1,2");
			throw std::runtime_error("");
		}

		if (size == 0)
			return size;

		//if (types[0] != CoreType::TABLE) {
		//	//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
		//	Text::LuaLog<Text::LuaLogType::WRONG_TYPE_ARGUMENT> text;
		//	log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::TABLE));
		//	throw std::runtime_error("");
		//}

		//if (size == 2)
		//	return size;

		if (types[0] != CoreType::STRING) {
			//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::STRING));
			throw std::runtime_error("");
		}

		return size;
	}
};

enum class PrimitiveType : uint16_t {
	NONE,
	LINE_LIST
};

PrimitiveType toPrimitiveType(const std::string str) {
	using enum PrimitiveType;
	if (str == "LINE" || str == "Line" || str == "line" || str == "LineList" || str == "LINELIST" || str == "linelist")
		return LINE_LIST;

	return NONE;
}

Ret FS::Lua::create_Draw(State L) {
	LuaCheck::Check<Draw::CREATE> check(L, log);
	auto size = check.arg();

	{
		auto lock = vertex->getLock();
		vertex->vertices.emplace_back();
		vertex->indices.emplace_back();
	
		vertex->vertexBuffers.emplace_back(std::string("Vertex") + std::to_string(vertex->vertices.size()));
		vertex->indexBuffers.emplace_back(std::string("Index") + std::to_string(vertex->indices.size()));
	}

	{
		auto vlock = vertex->getLock();
		auto lock = fvkKey->getLock();
		//table‚©‚çî•ñ‚ğ“Ç‚İæ‚Á‚Ä“ü‚ê‚é
		//LuaAssist::setPosColorVerticesAndListIndices(L, &vertex->vertices.back(), &vertex->indices.back());

		FVK::VertexBufferKeys vertexBufferKeys(fvkKey->physicalDevice, fvkKey->device, fvkKey->commandPool);
		FVK::VertexBufferParameter vertexBufferParam(&vertex->vertices.back());
		vertexBufferParam.bufferSize = sizeof(FD::Object::Vertex) * 2000;//2000’¸“_•ªŠm•Û
		vk->addVertexBuffer(vertex->vertexBuffers.back(), vertexBufferKeys, vertexBufferParam);

		FVK::IndexBufferKeys indexBufferKeys(fvkKey->physicalDevice, fvkKey->device, fvkKey->commandPool);
		FVK::IndexBufferParameter indexBufferParam(&vertex->indices.back());
		indexBufferParam.bufferSize =  sizeof(FD::Object::IndexSize) * 4000;//4000•ªŠm•Û

		vk->addIndexBuffer(vertex->indexBuffers.back(), indexBufferKeys, indexBufferParam);

		const auto nextSize = commands->getIndices(FVK::CommandType::NEXT).size();

		for (uint32_t i = 0; i < nextSize; i++) {
			FVK::BindVertexBuffersCommand bindVertexBuffer = vk->makeBindVertexBuffersCommand(fvkKey->commandBuffers.at(i), vertex->vertexBuffers.back());
			FVK::BindIndexBufferCommand bindIndexBuffer = vk->makeBindIndexBufferCommand(fvkKey->commandBuffers.at(i), vertex->indexBuffers.back());
			FVK::DrawIndexedCommand drawIndexedCommand = vk->makeDrawIndexedCommand(fvkKey->commandBuffers.at(i));

			auto indices = commands->getIndices(FVK::CommandType::IMGUI_RENDERDRAW_DATA);
			commands->insert(indices.at(i), bindVertexBuffer);
			commands->insert(indices.at(i) + 1, bindIndexBuffer);
			commands->insert(indices.at(i) + 2, drawIndexedCommand);
			drawIndexedCommand->setDrawSize(0);
		}
	}


	//if (size == 2) {
	//	String primitiveTypeStr = lua_tostring(L, 2);
	//	const auto primitiveType = toPrimitiveType(primitiveTypeStr);
	//	lua_pop(L, 1);//string‚ğÁ‚·@‚±‚ê‚ğ‚µ‚È‚¢‚Æassist.setPosColorVerti...‚ªƒGƒ‰[‚ğ“f‚­

	//	auto lock = vertex->getLock();
	//	LuaAssist::setPosColorVerticesAndListIndices(L, &vertex->vertices.back(), &vertex->indices.back());
	//}

	LuaAssist::pop(L);

	//index vertexbufferkey indexbufferkey‚ğÚ‚¹‚Ä•Ô‚·
	auto lock = vertex->getLock();
	lua_pushinteger(L, vertex->vertices.size() - 1);
	lua_pushinteger(L, vertex->indices.size() - 1);

	lua_pushstring(L, vertex->vertexBuffers.back());
	lua_pushstring(L, vertex->indexBuffers.back());

	return 4;
}
