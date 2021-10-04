#include "utils.h"

void FS::Lua::printStack(State L) {
	/*参考：http://marupeke296.com/LUA_No2_Begin.html　*/
	/*
	　「○×（まるぺけ）つくろーどっとコム」をお訪ね頂きまして真にありがとうございます。
　	    ○×つくろーどっとコム内に掲載されているプログラムソースは、ゲームを作りたいと考えている皆様に自由に使用して頂きたいと考えております。しかしながら、出来る限りの見直しを行ってはおりますが、管理人が気が付かないスペルミスや論理的バグが含まれている事があります。よって、その動作の保障は致しかねますので予めご了承下さい。うまく動かない箇所が御座いましたら、是非連絡を頂きたく思います。
　      本サイトのソースはオープンソースです。よって、コピーや改変は自由に行って頂いて結構です。トリビアな話になりますが、プログラムの著作権については、著作権法の中ではあまり明確に定義されておりません。一般に、ある目的を達しようとするプログラムは必然的に似る傾向にあることから、プログラムソースがよっぽど特徴的であること以外、その著作権というのは殆ど認められていないようです。
　      しかし、本サイトのプログラムは一部参照文献の内容を参考にさせて頂いている部分があります。それに関しまして掲載上の問題がある場合はご連絡を頂ければと思います。
		(2005.5.28 管理人 IKD)
	*/

	// スタック数を取得
	const int num = lua_gettop(L);
	if (num == 0) {
		printf("No stack.\n");
		return;
	}
	for (int i = num; i >= 1; i--) {
		printf("%03d(%04d): ", i, -num + i - 1);
		int type = lua_type(L, i);
		switch (type) {
		case LUA_TNIL:
			printf("NIL\n");
			break;
		case LUA_TBOOLEAN:
			printf("BOOLEAN %s\n", lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TLIGHTUSERDATA:
			printf("LIGHTUSERDATA\n");
			break;
		case LUA_TNUMBER:
			printf("NUMBER %f\n", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("STRING %s\n", lua_tostring(L, i));
			break;
		case LUA_TTABLE:
			printf("TABLE\n");
			break;
		case LUA_TFUNCTION:
			printf("FUNCTION\n");
			break;
		case LUA_TUSERDATA:
			printf("USERDATA\n");
			break;
		case LUA_TTHREAD:
			printf("THREAD\n");
			break;
		}
	}
	printf("-----------------------------\n\n");
}

void FS::Lua::pop(State L) {
	int32_t num = lua_gettop(L);
	lua_pop(L, num);
}

void FS::Lua::popAndPushFalse(State L) {
	int32_t num = lua_gettop(L);
	lua_pop(L, num);
	lua_pushboolean(L, false);
}

void FS::Lua::popAndPushTrue(State L) {
	int32_t num = lua_gettop(L);
	lua_pop(L, num);
	lua_pushboolean(L, true);
}

std::vector<FS::Lua::CoreType> FS::Lua::getCoreTypes(State L) {
	int32_t i = 1;
	std::vector<CoreType> types = {};

	//全ての引数
	while (!lua_isnone(L, i)) {
		if (lua_isinteger(L, i))
			types.emplace_back(CoreType::Integer);
		else if (lua_isnumber(L, i))
			types.emplace_back(CoreType::Number);
		else if (lua_isstring(L, i))
			types.emplace_back(CoreType::String);
		else if (lua_isfunction(L, i))
			types.emplace_back(CoreType::Function);
		else if (lua_istable(L, i))
			types.emplace_back(CoreType::Table);
		else if (lua_isboolean(L, i))
			types.emplace_back(CoreType::Boolean);
		else if (lua_isnil(L, i))
			types.emplace_back(CoreType::Nil);

		i++;
	}

	return types;
}

std::size_t FS::Lua::getArgSize(State L) {
	int32_t i = 0;
	int32_t count = 0;
	while (!lua_isnone(L, i)) {
		i++;
		count++;
	}
	return count;
}

const char* FS::Lua::getCoreTypeName(const CoreType type) noexcept {
	if (type == CoreType::Integer)
		return "Integer";
	else if (type == CoreType::Number)
		return "Number";
	else if (type == CoreType::String)
		return "String";
	else if (type == CoreType::Function)
		return "Function";
	else if (type == CoreType::Table)
		return "Table";
	else if (type == CoreType::Boolean)
		return "Boolean";
	else
		return "Nil";
}

//void FS::LuaAssist::setPosColorVerticesAndListIndices(LuaType::State L, FD::Object::Vertices* vertices, FD::Object::Indices* indices) {
//	assert(getCoreTypes(L).size() == 1);
//
//	glm::vec3 pos;
//	glm::vec4 color;
//
//	for (uint32_t subLoop = 1; subLoop < std::numeric_limits<uint32_t>::max(); subLoop++) {
//
//		if (lua_rawgeti(L, 1, subLoop) != LUA_TNIL) {
//			lua_rawgeti(L, 2, 1);//(L,何番目に読み込むtableがある,tableの何番目を読み込むか)
//
//			//posを取得
//			for (uint16_t k = 1; k < 4; k++) {
//				lua_rawgeti(L, 3, k);
//
//				pos[k - 1] = (float)lua_tonumber(L, k + 3) / 10.0f;
//			}
//			lua_pop(L, 4);
//
//			//colorを取得
//			lua_rawgeti(L, 2, 2);
//
//			for (uint16_t k = 1; k < 5; k++) {
//				lua_rawgeti(L, 3, k);
//
//				color[k - 1] = (float)lua_tonumber(L, k + 3) / 100.0f;
//			}
//			lua_pop(L, 6);
//
//			//入れる
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
//	//2つ多いので消す
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
//	//sizeを確保する
//	auto itr = std::max_element(changedIndices.begin(), changedIndices.end());
//	if (vertices->size() < *itr)
//		vertices->resize(*itr);
//
//
//	for (uint32_t i = 0; i < changedIndices.size(); i++) {
//		if (lua_rawgeti(L, 1, changedIndices[i]) != LUA_TNIL) {
//			lua_rawgeti(L, 2, 1);//(L,何番目に読み込むtableがある,tableの何番目を読み込むか)
//
//			//posを取得
//			for (uint16_t k = 1; k < 4; k++) {
//				lua_rawgeti(L, 3, k);
//
//				pos[k - 1] = (float)lua_tonumber(L, k + 3) / 10.0f;
//			}
//			lua_pop(L, 4);
//
//			//colorを取得
//			lua_rawgeti(L, 2, 2);
//
//			for (uint16_t k = 1; k < 5; k++) {
//				lua_rawgeti(L, 3, k);
//
//				color[k - 1] = (float)lua_tonumber(L, k + 3) / 100.0f;
//			}
//			lua_pop(L, 6);
//
//			//入れる
//			FD::Object::Vertex ver = { pos,color };
//			vertices->at(changedIndices[i] - 1) = ver;
//		}
//		else { break; }
//	}
//
//}
//
//void FS::LuaAssist::setIndices(LuaType::State L, FD::Object::Indices* indices, const std::vector<uint32_t>& changedIndices) {
//
//	//sizeを確保する
//	auto itr = std::max_element(changedIndices.begin(), changedIndices.end());
//	indices->resize(*itr);
//
//	for (FD::Object::IndexSize i = 0; i < changedIndices.size(); i++) {
//		if (lua_rawgeti(L, 1, changedIndices[i]) != LUA_TNIL) {
//			indices->at(changedIndices[i] - 1) = static_cast<FD::Object::IndexSize>(lua_tointeger(L, -1) - 1);
//			lua_pop(L, 1);
//		}
//		else {
//			break;
//		}
//	}
//}
//
//void FS::LuaAssist::setSingleLineIndices(LuaType::State L, FD::Object::Indices* indices, const std::vector<uint32_t>& changedIndices) {
//
//}
//
//bool FS::LuaAssist::isPosColor(LuaType::State L, const int32_t tableStackIndex) {
//	lua_rawgeti(L, tableStackIndex, 1);
//
//	if (lua_istable(L, -1)) {
//		lua_pop(L, 1);
//		return true;
//	}
//
//	lua_pop(L, 1);
//	return false;
//}
//
//
//
//
//
//
//
//
//
//
