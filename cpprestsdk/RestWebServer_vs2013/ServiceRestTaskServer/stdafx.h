// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <afx.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include <string>
#include <vector>
#include <iostream>
using namespace std;

#import "msxml3.dll" //named_guids no_namespace
using namespace MSXML2;

#include <Windows.h>

#include <locale>
#include <ctime>

#include "cpprest/json.h"
#include "cpprest/http_listener.h"
#include "cpprest/uri.h"

#pragma warning (push)
#pragma warning (disable : 4457)
#pragma warning (pop)

#pragma comment(lib, "./Lib64/IdrAgent64.lib")
#pragma comment(lib, "./Lib64/BizClient64.lib")
// TODO: reference additional headers your program requires here
