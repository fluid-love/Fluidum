#pragma once

#include <iostream>


/*debug*/
//logを出力しない
//#define FVK_NO_LOG
 

//debug of debug
//apiからmanagerへのconnectionのチェック
//#define FVK_NO_CHECK_API_CONNECTION_TO_MANAGER
//
//
//#ifndef FVK_NO_LOG
//#ifndef NDEBUG
//#define FVKlog(text) std::cout << text << std::endl;
//#else
//#define FVKlog
//#endif
//#else
//#define FVKlog
//#endif

//
//#ifndef FVK_NO_CHECK_API_CONNECTION_TO_MANAGER
//
//#define FVKcheck_api_connection(type,...)\
//constexpr std::array<FvkType,GetDataSize<type>()> arr ={__VA_ARGS__};\
//static_assert(checkApiConnection<type,arr>() == true,"ERROR");\
//
//#define FVKcheck_api_connection_cmd(type,...)\
//constexpr std::array<FvkType,GetDataSizeCmd<type>()> arr ={__VA_ARGS__};\
//static_assert(checkApiConnectionCmd<type,arr>() == true,"ERROR");\
//
//#else
//#define FVKcheck_api_connection
//#endif




