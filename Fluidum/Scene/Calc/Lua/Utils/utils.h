#pragma once

#include "table.h"

namespace FS::Lua {

	void printStack(State L);
	void pop(State L);
	void popAndPushFalse(State L);
	void popAndPushTrue(State L);

	//integer > number
	std::vector<CoreType> getCoreTypes(State L);

	_NODISCARD std::size_t getArgSize(State L);

	const char* getCoreTypeName(const CoreType type) noexcept;

	//void setPosColorVerticesAndListIndices(LuaType::State L, FD::Object::Vertices* vertices, FD::Object::Indices* indices);
	//void setPosColorVertices(LuaType::State L, FD::Object::Vertices* vertices,const std::vector<uint32_t>& changedIndices);
	//
	//void setIndices(LuaType::State L, FD::Object::Indices* indices,const std::vector<uint32_t>& changedIndices);
	//void setSingleLineIndices(LuaType::State L, FD::Object::Indices* indices,const std::vector<uint32_t>& changedIndices);

	//bool isPosColor(LuaType::State L,const int32_t tableStackIndex);

	//êîílÇÃÇ›ÇÃtableÇìnÇ∑Ç±Ç∆
	//template<std::integral T >
	//std::vector<T> toVector(LuaType::State L) {
	//	std::vector<T> result;
	//	uint16_t i = 1;
	//	while (lua_rawgeti(L, 1, i) != LUA_TNIL) {
	//		result.emplace_back(static_cast<T>(lua_tointeger(L, -1) - 1));
	//		i++;
	//	}
	//	return result;
	//}

}