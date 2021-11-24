#pragma once

#include "../Common/type.h"

namespace LuAssist::Utils {

	void printStack(State L);

	void popAll(State L);
	void popAllAndPushFalse(State L);
	void popAllAndPushTrue(State L);

	[[nodiscard]] std::vector<BasicType> basicTypes(State L);

	//integer       > number
	//cfnction      > function
	//lightuserdata > userdata
	[[nodiscard]] Type type(State L,const int32_t index);
	[[nodiscard]] std::vector<Type> types(State L);


	[[nodiscard]] std::size_t numOfArgs(State L);


	const char* typeName(const Type type) noexcept;

	//[Source {},Line {}]
	[[nodiscard]] std::string getSrcCurrentLine(State L,const int32_t depth);

	[[nodiscard]] int32_t lineDefined(State L);
	[[nodiscard]] int32_t currentLine(State L);

	[[nodiscard]] int32_t size(State L);

	//void setPosColorVerticesAndListIndices(LuaType::State L, FD::Object::Vertices* vertices, FD::Object::Indices* indices);
	//void setPosColorVertices(LuaType::State L, FD::Object::Vertices* vertices,const std::vector<uint32_t>& changedIndices);
	//
	//void setIndices(LuaType::State L, FD::Object::Indices* indices,const std::vector<uint32_t>& changedIndices);
	//void setSingleLineIndices(LuaType::State L, FD::Object::Indices* indices,const std::vector<uint32_t>& changedIndices);

	//bool isPosColor(LuaType::State L,const int32_t tableStackIndex);

	//���l�݂̂�table��n������
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