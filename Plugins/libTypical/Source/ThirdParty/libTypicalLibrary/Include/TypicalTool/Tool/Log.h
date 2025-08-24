#pragma once

#ifndef _LOG_H
#define _LOG_H


#include <TypicalTool/Tool/pch.h>
#include <TypicalTool/Tool/Tchar_Typical.h>
#include <TypicalTool/Tool/StringManage.h>



namespace TypicalTool
{
	using namespace StringManage;


#ifndef TYPICAL_DEFINE_ENUM_FLAG_OPERATORS
#define TYPICAL_DEFINE_ENUM_FLAG_OPERATORS(EnumName) \
using EnumName##_Underlying = std::underlying_type_t<EnumName>; \
constexpr EnumName operator~(EnumName a) noexcept { \
    return static_cast<EnumName>(~static_cast<EnumName##_Underlying>(a)); \
} \
constexpr EnumName operator|(EnumName a, EnumName b) noexcept { \
    return static_cast<EnumName>(static_cast<EnumName##_Underlying>(a) | static_cast<EnumName##_Underlying>(b)); \
} \
constexpr EnumName operator&(EnumName a, EnumName b) noexcept { \
    return static_cast<EnumName>(static_cast<EnumName##_Underlying>(a) & static_cast<EnumName##_Underlying>(b)); \
} \
constexpr EnumName operator^(EnumName a, EnumName b) noexcept { \
    return static_cast<EnumName>(static_cast<EnumName##_Underlying>(a) ^ static_cast<EnumName##_Underlying>(b)); \
} \
constexpr EnumName& operator|=(EnumName& a, EnumName b) noexcept { \
    return a = a | b; \
} \
constexpr EnumName& operator&=(EnumName& a, EnumName b) noexcept { \
    return a = a & b; \
} \
constexpr EnumName& operator^=(EnumName& a, EnumName b) noexcept { \
    return a = a ^ b; \
} \
constexpr void SetFlag(EnumName& flags, EnumName value) noexcept { \
    flags |= value; \
} \
constexpr void ClearFlag(EnumName& flags, EnumName value) noexcept { \
    flags &= ~value; \
} \
constexpr void ToggleFlag(EnumName& flags, EnumName value) noexcept { \
    flags ^= value; \
} \
constexpr bool HasFlag(EnumName flags, EnumName flag) noexcept { \
return (flags & flag) != EnumName{ 0 }; \
} \
constexpr bool HasAllFlag(EnumName flags, EnumName mask) noexcept { \
	return (flags & mask) == mask; \
}
#endif

		
	class Printf {
	private:
		Tstring FormatStr;

	public:
		// 使用一个占位符（%s）来替换格式化字符串中的参数
		template<typename... Args>
		Printf(const Tstring& format, Args... args)
		{
			std::vector<Tstring> placeholders = { format };
			FormatStr = format_impl(placeholders, args...);
		}

	private:
		// 基础模板：处理最后一个参数
		template<typename T = bool>
		Tstring format_impl(std::vector<Tstring>& placeholders) { return placeholders[0]; }
		// 递归模板：处理其他参数
		template<typename T, typename... Args>
		Tstring format_impl(std::vector<Tstring>& placeholders, T first, Args... args) 
		{
			Tstring formatted_string = placeholders[0];
			size_t pos = formatted_string.find(TEXT("%s")); // 查找第一个占位符

			if (pos != Tstring::npos) {
				Tostringstream oss;
				oss << first;  // 将第一个参数转换为字符串
				formatted_string.replace(pos, 2, oss.str()); // 替换占位符
			}
			placeholders[0] = formatted_string; // 递归替换剩余的占位符
			return format_impl(placeholders, args...);
		}

	public:
		// 获取格式化后的字符串
		template<typename T = bool>
		const Tchar* ToString() const { return FormatStr.c_str(); }
		
		// 自动转换
		template<typename T = bool>
		operator const Tchar* () const {
			return FormatStr.c_str();
		}
	};


	enum AnsiescColor {
		Reset = 0,		 // Color: null
		Red,		 // Color: RED
		Yellow,		 // Color: YELLOW
		Green		 // Color: GREEN
	};
	template<typename T = bool>
	const Tchar* AnsiescColorToStr(const AnsiescColor& _AnsiescColor)
	{
#ifndef _ANSIESC_CONSOLE_CHAR_COLOR
		return TEXT("");
#else
		switch (_AnsiescColor)
		{
		case AnsiescColor::Reset: {
			return TEXT("\033[0m");
		}
		case AnsiescColor::Red: {
			return TEXT("\033[31m");
		}
		case AnsiescColor::Yellow: {
			return TEXT("\033[33m");
		}
		case AnsiescColor::Green: {
			return TEXT("\033[32m");
		}
		default:
			return TEXT("\033[0m");
		}
#endif
	}
	template<typename T = bool>
	const Tchar* ToColor(const AnsiescColor& _Flag) {
		return AnsiescColorToStr(_Flag);
	}

	//日志等级标志
	enum class LogLevelFlags : uint32_t {
		None	 = 0,
		Error	 = 1 << 0,		 //错误: Error	 | TEXT:[ERROR]	  | Color:红色
		Warning  = 1 << 1,		 //警告: Warning	 | TEXT:[WARRING] | Color:黄色
		Info	 = 1 << 2,		 //信息: Info	 | TEXT:[INFO]	  | Color:绿色
		LineEnd  = 1 << 3,		 //结束: 不换行
		LineFeed = 1 << 4		 //换行: '\n'
	};
	TYPICAL_DEFINE_ENUM_FLAG_OPERATORS(LogLevelFlags)
	typedef LogLevelFlags LLF; //日志等级标志

	//日志配置标志
	enum class LogConfigFlags : uint32_t {
		None		= 0,
		Initialize	= 1 << 0,		 // Log: Initialize
		FileWrite	= 1 << 1,		 // Log: FileWrite
		ShowLog		= 1 << 2,		 // Log: ShowLog
		ShowTime	= 1 << 3,		 // Log: ShowTime
	};
	TYPICAL_DEFINE_ENUM_FLAG_OPERATORS(LogConfigFlags)
	typedef LogConfigFlags LCF; //日志配置标志

	//日志标志
	enum class LogFlags : uint32_t {
		None		= 0,
		Gui			= 1 << 0,		 // Project Flag: Gui
		Console		= 1 << 1,		 // Project Flag: Console
		Debug		= 1 << 2,		 // Project Flag: Debug
		Release		= 1 << 3		 // Project Flag: Release
	};
	TYPICAL_DEFINE_ENUM_FLAG_OPERATORS(LogFlags)
	typedef LogFlags LF; //日志标志

	template<typename T = bool>
	AnsiescColor LogLevelFlagsToAnsiescColor(const LogLevelFlags& _LogLevelFlags)
	{
		if (HasFlag(_LogLevelFlags, LogLevelFlags::Error)) {
			return AnsiescColor::Red;
		}
		if (HasFlag(_LogLevelFlags, LogLevelFlags::Warning)) {
			return AnsiescColor::Yellow;
		}
		if (HasFlag(_LogLevelFlags, LogLevelFlags::Info)) {
			return AnsiescColor::Green;
		}
		return AnsiescColor::Reset;
	}
	template<typename T = bool>
	const Tchar* LogLevelFlagsToString(const LogLevelFlags& _LogLevelFlags, bool bGui)
	{
		if (HasFlag(_LogLevelFlags, LogLevelFlags::Error)) {
			return Log_Error;
		}
		if (HasFlag(_LogLevelFlags, LogLevelFlags::Warning)) {
			return Log_Warning;
		}
		if (HasFlag(_LogLevelFlags, LogLevelFlags::Info)) {
			return Log_Info;
		}
		return bGui ? Log_Text : Log_Tab;
	}
	template<typename T = bool>
	const Tchar* ToString(const LogLevelFlags& _LogLevelFlags, bool bGui = false) {
		return LogLevelFlagsToString(_LogLevelFlags, bGui);
	}
	template<typename T = bool>
	const Tchar* ToColor(const LogLevelFlags& _LogLevelFlags) {
		return ToColor(LogLevelFlagsToAnsiescColor(_LogLevelFlags));
	}


#ifndef MB_OK
#define MB_OK 0
#endif
#ifndef MB_ICONSTOP
#define MB_ICONSTOP 0
#endif
#ifndef MB_ICONWARNING
#define MB_ICONWARNING 0
#endif


	class LogStringBuffer {
	public:
		Tstring AnsiEsc_Color;
		Tstring TimeStr;
		Tstring Text;
		Tstring AnsiEsc_Reset;

	public:
		LogStringBuffer()
			: AnsiEsc_Color(TEXT("")), TimeStr(TEXT("")), Text(TEXT("")), AnsiEsc_Reset(TEXT("")) { }

	public:
		template<class T = bool>
		void SetString(const Tstring& _AnsiEsc_Color, const Tstring& _Text, const Tstring& _AnsiEsc_Reset)
		{
			AnsiEsc_Color = _AnsiEsc_Color;
			TimeStr = TEXT("");
			Text = _Text;
			AnsiEsc_Reset = _AnsiEsc_Reset;
		}
		template<class T = bool>
		void SetString(const Tstring& _AnsiEsc_Color, const Tstring& _TimeStr, const Tstring& _Text, const Tstring& _AnsiEsc_Reset)
		{
			AnsiEsc_Color = _AnsiEsc_Color;
			TimeStr = _TimeStr;
			Text = _Text;
			AnsiEsc_Reset = _AnsiEsc_Reset;
		}
		template<class T = bool>
		void SetTimeString(const Tstring& _TimeStr)
		{
			TimeStr = _TimeStr;
		}
	};
	template<class T = bool>
	Tostream& operator<<(Tostream& Ostream, const LogStringBuffer& LogStringBuffer) {
		return Ostream << LogStringBuffer.AnsiEsc_Color << LogStringBuffer.TimeStr
			<< LogStringBuffer.Text << LogStringBuffer.AnsiEsc_Reset;
	}


#ifndef UNICODE
#define LogOutOstream std::cout
#define LogErrOstream std::cerr
#else
#define LogOutOstream std::wcout
#define LogErrOstream std::wcerr
#endif


	class TYPICALTOOL_API LogFile {
	public:
		HWND hConsole = nullptr; //控制台句柄

		std::shared_ptr<Tofstream> LogFileStreamOut;			//日志文件流 输出
		std::mutex MutexLogFileStreamOut;
		std::queue<Tstring> LogFileWriteQueue;					//日志 WriteConfigFile队列
		std::thread LogFileProcessing;						//日志文件处理: 主要是输出到[./Log/时间_程序名.txt]文件
		std::atomic<bool> bIsLogFileWriteThreadStop = false;	//是否 停止日志文件写入线程
	};

	/* 
	* 不需要控制台颜色显示: #undef _ANSIESC_CONSOLE_CHAR_COLOR
	* ConfigFlag & LogConfigFlags::Debug: 需要显示指定 | 显示 Log中的 ConfigFlag & LogConfigFlags::Debug消息
	* WIN32APP:	对于 Win32app 显示指定控制台分离
	* English: 标准库控制台的地区支持, 默认为中文
	*/
	class TYPICALTOOL_API Log {
	private:
		static std::shared_ptr<LogFile> LogFileInstance;

		//static HWND hConsole; //控制台句柄

		//static std::shared_ptr<Tofstream> LogFileStreamOut;			//日志文件流 输出
		//static std::mutex MutexLogFileStreamOut;
		//static std::queue<Tstring> LogFileWriteQueue;					//日志 WriteConfigFile队列
		//static std::thread LogFileProcessing;						//日志文件处理: 主要是输出到[./Log/时间_程序名.txt]文件
		//static std::atomic<bool> bIsLogFileWriteThreadStop;	//是否 停止日志文件写入线程

	private:
		static LogConfigFlags ConfigFlag;			//配置: 命令行界面/图形用户界面 & Debug/Release
		static LogLevelFlags ConfigLevelFlag;		//通用: [写入]日志的级别
		LogFlags LogFlag;							//LogFlag: 初始化/日志显示/日志输出时间
		static LogFlags LogFlagVersion;							//LogFlag: Debug/Release

	public:
		Log(const LogFlags& _LogFlag = LogFlags::Console)
		{
			LogFlag = _LogFlag;

			Initialize();
		}

		~Log()
		{
			StopLogWrite(); //当程序结束时, 退出 日志文件写入线程

			if (!LogFileInstance) {
				return;
			}
			if (LogFileInstance->LogFileProcessing.joinable()) {
				LogFileInstance->LogFileProcessing.join();
			}
		}

	public:
		/* 控制台初始化状态: false(需要初始化), true(跳过初始化)
		*/
		template<typename T = bool>
		void Initialize() {

			//控制台初始化
			if (!HasFlag(ConfigFlag, LogConfigFlags::Initialize)) {
#ifdef WIN32APP
#ifdef _WINDOWS
				//Windows 控制台编码修改: UTF-8
				SetConsoleOutputCP(CP_UTF8);
				SetConsoleCP(CP_UTF8);
#ifndef _CONSOLE
				//分离控制台
				if (FreeConsole() == 0) {
					LogMessageBox(TEXT("Log::Initialize: 分离控制台失败!"), LogLevelFlags::Error, MB_ICONSTOP);
					LogMessageBox(Printf(TEXT("Log::Initialize: 错误代码: [%s]"), ToStr(GetLastError())), LogLevelFlags::Error, MB_ICONSTOP);
				}
				//分配控制台: 当不是控制台程序时
				if (AllocConsole() == 0) {
					LogMessageBox(TEXT("Log::Initialize: 分配控制台失败!"), LogLevelFlags::Error, MB_ICONSTOP);
					LogMessageBox(Printf(TEXT("Log::Initialize: 错误代码: [%s]"), ToStr(GetLastError())), LogLevelFlags::Error, MB_ICONSTOP);
				}

				// 获取标准输出流的句柄
				FILE* FilePtr;
				// 重定向标准输出流
				errno_t err = freopen_s(&FilePtr, "CONOUT$", "w", stdout);
				if (err != 0) {
					LogMessageBox(TEXT("Log::Initialize: 重定向标准输出流失败!"), LogLevelFlags::Error, MB_ICONSTOP);
					LogMessageBox(Printf(TEXT("Log::Initialize: 错误代码: [%s]"), err).ToString(), LogLevelFlags::Error, MB_ICONSTOP);
					return;
				}
				// 重定向标准错误流
				err = freopen_s(&FilePtr, "CONOUT$", "w", stderr);
				if (err != 0) {
					LogMessageBox(TEXT("Log::Initialize: : 重定向标准错误流失败!"), LogLevelFlags::Error, MB_ICONSTOP);
					LogMessageBox(Printf(TEXT("Log::Initialize: 错误代码: [%s]"), err).ToString(), LogLevelFlags::Error, MB_ICONSTOP);
					return;
				}
				err = freopen_s(&FilePtr, "CONIN$", "r", stdin);
				if (err != 0) {
					LogMessageBox(TEXT("Log::Initialize: : 重定向标准输入流失败!"), LogLevelFlags::Error, MB_ICONSTOP);
					LogMessageBox(Printf(TEXT("Log::Initialize: 错误代码: [%s]"), err).ToString(), LogLevelFlags::Error, MB_ICONSTOP);
					return;
				}
				// 设置标准句柄
				HANDLE hConOut = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_WRITE | FILE_SHARE_READ,
					NULL, OPEN_EXISTING, 0, NULL);
				SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
				SetStdHandle(STD_ERROR_HANDLE, hConOut);

				HANDLE hConIn = CreateFile(TEXT("CONIN$"), GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL, OPEN_EXISTING, 0, NULL);
				SetStdHandle(STD_INPUT_HANDLE, hConIn);
#endif

				// 获取控制台窗口的句柄
				if (LogFileInstance != nullptr) {
					LogFileInstance->hConsole = GetConsoleWindow();
				}
				EnableAnsiEscape();

				//检查标准输出流
				if (!LogOutOstream.good() || !LogErrOstream.good()) {
					LogMessageBox(TEXT("Log::Initialize: LogOutOstream/LogErrOstream 无效!"), LogLevelFlags::Error, MB_ICONSTOP);
				}
				else {
#ifdef English
					std::locale old_locale = std::locale::global(std::locale("en_US.UTF-8"));
#else
					std::locale old_locale = std::locale::global(std::locale("zh_CN.UTF-8"));
#endif
					std::locale new_locale = std::locale();  // 获取当前全局 locale
					Log_Out(LogErrOstream, TEXT(""), LogLevelFlags::Error);
#ifdef English
					if (new_locale.name() == "en_US.UTF-8") {
						Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: en_US.UTF-8 Successful!\n"), ToString(LogLevelFlags::Info), true), LogLevelFlags::Error);
					}
					else {
						Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: en_US.UTF-8 Fail!\n"), ToString(LogLevelFlags::Info), true), LogLevelFlags::Error);
					}
#else
					if (new_locale.name() == "zh_CN.UTF-8") {
						Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: zh_CN.UTF-8 Successful!\n"), ToString(LogLevelFlags::Info), true), LogLevelFlags::Error);
					}
					else {
						Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: zh_CN.UTF-8 Fail!\n"), ToString(LogLevelFlags::Info), true), LogLevelFlags::Error);
					}
#endif

					// 设置 cout/cerr 使用全局地区
					LogOutOstream.imbue(std::locale());
					LogErrOstream.imbue(std::locale());
				}
#endif
#else
				//检查标准输出流
				if (!LogOutOstream.good()) {
					LogMessageBox(TEXT("Log::Initialize: LogOutOstream 无效!"), LogLevelFlags::Error, MB_ICONSTOP);
					throw Truntime_error(TEXT("Log::Initialize: LogOutOstream Not good!"));
				}
				if (!LogErrOstream.good()) {
					LogMessageBox(TEXT("Log::Initialize: LogErrOstream 无效!"), LogLevelFlags::Error, MB_ICONSTOP);
					throw Truntime_error(TEXT("Log::Initialize: LogErrOstream Not good!"));
				}

#ifdef English
				std::locale old_locale = std::locale::global(std::locale("en_US.UTF-8"));
#else
				std::locale old_locale = std::locale::global(std::locale("zh_CN.UTF-8"));
#endif
				std::locale new_locale = std::locale();  // 获取当前全局 locale
#ifdef English
				if (new_locale.name() == "en_US.UTF-8") {
					Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: en_US.UTF-8 Successful!\n"), ToString(LogLevelFlags::Info)), LogLevelFlags::Error);
				}
				else {
					Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: en_US.UTF-8 Fail!\n"), ToString(LogLevelFlags::Info)), LogLevelFlags::Error);
				}
#else
				if (new_locale.name() == "zh_CN.UTF-8") {
					Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: zh_CN.UTF-8 Successful!\n"), ToString(LogLevelFlags::Info)), LogLevelFlags::Error);
				}
				else {
					Log_Out(LogErrOstream, Printf(TEXT("%sLog::Initialize: zh_CN.UTF-8 Fail!\n"), ToString(LogLevelFlags::Info)), LogLevelFlags::Error);
				}
#endif

				// 设置 cout/cerr 使用全局地区
				//LogOutOstream.imbue(std::locale(""));
				//LogErrOstream.imbue(std::locale(""));
				LogOutOstream.imbue(old_locale);
				LogErrOstream.imbue(old_locale);
#endif

				//完成初始化
				ConfigFlag |= LogConfigFlags::Initialize;
			}
		}

	public:
		// 日志文件有效时: 总是返回 true
		// 输出流 && 日志文件都无效: 返回 false
		template<typename T = bool>
		static bool Log_Out(Tostream& _Ostream, const Tchar* _str,
			const LogLevelFlags& _LogOutputLevel = LogLevelFlags::Info)
		{
			// 输出流无效
			if (!_Ostream.good()) {
				if (!LogFileInstance) { //日志文件无效
					return false;
				}
			} 

			// 日志文件正常: 不打印, 但保存到日志文件
			LogStringBuffer tempLogBuffer;
			tempLogBuffer.SetString(ToColor(_LogOutputLevel), _str, ToColor(AnsiescColor::Reset));
			if (HasFlag(ConfigFlag, LogConfigFlags::ShowTime)) {
				tempLogBuffer.SetTimeString(Log::GetFormatTime());
			}

			_Ostream << tempLogBuffer;

			//WriteConfigFile log日志
			if (!HasFlag(ConfigFlag, LogConfigFlags::FileWrite)) { return true; }

			if (HasFlag(ConfigLevelFlag, LogLevelFlags::Error) &&
				HasFlag(_LogOutputLevel, LogLevelFlags::Error)) { //级别: 错误
				LogWrite(tempLogBuffer);
			}
			else if (HasFlag(ConfigLevelFlag, LogLevelFlags::Warning) &&
				HasAllFlag(_LogOutputLevel, LogLevelFlags::Error | LogLevelFlags::Warning)) { //级别: 警告
				LogWrite(tempLogBuffer);
			}
			else if (HasFlag(ConfigLevelFlag, LogLevelFlags::Info) &&
				HasAllFlag(_LogOutputLevel, LogLevelFlags::Error | LogLevelFlags::Warning | LogLevelFlags::Info)) { //级别: 提示
				LogWrite(tempLogBuffer);
			}
			else { //级别: 一般
				LogWrite(tempLogBuffer);
			}

			if (!_Ostream.good()) {
				return false;
			}
			return true;
		}

	private:
		// _WINDOWS || _CONSOLE
		template<typename T = bool>
		bool Logs_ustr(const Tchar* _Text, const LogLevelFlags& _Lm) {
			if (HasFlag(_Lm, LogLevelFlags::Error)) {
				if (HasFlag(LogFlag, LogFlags::Console)) {
					return Log_Out(LogErrOstream, Printf(TEXT("%s%s%s"), ToString(LogLevelFlags::Error), _Text, Log_LineFeed), LogLevelFlags::Error);
				}
				else if (HasFlag(LogFlag, LogFlags::Gui)) {
					LogMessageBox(_Text, LogLevelFlags::Error, MB_ICONSTOP);
					return true;
				}
			}
			else if (HasFlag(_Lm, LogLevelFlags::Warning)) {
				if (HasFlag(LogFlag, LogFlags::Console)) {
					return Log_Out(LogOutOstream, Printf(TEXT("%s%s%s"), ToString(LogLevelFlags::Warning), _Text, Log_LineFeed), LogLevelFlags::Warning);
				}
				else if (HasFlag(LogFlag, LogFlags::Gui)) {
					LogMessageBox(_Text, LogLevelFlags::Warning, MB_ICONWARNING);
					return true;
				}
			}
			else if (HasFlag(_Lm, LogLevelFlags::Info)) {
				if (HasFlag(LogFlag, LogFlags::Console)) {
					return Log_Out(LogOutOstream, Printf(TEXT("%s%s%s"), ToString(LogLevelFlags::Info), _Text, Log_LineFeed), LogLevelFlags::Info);
				}
				else if (HasFlag(LogFlag, LogFlags::Gui)) {
					LogMessageBox(_Text, LogLevelFlags::Info, MB_OK);
					return true;
				}
			}
			else if (HasFlag(_Lm, LogLevelFlags::LineFeed)) {
				if (HasFlag(LogFlag, LogFlags::Console)) {
					return Log_Out(LogOutOstream, Printf(TEXT("%s%s%s"), ToString(LogLevelFlags::LineFeed), _Text, Log_LineFeed), LogLevelFlags::LineFeed);
				}
				else if (HasFlag(LogFlag, LogFlags::Gui)) {
					LogMessageBox(_Text, LogLevelFlags::LineFeed, MB_OK);
					return true;
				}
			}
			else if (HasFlag(_Lm, LogLevelFlags::LineEnd)) {
				if (HasFlag(LogFlag, LogFlags::Console)) {
					return Log_Out(LogOutOstream, Printf(TEXT("%s%s"), ToString(LogLevelFlags::LineEnd), _Text), LogLevelFlags::LineEnd);
				}
				else if (HasFlag(LogFlag, LogFlags::Gui)) {
					LogMessageBox(_Text, LogLevelFlags::LineEnd, MB_OK);
					return true;
				}
			}
			else if (HasFlag(_Lm, LogLevelFlags::None)) { return true; }
			return true;
		}
		bool Logs_lgm()
		{
			if (HasFlag(LogFlag, LogFlags::Console)) {
				return Log_Out(LogOutOstream, Log_LineFeed, LogLevelFlags::LineFeed);
			}
			return true;
		}


	public:

		//显示/隐藏 Log消息
		template<class T = bool>
		bool operator()(const Tchar* _Text, const LogLevelFlags& _Lm = LogLevelFlags::Error)
		{
			if (!HasFlag(ConfigFlag, LogConfigFlags::ShowLog)) { return true; }

			if (HasFlag(LogFlag, LogFlags::Debug) && HasFlag(LogFlagVersion, LogFlags::Debug)) { // ConfigFlag & LogConfigFlags::Debug
				return Logs_ustr(_Text, _Lm);
			}
			else if (HasFlag(LogFlag, LogFlags::Release)) { // ConfigFlag & LogConfigFlags::Release
				return Logs_ustr(_Text, _Lm);
			}

			return true;
		}
		template<class T = bool>
		bool operator()(const LogLevelFlags& _Lm, const Tchar* _Text)
		{
			if (!HasFlag(ConfigFlag, LogConfigFlags::ShowLog)) { return true; }

			if (HasFlag(LogFlag, LogFlags::Debug) && HasFlag(LogFlagVersion, LogFlags::Debug)) { // ConfigFlag & LogConfigFlags::Debug
				return Logs_ustr(_Text, _Lm);
			}
			else if (HasFlag(LogFlag, LogFlags::Release)) { // ConfigFlag & LogConfigFlags::Release
				return Logs_ustr(_Text, _Lm);
			}

			return true;
		}
		template<class T = bool>
		bool Out(const LogLevelFlags& _Lm, const Tchar* _Text)
		{
			if (!HasFlag(ConfigFlag, LogConfigFlags::ShowLog)) { return true; }

			if (HasFlag(LogFlag, LogFlags::Debug) && HasFlag(LogFlagVersion, LogFlags::Debug)) { // ConfigFlag & LogConfigFlags::Debug
				return Logs_ustr(_Text, _Lm);
			}
			else if (HasFlag(LogFlag, LogFlags::Release)) { // ConfigFlag & LogConfigFlags::Release
				return Logs_ustr(_Text, _Lm);
			}

			return true;
		}
		/*
		* 输出 _LineNumber 个换行符
		*/
		bool operator()(int64_t _LineNumber = 0)
		{
			if (!HasFlag(ConfigFlag, LogConfigFlags::ShowLog)) { return true; }

			if (HasFlag(LogFlag, LogFlags::Debug) && HasFlag(LogFlagVersion, LogFlags::Debug)) { // ConfigFlag & LogConfigFlags::Debug
				for (auto i = 0; i < _LineNumber; i++) {
					if (!Logs_lgm()) return false;
				}
			}
			else if (HasFlag(LogFlag, LogFlags::Release)) { // ConfigFlag & LogConfigFlags::Release
				for (auto i = 0; i < _LineNumber; i++) {
					if (!Logs_lgm()) return false;
				}
			}

			return true;
		}
		bool Out(int64_t _LineNumber = 0)
		{
			if (!HasFlag(ConfigFlag, LogConfigFlags::ShowLog)) { return true; }

			if (HasFlag(LogFlag, LogFlags::Debug) && HasFlag(LogFlagVersion, LogFlags::Debug)) { // ConfigFlag & LogConfigFlags::Debug
				for (auto i = 0; i < _LineNumber; i++) {
					if (!Logs_lgm()) return false;
				}
			}
			else if (HasFlag(LogFlag, LogFlags::Release)) { // ConfigFlag & LogConfigFlags::Release
				for (auto i = 0; i < _LineNumber; i++) {
					if (!Logs_lgm()) return false;
				}
			}

			return true;
		}

	public:
		//设置 日志显示
		static void SetShowLog(bool _ShowLog) { _ShowLog ? SetFlag(ConfigFlag, LogConfigFlags::ShowLog) : ClearFlag(ConfigFlag, LogConfigFlags::ShowLog); }
		//设置 时间显示
		static void SetShowTime(bool _ShowTime) { _ShowTime ? SetFlag(ConfigFlag, LogConfigFlags::ShowTime) : ClearFlag(ConfigFlag, LogConfigFlags::ShowTime); }

		//设置 控制台显示: _WINDOWS(Win32 API)
		template<typename T = bool>
		static void SetShowConsole(bool _ShowConsole)
		{
#ifdef _WINDOWS
			if (LogFileInstance == nullptr) return;

			//显示/隐藏 窗口
			if (_ShowConsole) {
				ShowWindow(LogFileInstance->hConsole, SW_SHOWDEFAULT);
			}
			else {
				ShowWindow(LogFileInstance->hConsole, SW_HIDE);
			}
#else
			if (HasFlag(LogFlagVersion, LogFlags::Debug)) {
				Log_Out(LogErrOstream,
					Printf(TEXT("%s%s"), ToString(LogLevelFlags::Error), TEXT("%sLog::SetShowConsole: 显示控制台: 无效(#ifndef _WINDOWS)\n")), LogLevelFlags::Error);
			}
#endif
		}
		//设置 ConfigFlag & LogConfigFlags::Debug 模式
		static void SetDebug(bool _bDebug)
		{
			if (_bDebug) {
				SetFlag(LogFlagVersion, LogFlags::Debug);
				ClearFlag(LogFlagVersion, LogFlags::Release);
			}
			else {
				ClearFlag(LogFlagVersion, LogFlags::Debug);
				SetFlag(LogFlagVersion, LogFlags::Release);
			}
		}
		//设置 日志文件写入
		static void SetLogFileWrite(bool _bIsLogFileWrite) { _bIsLogFileWrite ? SetFlag(ConfigFlag, LogConfigFlags::FileWrite) : ClearFlag(ConfigFlag, LogConfigFlags::FileWrite); }

		template<class T = bool>
		//设置 日志写入等级
		static void SetLogWriteLevel(const LogLevelFlags& LogWriteLevel = LogLevelFlags::Error)
		{
			//输出所有级别
			ClearFlag(ConfigLevelFlag, LogLevelFlags::Error | LogLevelFlags::Warning | LogLevelFlags::Info | LogLevelFlags::LineFeed | LogLevelFlags::LineEnd);
			SetFlag(ConfigLevelFlag, LogWriteLevel);

			if (HasFlag(ConfigLevelFlag, LogLevelFlags::Error)) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::SetLogWriteLevel: 文件输出级别[Error]\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
			}
			else if (HasFlag(ConfigLevelFlag, LogLevelFlags::Warning)) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::SetLogWriteLevel: 文件输出级别[Warning]\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Warning);
			}
			else if (HasFlag(ConfigLevelFlag, LogLevelFlags::Info)) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::SetLogWriteLevel: 文件输出级别[Info]\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Info);
			}
			else {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::SetLogWriteLevel: 文件输出级别[All]\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::LineFeed);
			}
		}

	public:

		/// <summary>
		/// 设置 日志文件写入的配置
		/// </summary>
		/// <param name="_LogFileName">: 日志文件名</param>
		/// <param name="_LogFolderPath">: 日志文件所在文件夹路径</param>
		/// <param name="bSingleLogFile">: 单日志文件</param>
		template<typename T = bool>
		static bool SetLogFileWriteConfig(const Tstring& _LogFileName, const Tstring& _LogFolderPath, bool _bSingleLogFile = true)
		{
			//获取 当前路径/Log/文件名_Log.txt 
			//创建文件夹 Log
			Tstring LogFilePath = _LogFileName + TEXT("_Log.log");
			if (!_bSingleLogFile) {
				Tstring Log_FolderName = _LogFolderPath;
				if (Log_FolderName == TEXT("")) {
					Log_FolderName = TEXT("Log");
				}

				if (std::filesystem::exists(Log_FolderName)) { //路径存在
					if (std::filesystem::is_directory(Log_FolderName)) { // 是目录
						//Log文件名: 格式化日期时间(年-月-日_时-分-秒) + .txt
						Tstring LogFileName_Time = Log::GetFormatTime(TEXT("%Y-%m-%d_%H-%M-%S_"), TEXT(""), TEXT(""));
						// ./Log/时间.txt  ||  ./时间.txt
						LogFilePath = Printf(TEXT("%s%s%s%s"), Log_FolderName, PATH_SLASH, LogFileName_Time, LogFilePath);
					}
					else { // 不是目录
					}
				}
				else { //目录不存在
					std::filesystem::create_directories(Log_FolderName); //创建目录
					//Log文件名: 格式化日期时间(年-月-日_时-分-秒) + .txt
					Tstring LogFileName_Time = Log::GetFormatTime(TEXT("%Y-%m-%d_%H-%M-%S_"), TEXT(""), TEXT(""));
					//Log/时间.txt  ||  ./时间.txt
					LogFilePath = Printf(TEXT("%s%s%s%s"), Log_FolderName, PATH_SLASH, LogFileName_Time, LogFilePath);
				}
			}
			else {
				Tstring Log_FolderName = _LogFolderPath;
				if (Log_FolderName == TEXT("")) {
					Log_FolderName = TEXT("Log");
				}

				if (!std::filesystem::exists(Log_FolderName)) { //路径不存在
					std::filesystem::create_directories(Log_FolderName); //创建目录
				}

				if (std::filesystem::is_directory(Log_FolderName)) { // 是目录
					LogFilePath = Printf(TEXT("%s%s%s"), Log_FolderName, PATH_SLASH, LogFilePath);
				}
			}
			Log_Out(LogErrOstream, Printf(TEXT("%sLog::SetLogFileWriteConfig: 日志输出文件名[%s]\n"), ToString(LogLevelFlags::Info), LogFilePath), LogLevelFlags::Error);

			//打开文件
			return CreateLogFileManageThread(LogFilePath);
		}

		// 日志写入相关
	private:
		template<typename T = bool>
		static void StopLogWrite()
		{
			if (!LogFileInstance) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::StopLogWrite: LogFileInstance无效!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
				return;
			}

			if (!LogFileInstance->bIsLogFileWriteThreadStop.load()) {
				LogFileInstance->bIsLogFileWriteThreadStop.store(true); //退出 条件

				if (LogFileInstance->LogFileProcessing.joinable()) {
					LogFileInstance->LogFileProcessing.join(); //主线程等待 后台退出
				}

				if (LogFileInstance->LogFileStreamOut && LogFileInstance->LogFileStreamOut->is_open()) {
					LogFileInstance->LogFileStreamOut->close();
				}
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::StopLogWrite: 日志写入完成! 程序退出...\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
				//Time::wait_s<time::ms>(10); //主线程等待 后台退出
			}
		}

		template<typename T = bool>
		static void LogWrite(const LogStringBuffer& _LogBuffer)
		{
			{
				std::lock_guard<std::mutex> QueueGuard(LogFileInstance->MutexLogFileStreamOut);

				if (LogFileInstance == nullptr) {
					Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWrite: LogFileInstance无效!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
					return;
				}
				LogFileInstance->LogFileWriteQueue.push(_LogBuffer.TimeStr);
				LogFileInstance->LogFileWriteQueue.push(_LogBuffer.Text);
			}
		}
		template<typename T = bool>
		static bool CreateLogFileManageThread(const Tstring& _LogFilePath)
		{
			if (LogFileInstance == nullptr) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::CreateLogFileManageThread: LogFileInstance无效!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
				return false;
			}

			LogFileInstance->LogFileStreamOut = std::make_shared<Tofstream>(_LogFilePath, std::ios::out);

			if (!LogFileInstance->LogFileStreamOut->is_open()) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::CreateLogFileManageThread: [%s]打开文件失败!\n"), ToString(LogLevelFlags::Error), _LogFilePath), LogLevelFlags::Error);
				return false;
			}
			Log_Out(LogErrOstream, Printf(TEXT("%sLog::CreateLogFileManageThread: [%s]打开文件成功!\n"), ToString(LogLevelFlags::Info), _LogFilePath), LogLevelFlags::Error);

			//初始化: 日志WriteConfigFile线程
			std::thread LogWriteToFileThread([=]() {
				LogWriteToFile(_LogFilePath, LogFileInstance);
				});
			LogFileInstance->LogFileProcessing = std::move(LogWriteToFileThread);
			Log_Out(LogErrOstream, Printf(TEXT("%sLog::CreateLogFileManageThread: 日志写入线程 启动!\n"), ToString(LogLevelFlags::Info)), LogLevelFlags::Error);

			return true;
		}
				
		template<typename T = bool>
		static void LogWriteToFile(const Tstring& _LogFilePath, std::shared_ptr<TypicalTool::LogFile> _LogFileInstance)
		{
			try {
				// 不退出, 且队列不为空
				while (1) {
					static bool bSleep = false;
					if (bSleep) {
						Log::Sleep(50, false);
						bSleep = false;
					}

					{
						std::lock_guard<std::mutex> QueueGuard(_LogFileInstance->MutexLogFileStreamOut);

						if (!_LogFileInstance) {
							Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: LogFileInstance 无效!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
							Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: 日志写入线程: 异常[%s]!\n"), ToString(LogLevelFlags::Warning), TEXT("日志文件实例无效!")), LogLevelFlags::Error);
							return;
						}

						//写入日志
						if (!_LogFileInstance->LogFileWriteQueue.empty()) {
							if (!_LogFileInstance->LogFileStreamOut->good()) {
								Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: 文件流状态异常!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);

								std::ios::iostate state = _LogFileInstance->LogFileStreamOut->rdstate();
								if (state & std::ios::eofbit)
									Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: Reached end of file!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
								if (state & std::ios::failbit)
									Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: Logical operation failed!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
								if (state & std::ios::badbit)
									Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: Critical I/O error!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
							}

							if (!_LogFileInstance->LogFileStreamOut->is_open()) { //再次打开文件流
								_LogFileInstance->LogFileStreamOut->open(_LogFilePath, std::ios::app);

								if (!*_LogFileInstance->LogFileStreamOut) {
									Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: 无法重新打开日志文件!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
								}
								else {
									*_LogFileInstance->LogFileStreamOut << _LogFileInstance->LogFileWriteQueue.front();
									_LogFileInstance->LogFileWriteQueue.pop();
								}
							}
							else {
								*_LogFileInstance->LogFileStreamOut << _LogFileInstance->LogFileWriteQueue.front();
								_LogFileInstance->LogFileWriteQueue.pop();
							}
						}
						else { //没有剩余日志写入
							if (_LogFileInstance->bIsLogFileWriteThreadStop.load()) { //停止写入线程
								if (_LogFileInstance->LogFileStreamOut->is_open()) {
									_LogFileInstance->LogFileStreamOut->close();
								}
								break;
							}
							else { //休眠, 减少cpu占用
								if (_LogFileInstance->LogFileStreamOut->is_open()) {
									_LogFileInstance->LogFileStreamOut->close();
								}
								bSleep = true;
							}
						}
					}
				}
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: 日志写入线程: 退出!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
			}
			catch (const std::exception& e) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: 日志写入线程: 异常[%s]!\n"), ToString(LogLevelFlags::Warning), u8tow(e.what())), LogLevelFlags::Error);
			}
			catch (...) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::LogWriteToFile: 日志写入线程: 未知异常!\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
				throw Truntime_error(TEXT("Log::LogWriteToFile: Unknown exception!"));
			}
		}

	private:
		/*
		* 启用 ANSI转义符支持
		*/
		template<typename T = bool>
		void EnableAnsiEscape()
		{
#ifdef _WINDOWS
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD dwMode = 0;
			if (GetConsoleMode(hOut, &dwMode)) {
				dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				SetConsoleMode(hOut, dwMode);
			}
#endif
		}

		template<typename T = bool>
		void LogMessageBox(const Tchar* Text, const LogLevelFlags& _LogLevel, int32_t MessageBoxIcon = MB_OK)
		{
#ifdef _WINDOWS
#ifdef UTF8
			MessageBoxW(NULL, UTF8ToWstring(Text).c_str(), UTF8ToWstring(ToString(_LogLevel, true)).c_str(), MessageBoxIcon);

#else
#ifdef UNICODE
			MessageBoxW(NULL, Text, ToString(_LogLevel, true), MessageBoxIcon);
#else

			MessageBoxW(NULL, UTF8ToWstring(Text).c_str(), UTF8ToWstring(ToString(_LogLevel, true)).c_str(), MessageBoxIcon);
#endif
#endif
#endif
		}

	public:
		template<typename T = bool>
		static Tstring GetFormatTime(const Tstring& _TimeFormat = TEXT("%Y-%m-%d %H:%M:%S"),
			const Tstring& _TextLeftFormat = TEXT("["), const Tstring& _TextRigthFormat = TEXT("]"))
		{
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();;
			// 获取当前时间点（自epoch以来的时间）
			// 将时间点转换为time_t（用于localtime函数）
			std::time_t ttm = std::chrono::system_clock::to_time_t(now);
			// 使用localtime函数将time_t转换为本地时间（std::tm结构）
			std::tm* now_tm = std::localtime(&ttm);
			// 使用 std::put_time 格式化时间
			Tostringstream oss;
			oss << std::put_time(now_tm, _TimeFormat.c_str()); // 自定义时间格式
			//不需要修饰字符时, 直接返回格式化后的时间文本
			if (_TextLeftFormat == TEXT("") && _TextRigthFormat == TEXT("")) {
				return oss.str();
			}
			else {
				return Printf(TEXT("%s%s%s"), _TextLeftFormat, oss.str(), _TextRigthFormat).ToString();
			}
		}

		template<typename T = bool>
		static void Sleep(int64_t _Number, bool _bShowLog)
		{
			if (_bShowLog) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::Sleep: [%s]ms\n"), ToString(LogLevelFlags::Warning), _Number), LogLevelFlags::Error);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(_Number));
			if (_bShowLog) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::Sleep: 结束\n"), ToString(LogLevelFlags::Warning)), LogLevelFlags::Error);
			}
		}

		template<typename T = bool>
		static void Wait(int64_t _Number, bool _bShowLog)
		{
			if (_bShowLog) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::Wait: [%s]ms\n"), ToString(LogLevelFlags::Warning), _Number), LogLevelFlags::Error);
			}
			std::chrono::milliseconds timeTarget = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()) +
				std::chrono::milliseconds(_Number);
			while (timeTarget > std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())) {}
			if (_bShowLog) {
				Log_Out(LogErrOstream, Printf(TEXT("%sLog::Wait: 结束\n"), ToColor(AnsiescColor::Yellow)), LogLevelFlags::Warning);
			}
		}
	};


	/* 模式 mode : (_CONSOLE | _WINDOWS)
	*/
	TYPICALTOOL_API extern Log LogGuiDebug; //Debug
	TYPICALTOOL_API extern Log LogGuiRelease; //Release
	/* 模式 mode: (_CONSOLE)
	*/
	TYPICALTOOL_API extern Log LogDebug; //Debug
	TYPICALTOOL_API extern Log LogRelease; //Relase


	//class TYPICALTOOL_API GeneralLogger {
	//public:
	//	// 定义日志回调类型（支持函数、lambda、成员函数等）
	//	using LogCallback = std::function<void()>;
	//	const Tchar* LogString;
	//private:
	//	std::mutex MutexCallback;
	//	LogCallback LogCallbackOutput = nullptr;

	//public:
	//	GeneralLogger() { }
	//	GeneralLogger(LogCallback _Callback)
	//	{
	//		SetOutputCallback(_Callback);
	//	}

	//	// 设置全局日志回调
	//	template<class T = bool>
	//	void SetOutputCallback(LogCallback _Callback)
	//	{
	//		std::lock_guard<std::mutex> lock(MutexCallback);
	//		LogCallbackOutput = _Callback;
	//	}

	//	// 通用日志接口
	//	template<class T = bool>
	//	void Out(const Tchar* String)
	//	{
	//		std::lock_guard<std::mutex> lock(MutexCallback);

	//		if (!LogCallbackOutput) {
	//			LogErrOstream << TEXT("Deadly Error: GeneralLogger is null! (TypicalTool: Log.h -> GeneralLogger)\n");
	//			return;  // 未设置回调时忽略
	//		}
	//		LogCallbackOutput(String);
	//	}

	//	template<class T = bool>
	//	void operator()(const Tchar* String)
	//	{
	//		Out(String);
	//	}
	//};

	//// System API: Windows
	//TYPICALTOOL_API extern GeneralLogger LogDebugWin = []() { tytl::LogGuiDebug.Out(TypicalTool::LogLevelFlags::Error, LogString); };
	//TYPICALTOOL_API extern GeneralLogger LogReleaseWin = []() { tytl::LogGuiRelease.Out(TypicalTool::LogLevelFlags::Error, LogString); };

	//// Error
	//TYPICALTOOL_API extern GeneralLogger LogError = []() { tytl::LogRelease.Out(TypicalTool::LogLevelFlags::Error, LogString); };
	//// Warning
	//TYPICALTOOL_API extern GeneralLogger LogWarning = []() { tytl::LogDebug.Out(TypicalTool::LogLevelFlags::Warning, LogString); };
	//// Info
	//TYPICALTOOL_API extern GeneralLogger LogInfo = []() { tytl::LogDebug.Out(TypicalTool::LogLevelFlags::Info, LogString); };
	//// LineFeed
	//TYPICALTOOL_API extern GeneralLogger LogOut = []() { tytl::LogDebug.Out(TypicalTool::LogLevelFlags::LineFeed, LogString); };
	//// LineEnd
	//TYPICALTOOL_API extern GeneralLogger LogEnd = []() { tytl::LogDebug.Out(TypicalTool::LogLevelFlags::LineEnd, LogString); };
}


#endif
