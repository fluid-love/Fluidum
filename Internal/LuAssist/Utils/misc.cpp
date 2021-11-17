#include "misc.h"

void LuAssist::Utils::printStack(State L) {
	/*参考：http://marupeke296.com/LUA_No2_Begin.html　*/
	/*
	　「○×（まるぺけ）つくろーどっとコム」をお訪ね頂きまして真にありがとうございます。
　	    ○×つくろーどっとコム内に掲載されているプログラムソースは、ゲームを作りたいと考えている皆様に自由に使用して頂きたいと考えております。しかしながら、出来る限りの見直しを行ってはおりますが、管理人が気が付かないスペルミスや論理的バグが含まれている事があります。よって、その動作の保障は致しかねますので予めご了承下さい。うまく動かない箇所が御座いましたら、是非連絡を頂きたく思います。
　      本サイトのソースはオープンソースです。よって、コピーや改変は自由に行って頂いて結構です。トリビアな話になりますが、プログラムの著作権については、著作権法の中ではあまり明確に定義されておりません。一般に、ある目的を達しようとするプログラムは必然的に似る傾向にあることから、プログラムソースがよっぽど特徴的であること以外、その著作権というのは殆ど認められていないようです。
　      しかし、本サイトのプログラムは一部参照文献の内容を参考にさせて頂いている部分があります。それに関しまして掲載上の問題がある場合はご連絡を頂ければと思います。
		(2005.5.28 管理人 IKD)
	*/

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

void LuAssist::Utils::popAll(State L) {
	int32_t num = lua_gettop(L);
	lua_pop(L, num);
}

void LuAssist::Utils::popAllAndPushFalse(State L) {
	popAll(L);
	lua_pushboolean(L, false);
}

void LuAssist::Utils::popAllAndPushTrue(State L) {
	popAll(L);
	lua_pushboolean(L, true);
}

std::vector<LuAssist::BasicType> LuAssist::Utils::basicTypes(State L) {
	int32_t i = 1;
	std::vector<BasicType> types = {};

	while (!lua_isnone(L, i)) {
		if (lua_isnumber(L, i))
			types.emplace_back(BasicType::Number);
		else if (lua_isstring(L, i))
			types.emplace_back(BasicType::String);
		else if (lua_isnil(L, i))
			types.emplace_back(BasicType::Nil);
		else if (lua_isboolean(L, i))
			types.emplace_back(BasicType::Boolean);
		else if (lua_istable(L, i))
			types.emplace_back(BasicType::Table);
		else if (lua_isfunction(L, i))
			types.emplace_back(BasicType::Function);
		else if (lua_isuserdata(L, i))
			types.emplace_back(BasicType::UserData);
		else if (lua_isthread(L, i))
			types.emplace_back(BasicType::Thread);
		else
			assert(false);

		i++;
	}

	return types;
}

LuAssist::Type LuAssist::Utils::type(State L, const int32_t index) {
	if (lua_isinteger(L, index))
		return Type::Integer;
	if (lua_isnumber(L, index))
		return Type::Number;
	else if (lua_isstring(L, index))
		return Type::String;
	else if (lua_isnil(L, index))
		return Type::Nil;
	else if (lua_isboolean(L, index))
		return Type::Boolean;
	else if (lua_istable(L, index))
		return Type::Table;
	else if (lua_iscfunction(L, index))
		return Type::CFunction;
	else if (lua_isfunction(L, index))
		return Type::Function;
	else if (lua_islightuserdata(L, index))
		return Type::LightUserData;
	else if (lua_isuserdata(L, index))
		return Type::UserData;
	else if (lua_isthread(L, index))
		return Type::Thread;
	else
		return Type::None;
}

std::vector<LuAssist::Type> LuAssist::Utils::types(State L) {
	int32_t i = 1;
	std::vector<Type> types = {};

	while (!lua_isnone(L, i)) {
		types.emplace_back(type(L, i));

		assert(types.back() != Type::None);

		i++;
	}

	return types;
}

std::size_t LuAssist::Utils::numOfArgs(State L) {
	std::size_t i = 0;
	std::size_t count = 0;
	while (!lua_isnone(L, i)) {
		i++;
		count++;
	}
	return count;
}

const char* LuAssist::Utils::typeName(const Type type) noexcept {
	if (type == Type::Integer)
		return "Integer";
	else if (type == Type::Number)
		return "Number";
	else if (type == Type::String)
		return "String";
	else if (type == Type::Nil)
		return "Function";
	else if (type == Type::Boolean)
		return "Boolean";
	else if (type == Type::Table)
		return "Table";
	else if (type == Type::CFunction)
		return "CFunction";
	else if (type == Type::Function)
		return "Function";
	else if (type == Type::LightUserData)
		return "LightUserData";
	else if (type == Type::UserData)
		return "UserData";
	else if (type == Type::Thread)
		return "Thread";
	else
		return "None";
}

std::string LuAssist::Utils::getSrcCurrentLine(State L,const int32_t depth) {
	lua_Debug info;

	//depth err -> return 0 
	//else      -> return 1
	if (lua_getstack(L, depth, &info) == 0)
		return std::string();
	
	lua_getinfo(L, "Sl", &info);

	return std::format("[Source\"{}\". Line\"{}\"] ", info.short_src, info.currentline);
}

int32_t LuAssist::Utils::lineDefined(State L) {
	lua_Debug info;

	//depth err -> return 0 
	//else      -> return 1
	if (lua_getstack(L, 1, &info) == 0)
		return -1;

	lua_getinfo(L, "l", &info);

	return info.linedefined;
}

int32_t LuAssist::Utils::currentLine(State L) {
	lua_Debug info;

	//depth err -> return 0 
	//else      -> return 1
	if (lua_getstack(L, 1, &info) == 0)
		return -1;

	lua_getinfo(L, "l", &info);

	return info.currentline;
}

int32_t LuAssist::Utils::size(State L) {
	return lua_gettop(L);
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
