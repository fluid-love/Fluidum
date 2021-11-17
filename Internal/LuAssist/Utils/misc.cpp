#include "misc.h"

void LuAssist::Utils::printStack(State L) {
	/*�Q�l�Fhttp://marupeke296.com/LUA_No2_Begin.html�@*/
	/*
	�@�u���~�i�܂�؂��j����[�ǂ��ƃR���v�����K�˒����܂��Đ^�ɂ��肪�Ƃ��������܂��B
�@	    ���~����[�ǂ��ƃR�����Ɍf�ڂ���Ă���v���O�����\�[�X�́A�Q�[������肽���ƍl���Ă���F�l�Ɏ��R�Ɏg�p���Ē��������ƍl���Ă���܂��B�������Ȃ���A�o�������̌��������s���Ă͂���܂����A�Ǘ��l���C���t���Ȃ��X�y���~�X��_���I�o�O���܂܂�Ă��鎖������܂��B����āA���̓���̕ۏ�͒v�����˂܂��̂ŗ\�߂������������B���܂������Ȃ��ӏ���������܂�����A����A���𒸂������v���܂��B
�@      �{�T�C�g�̃\�[�X�̓I�[�v���\�[�X�ł��B����āA�R�s�[����ς͎��R�ɍs���Ē����Č��\�ł��B�g���r�A�Șb�ɂȂ�܂����A�v���O�����̒��쌠�ɂ��ẮA���쌠�@�̒��ł͂��܂薾�m�ɒ�`����Ă���܂���B��ʂɁA����ړI��B���悤�Ƃ���v���O�����͕K�R�I�Ɏ���X���ɂ��邱�Ƃ���A�v���O�����\�[�X������ۂǓ����I�ł��邱�ƈȊO�A���̒��쌠�Ƃ����͖̂w�ǔF�߂��Ă��Ȃ��悤�ł��B
�@      �������A�{�T�C�g�̃v���O�����͈ꕔ�Q�ƕ����̓��e���Q�l�ɂ����Ē����Ă��镔��������܂��B����Ɋւ��܂��Čf�ڏ�̖�肪����ꍇ�͂��A���𒸂���΂Ǝv���܂��B
		(2005.5.28 �Ǘ��l IKD)
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
//			lua_rawgeti(L, 2, 1);//(L,���Ԗڂɓǂݍ���table������,table�̉��Ԗڂ�ǂݍ��ނ�)
//
//			//pos���擾
//			for (uint16_t k = 1; k < 4; k++) {
//				lua_rawgeti(L, 3, k);
//
//				pos[k - 1] = (float)lua_tonumber(L, k + 3) / 10.0f;
//			}
//			lua_pop(L, 4);
//
//			//color���擾
//			lua_rawgeti(L, 2, 2);
//
//			for (uint16_t k = 1; k < 5; k++) {
//				lua_rawgeti(L, 3, k);
//
//				color[k - 1] = (float)lua_tonumber(L, k + 3) / 100.0f;
//			}
//			lua_pop(L, 6);
//
//			//�����
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
//	//2�����̂ŏ���
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
//	//size���m�ۂ���
//	auto itr = std::max_element(changedIndices.begin(), changedIndices.end());
//	if (vertices->size() < *itr)
//		vertices->resize(*itr);
//
//
//	for (uint32_t i = 0; i < changedIndices.size(); i++) {
//		if (lua_rawgeti(L, 1, changedIndices[i]) != LUA_TNIL) {
//			lua_rawgeti(L, 2, 1);//(L,���Ԗڂɓǂݍ���table������,table�̉��Ԗڂ�ǂݍ��ނ�)
//
//			//pos���擾
//			for (uint16_t k = 1; k < 4; k++) {
//				lua_rawgeti(L, 3, k);
//
//				pos[k - 1] = (float)lua_tonumber(L, k + 3) / 10.0f;
//			}
//			lua_pop(L, 4);
//
//			//color���擾
//			lua_rawgeti(L, 2, 2);
//
//			for (uint16_t k = 1; k < 5; k++) {
//				lua_rawgeti(L, 3, k);
//
//				color[k - 1] = (float)lua_tonumber(L, k + 3) / 100.0f;
//			}
//			lua_pop(L, 6);
//
//			//�����
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
//	//size���m�ۂ���
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
