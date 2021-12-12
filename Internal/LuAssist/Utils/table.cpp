#include "table.h"
#include "misc.h"

//void FS::LuaAssist::setPosColorVerticesAndListIndices(LuaType::State L, FD::Object::Vertices* vertices, FD::Object::Indices* indices) {
//	assert(getCoreTypes(L).size() == 1);
//
//	glm::vec3 pos;
//	glm::vec4 color;
//
//	for (uint32_t subLoop = 1; subLoop < std::numeric_limits<uint32_t>::max(); subLoop++) {
//
//		if (lua_rawgeti(L, 1, subLoop) != LUA_TNIL) {
//			lua_rawgeti(L, 2, 1);//(L,âΩî‘ñ⁄Ç…ì«Ç›çûÇﬁtableÇ™Ç†ÇÈ,tableÇÃâΩî‘ñ⁄Çì«Ç›çûÇﬁÇ©)
//
//			//posÇéÊìæ
//			for (uint16_t k = 1; k < 4; k++) {
//				lua_rawgeti(L, 3, k);
//
//				pos[k - 1] = (float)lua_tonumber(L, k + 3) / 10.0f;
//			}
//			lua_pop(L, 4);
//
//			//colorÇéÊìæ
//			lua_rawgeti(L, 2, 2);
//
//			for (uint16_t k = 1; k < 5; k++) {
//				lua_rawgeti(L, 3, k);
//
//				color[k - 1] = (float)lua_tonumber(L, k + 3) / 100.0f;
//			}
//			lua_pop(L, 6);
//
//			//ì¸ÇÍÇÈ
//			FD::Object::Vertex ver = { pos,color };
//			vertices->emplace_back(ver);
//
//			//0,1   1,2   2,3  ....
//			indices->emplace_back(subLoop - 1);
//			indices->emplace_back(subLoop);
//		}
//		else { break; }
//	}
//
//	//2Ç¬ëΩÇ¢ÇÃÇ≈è¡Ç∑
//	indices->pop_back();
//	indices->pop_back();
//}
//
//void FS::LuaAssist::setPosColorVertices(LuaType::State L, FD::Object::Vertices* vertices, const std::vector<uint32_t>& changedIndices) {
//	assert(getCoreTypes(L).size() == 1);
//
//	glm::vec3 pos;
//	glm::vec4 color;
//
//	//sizeÇämï€Ç∑ÇÈ
//	auto itr = std::max_element(changedIndices.begin(), changedIndices.end());
//	if (vertices->size() < *itr)
//		vertices->resize(*itr);
//
//
//	for (uint32_t i = 0; i < changedIndices.size(); i++) {
//		if (lua_rawgeti(L, 1, changedIndices[i]) != LUA_TNIL) {
//			lua_rawgeti(L, 2, 1);//(L,âΩî‘ñ⁄Ç…ì«Ç›çûÇﬁtableÇ™Ç†ÇÈ,tableÇÃâΩî‘ñ⁄Çì«Ç›çûÇﬁÇ©)
//
//			//posÇéÊìæ
//			for (uint16_t k = 1; k < 4; k++) {
//				lua_rawgeti(L, 3, k);
//
//				pos[k - 1] = (float)lua_tonumber(L, k + 3) / 10.0f;
//			}
//			lua_pop(L, 4);
//
//			//colorÇéÊìæ
//			lua_rawgeti(L, 2, 2);
//
//			for (uint16_t k = 1; k < 5; k++) {
//				lua_rawgeti(L, 3, k);
//
//				color[k - 1] = (float)lua_tonumber(L, k + 3) / 100.0f;
//			}
//			lua_pop(L, 6);
//
//			//ì¸ÇÍÇÈ
//			FD::Object::Vertex ver = { pos,color };
//			vertices->at(changedIndices[i] - 1) = ver;
//		}
//		else { break; }
//	}
//
//}

std::vector<LuAssist::Num> LuAssist::Utils::Table::convertTableToArrayOfNumbers(State L, const int32_t tableIndex) {

	if (lua_istable(L, tableIndex))
		throw Exception::NotTable();

	std::vector<Num> result{};
	result.reserve(1000);

	int32_t i = 1;
	while (!lua_isnone(L, i++)) {
		//push back
		lua_rawgeti(L, tableIndex, -1);

		if (!lua_isnumber(L, -1))
			throw Exception::NotNumber{ static_cast<std::size_t>(i - 1) ,typeName(type(L,-1)) };

		result.emplace_back(lua_tonumber(L, -1));
		lua_pop(L, 1);
	}

	return result;
}

std::vector<std::string> LuAssist::Utils::Table::convertTableToArrayOfStrings(State L, const int32_t tableIndex) {

	if (lua_istable(L, tableIndex))
		throw Exception::NotTable();

	std::vector<std::string> result{};
	result.reserve(1000);

	int32_t i = 1;
	while (!lua_isnone(L, i++)) {
		//push back
		lua_rawgeti(L, tableIndex, -1);

		if (!lua_isstring(L, -1))
			throw Exception::NotString{ static_cast<std::size_t>(i - 1) ,typeName(type(L,-1)) };

		result.emplace_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	return result;
}

std::vector<bool> LuAssist::Utils::Table::convertTableToArrayOfBooleans(State L, const int32_t tableIndex) {

	if (lua_istable(L, tableIndex))
		throw Exception::NotTable();

	std::vector<bool> result{};
	result.reserve(1000);

	int32_t i = 1;
	while (!lua_isnone(L, i++)) {
		//push back
		lua_rawgeti(L, tableIndex, -1);

		if (!lua_isboolean(L, -1))
			throw Exception::NotBoolean{ static_cast<std::size_t>(i - 1) ,typeName(type(L,-1)) };

		result.emplace_back(lua_toboolean(L, -1));
		lua_pop(L, 1);
	}

	return result;
}



























