#include "texteditor.h"
#include "../../Utils/include.h"

::TextEditor::LanguageDefinition FTE::getLuaLanguageDefinition() {
	static bool inited = false;
	static TextEditor::LanguageDefinition lua{};

	if (inited)
		return lua;

	constexpr const char* keywords[] = {
		"and", 
		"break",
		"do", 
		"", 
		"else", 
		"elseif",
		"end", 
		"false", 
		"for", 
		"function",
		"if",
		"in", 
		"local",
		"nil", 
		"not", 
		"or", 
		"repeat",
		"return",
		"then", 
		"true",
		"until",
		"while"
	};

	for (const auto x : keywords)
		lua.mKeywords.insert(x);

	constexpr const char* identifiers[] = {
		"assert", "collectgarbage", "dofile", "error", "getmetatable", "ipairs", "loadfile", "load", "loadstring",  "next",  "pairs",  "pcall",  "print",  "rawequal",  "rawlen",  "rawget",  "rawset",
		"select",  "setmetatable",  "tonumber",  "tostring",  "type",  "xpcall",  "_G",  "_VERSION","arshift", "band", "bnot", "bor", "bxor", "btest", "extract", "lrotate", "lshift", "replace",
		"rrotate", "rshift", "create", "resume", "running", "status", "wrap", "yield", "isyieldable", "debug","getuservalue", "gethook", "getinfo", "getlocal", "getregistry", "getmetatable",
		"getupvalue", "upvaluejoin", "upvalueid", "setuservalue", "sethook", "setlocal", "setmetatable", "setupvalue", "traceback", "close", "flush", "input", "lines", "open", "output", "popen",
		"read", "tmpfile", "type", "write", "close", "flush", "lines", "read", "seek", "setvbuf", "write", "__gc", "__tostring", "abs", "acos", "asin", "atan", "ceil", "cos", "deg", "exp", "tointeger",
		"floor", "fmod", "ult", "log", "max", "min", "modf", "rad", "random", "randomseed", "sin", "sqrt", "string", "tan", "type", "atan2", "cosh", "sinh", "tanh",
		"pow", "frexp", "ldexp", "log10", "pi", "huge", "maxinteger", "mininteger", "loadlib", "searchpath", "seeall", "preload", "cpath", "path", "searchers", "loaded", "module", "require", "clock",
		"date", "difftime", "execute", "exit", "getenv", "remove", "rename", "setlocale", "time", "tmpname", "byte", "char", "dump", "find", "format", "gmatch", "gsub", "len", "lower", "match", "rep",
		"reverse", "sub", "upper", "pack", "packsize", "unpack", "concat", "maxn", "insert", "pack", "unpack", "remove", "move", "sort", "offset", "codepoint", "char", "len", "codes", "charpattern",
		"coroutine", "table", "io", "os", "string", "utf8", "bit32", "math", "debug", "package"
	};

	for (const auto& x : identifiers) {
		TextEditor::Identifier id{};
		id.mDeclaration = "Built-In";
		lua.mIdentifiers.insert(std::make_pair(std::string(x), id));
	}

	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("L?\\\"(\\\\.|[^\\\"])*\\\"", TextEditor::PaletteIndex::String));
	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("\\\'[^\\\']*\\\'", TextEditor::PaletteIndex::String));
	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", TextEditor::PaletteIndex::Number));
	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("[+-]?[0-9]+[Uu]?[lL]?[lL]?", TextEditor::PaletteIndex::Number));
	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*", TextEditor::PaletteIndex::Identifier));
	lua.mTokenRegexStrings.push_back(std::make_pair<std::string, ::FTE::TextEditor::PaletteIndex>("[\\[\\]\\{\\}\\!\\%\\^\\&\\*\\(\\)\\-\\+\\=\\~\\|\\<\\>\\?\\/\\;\\,\\.]", TextEditor::PaletteIndex::Punctuation));

	lua.mCommentStart = "--[[";
	lua.mCommentEnd = "]]";
	lua.mSingleLineComment = "--";

	lua.mCaseSensitive = true;
	lua.mAutoIndentation = false;

	lua.mName = "Lua";

	inited = true;

	return lua;
}

::TextEditor::Palette FTE::getDarkPalette() {
	constexpr ::TextEditor::Palette p = { {
			0xff7f7f7f,	// Default
			0xffd69c56,	// Keyword	
			0xff00ff00,	// Number
			FU::ImGui::ConvertImVec4ToImU32(1.0f,1.0f,1.0f,1.0f),	// String
			0xff70a0e0, // Char literal
			0xffffffff, // Punctuation
			0xff408080,	// Preprocessor
			0xffaaaaaa, // Identifier
			0xff9bc64d, // Known identifier
			0xffc040a0, // Preproc identifier
			0xff206020, // Comment (single line)
			0xff406020, // Comment (multi line)
			FU::ImGui::ConvertImVec4ToImU32(0.02f,0.02f,0.02f,1.0f), // Background
			0xffe0e0e0, // Cursor
			0x80a06020, // Selection
			0x800020ff, // ErrorMarker
			FU::ImGui::ConvertImVec4ToImU32(0.4f,0.8f,0.8f,0.08f), // Breakpoint
			0xff707000, // Line number
			0x40000000, // Current line fill
			FU::ImGui::ConvertImVec4ToImU32(0.4f,0.4f,0.4f,0.08f), // Current line fill (inactive)
			0x40a0a0a0, // Current line edge
		} };
	return p;
}
