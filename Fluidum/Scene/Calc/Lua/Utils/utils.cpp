#include "utils.h"

void FS::Lua::printStack(State L) {
	/*�Q�l�Fhttp://marupeke296.com/LUA_No2_Begin.html�@*/
	/*
	�@�u���~�i�܂�؂��j����[�ǂ��ƃR���v�����K�˒����܂��Đ^�ɂ��肪�Ƃ��������܂��B
�@	    ���~����[�ǂ��ƃR�����Ɍf�ڂ���Ă���v���O�����\�[�X�́A�Q�[������肽���ƍl���Ă���F�l�Ɏ��R�Ɏg�p���Ē��������ƍl���Ă���܂��B�������Ȃ���A�o�������̌��������s���Ă͂���܂����A�Ǘ��l���C���t���Ȃ��X�y���~�X��_���I�o�O���܂܂�Ă��鎖������܂��B����āA���̓���̕ۏ�͒v�����˂܂��̂ŗ\�߂������������B���܂������Ȃ��ӏ���������܂�����A����A���𒸂������v���܂��B
�@      �{�T�C�g�̃\�[�X�̓I�[�v���\�[�X�ł��B����āA�R�s�[����ς͎��R�ɍs���Ē����Č��\�ł��B�g���r�A�Șb�ɂȂ�܂����A�v���O�����̒��쌠�ɂ��ẮA���쌠�@�̒��ł͂��܂薾�m�ɒ�`����Ă���܂���B��ʂɁA����ړI��B���悤�Ƃ���v���O�����͕K�R�I�Ɏ���X���ɂ��邱�Ƃ���A�v���O�����\�[�X������ۂǓ����I�ł��邱�ƈȊO�A���̒��쌠�Ƃ����͖̂w�ǔF�߂��Ă��Ȃ��悤�ł��B
�@      �������A�{�T�C�g�̃v���O�����͈ꕔ�Q�ƕ����̓��e���Q�l�ɂ����Ē����Ă��镔��������܂��B����Ɋւ��܂��Čf�ڏ�̖�肪����ꍇ�͂��A���𒸂���΂Ǝv���܂��B
		(2005.5.28 �Ǘ��l IKD)
	*/

	// �X�^�b�N�����擾
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

	//�S�Ă̈���
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
//
//
