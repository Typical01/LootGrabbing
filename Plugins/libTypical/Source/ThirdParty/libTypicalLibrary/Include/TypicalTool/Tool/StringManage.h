#pragma once

//常用工具
#ifndef _STRINGMANAGE_H
#define _STRINGMANAGE_H


#include <TypicalTool/Tool/pch.h>



namespace TypicalTool {
	//字符处理---------------------------------------------------------------------------------------------------------------
	namespace StringManage
	{
		inline bool IsRunTimeError = false;

		//字符转换-------------------------------------------------------------------------------------------------------
		template<class T = bool>
		std::string UTF8ToWstring_UTF8(const std::string& U8Str) { return U8Str; };
		TYPICALTOOL_API std::wstring UTF8ToWstring(const std::string&);
#ifdef u8tow
#undef u8tow
#endif
#ifdef stow
#undef stow
#endif

#ifdef UTF8
#define u8tow UTF8ToWstring_UTF8
#define stow UTF8ToWstring_UTF8

#else

#ifdef UNICODE

#define u8tow UTF8ToWstring
#define stow UTF8ToWstring
#else

#define u8tow UTF8ToWstring_UTF8
#define stow UTF8ToWstring_UTF8
#endif
#endif

		TYPICALTOOL_API std::string WstringToUTF8(const std::wstring&);
#ifdef wtou8
#undef wtou8
#endif
#ifdef wtos
#undef wtos
#endif

#define wtou8 WstringToUTF8
#define wtos WstringToUTF8

		//编码转换---------------------------------------------------------------------------------------------------------

		//工具
		template<typename T = bool>
		std::vector<std::string> SplitString(const std::string& str, const std::string& SplitStr) {
			std::vector<std::string> lines;
			size_t start = 0;
			size_t end = str.find(SplitStr);

			while (end != std::string::npos) {
				lines.push_back(str.substr(start, end - start));
				start = end + 1;
				end = str.find(SplitStr, start);
			}

			// 添加最后一行（如果最后没有 SplitStr）
			lines.push_back(str.substr(start));

			return lines;
		}
		template<typename T = bool>
		std::vector<std::wstring> SplitString(const std::wstring& str, const std::wstring& SplitStr) {
			std::vector<std::wstring> lines;
			size_t start = 0;
			size_t end = str.find(SplitStr);

			while (end != std::wstring::npos) {
				lines.push_back(str.substr(start, end - start));
				start = end + 1;
				end = str.find(SplitStr, start);
			}

			// 添加最后一行（如果最后没有 SplitStr）
			lines.push_back(str.substr(start));

			return lines;
		}
	}
}


#endif