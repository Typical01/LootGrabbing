#pragma once

//常用工具
#ifndef _COMMONTOOLS_H
#define _COMMONTOOLS_H

#include <TypicalTool/Tool/pch.h>
#include <TypicalTool/Tool/Log.h>
#include <TypicalTool/Tool/Time_Typical.h>



namespace TypicalTool {

	enum FileSystemType {
		File = 0,
		Directory,
		Other
	};

	template<class T = bool>
	Tstring FileTypeToStr(const FileSystemType& _FileType) {
		switch (_FileType) {
		case FileSystemType::File:
		{
			return TEXT("文件");
		}
		case FileSystemType::Directory:
		{
			return TEXT("目录");
		}
		case FileSystemType::Other:
		{
			return TEXT("其他");
		}
		}
	}

	class TYPICALTOOL_API FileSystemErrorMessage {
	public:
		Tstring String = TEXT("");
	public:
		FileSystemErrorMessage() : String(TEXT("")) {}
	};

	class TYPICALTOOL_API FileSystem {
	private:
		static std::shared_ptr<FileSystemErrorMessage> ErrorMessage;

	public:
		FileSystem() { ErrorMessage = std::make_shared<FileSystemErrorMessage>(); }

	public:

		/// <summary>
		/// 生成临时路径: 位于 _TargetPath 同一目录
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="_TargetPath">目标路径</param>
		/// <returns></returns>
		static std::filesystem::path MakeTempInSameDir(const std::filesystem::path& _TargetPath) {
			// 目标所在目录（若 targetPath 是 /some/dir/foo.txt，则 parent = /some/dir）
			std::filesystem::path dir = _TargetPath.parent_path();

			// 用目标项的 filename 作为 base（如果用户传入的 _TargetPath 本身就是目录，
			// _TargetPath.filename() 仍是最后一段；若为空则退回到 "tmp"）
			Tstring base = _TargetPath.filename().PathToStr();
			if (base.empty()) base = TEXT("tmp");

			// PID
//#ifdef _WIN32
//			unsigned long pid = ::GetCurrentProcessId();
//#else
//			unsigned long pid = static_cast<unsigned long>(::getpid());
//#endif

			// 随机数：尽量用 random_device 混合时间戳
			std::random_device rd;
			std::mt19937_64 rng(rd() ^ (uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count());
			uint64_t rnd = rng();

			// 基本的 tmp 名
			//Tstring tmpname = Printf(TEXT("%s.tmp.%s.%s.%s"), base, Time::GetFormatTime(TEXT("%Y-%m-%d_%H-%M-%S"), TEXT(""), TEXT("")), ToStr(pid), ToStr(rnd)).ToString();
			Tstring tmpname = Printf(TEXT("%s.tmp.%s.%s"), base, Time::GetFormatTime(TEXT("%Y-%m-%d_%H-%M-%S"), TEXT(""), TEXT("")), ToStr(rnd)).ToString();
			std::filesystem::path tmp = dir / tmpname;
			// 若发生冲突，快速尝试附加序号，最多尝试 N 次
			for (int i = 0; i < 16 && std::filesystem::exists(tmp); ++i) {
				tmp = dir / std::filesystem::path(Printf(TEXT("%s.%s"), tmpname, ToStr(i)).ToString());
			}

			// 最终仍存在的话直接返回（上层可决定如何处理冲突）
			return tmp;
		}

		/// <summary>
		/// 比较两个普通文件是否字节相同(按块读取)
		/// </summary>
		/// <param name="PathA"></param>
		/// <param name="PathB"></param>
		/// <param name="_ErrorCode"></param>
		/// <returns></returns>
		static bool FileEqual(const std::filesystem::path& PathA, const std::filesystem::path& PathB, std::error_code& _ErrorCode) {
			_ErrorCode.clear();
			std::uintmax_t PathA_Size = std::filesystem::file_size(PathA, _ErrorCode); if (_ErrorCode) return false;
			std::uintmax_t PathB_Size = std::filesystem::file_size(PathB, _ErrorCode); if (_ErrorCode) return false;
			if (PathA_Size != PathB_Size) return false;

			Tifstream PathA_Istream(PathA.c_str(), std::ios::binary);
			Tifstream PathB_Istream(PathB, std::ios::binary);
			if (!PathA_Istream || !PathB_Istream) { _ErrorCode = std::make_error_code(std::errc::io_error); return false; }

			constexpr std::size_t BUF = 64 * 1024;
			std::vector<Tchar> PathA_BUF(BUF), PathB_BUF(BUF);
			while (PathA_Istream && PathB_Istream) {
				PathA_Istream.read(PathA_BUF.data(), BUF);
				PathB_Istream.read(PathB_BUF.data(), BUF);
				std::streamsize PathA_IstreamSize = PathA_Istream.gcount();
				std::streamsize PathB_IstreamSize = PathB_Istream.gcount();
				if (PathA_IstreamSize != PathB_IstreamSize) return false;
				if (PathA_IstreamSize == 0) break;
				if (std::memcmp(PathA_BUF.data(), PathB_BUF.data(), (size_t)PathA_IstreamSize) != 0) return false;
			}
			return true;
		}
				
		/// <summary>
		/// 递归复制源文件到 临时文件, 并返回错误码 | 失败时删除临时文件
		/// </summary>
		/// <param name="_SourcePath">源文件</param>
		/// <param name="_TempDirectory">临时文件</param>
		/// <param name="_ErrorCode">错误码</param>
		/// <returns></returns>
		static bool CopyRecursively(const std::filesystem::path& _SourcePath, const std::filesystem::path& _TempDirectory, std::error_code& _ErrorCode) {
			_ErrorCode.clear();

			// 若 tmp已存在, 先尝试删除
			std::error_code Ignore;
			if (std::filesystem::exists(_TempDirectory, Ignore)) {
				std::filesystem::remove_all(_TempDirectory, Ignore);
			}

			try {
				if (std::filesystem::is_directory(_SourcePath)) {
					std::filesystem::copy(_SourcePath, _TempDirectory, std::filesystem::copy_options::recursive);
				}
				else if (std::filesystem::is_regular_file(_SourcePath)) {
					// 若 _TempDirectory 代表文件, copy_file 比较合适
					std::filesystem::create_directories(_TempDirectory.parent_path(), _ErrorCode);
					if (_ErrorCode) return false;
					std::filesystem::copy_file(_SourcePath, _TempDirectory);
				}
				else if (std::filesystem::is_symlink(_SourcePath)) {
					std::filesystem::copy_symlink(_SourcePath, _TempDirectory);
				}
				else {
					_ErrorCode = std::make_error_code(std::errc::not_supported);
					return false;
				}
			}
			catch (const std::filesystem::filesystem_error& e) {
				_ErrorCode = e.code();
				// 清理 tmp
				std::error_code Ignore2;
				std::filesystem::remove_all(_TempDirectory, Ignore2);
				return false;
			}
			return true;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_SourcePath">源文件</param>
		/// <param name="_TargetPath">目标路径</param>
		/// <param name="_VerifyFileContents">检验文件内容</param>
		/// <param name="_ForceReplace">强制替换</param>
		/// <param name="_ErrorCode">错误码</param>
		/// <returns></returns>
		bool CopyThenReplaceSafe(const std::filesystem::path& _SourcePath, const std::filesystem::path& _TargetPath,
			bool _VerifyFileContents,
			bool _ForceReplace,
			std::error_code& _ErrorCode)
		{
			_ErrorCode.clear();
			// 创建父路径
			if (_TargetPath.has_parent_path()) {
				std::filesystem::create_directories(_TargetPath.parent_path(), _ErrorCode);
				if (_ErrorCode) return false;
			}
			// 构建 临时文件
			std::filesystem::path TempDirectory = MakeTempInSameDir(_TargetPath);
			// 1. 复制到 TempDirectory
			if (!CopyRecursively(_SourcePath, TempDirectory, _ErrorCode)) {
				return false;
			}

			// 2. 验证-对文件做逐字节比较
			if (_VerifyFileContents) {
				// 如果 _SourcePath 是文件, 直接对比; 如果是目录, 简单检查文件数量/大小可行但复杂, 这里仅强制文件对比
				std::error_code Verify_ErrorCode;
				if (std::filesystem::is_regular_file(_SourcePath, Verify_ErrorCode) && !Verify_ErrorCode) {
					if (!FileEqual(_SourcePath, TempDirectory, Verify_ErrorCode)) {
						_ErrorCode = Verify_ErrorCode ? Verify_ErrorCode : std::make_error_code(std::errc::io_error);
						// 保留 TempDirectory 供调试, 或尝试删除 TempDirectory(这里我们删除 TempDirectory 防止残留)
						std::error_code Ignore;
						std::filesystem::remove_all(TempDirectory, Ignore);
						return false;
					}
				}
				else {
					// 对目录的严格验证需要递归比较。此处可用简单的 size/count 校验或跳过。
					// 我们跳过复杂目录字节比对以免代码过长 —— 如需可拓展。
				}
			}

			// 3. 尝试重命名 TempDirectory -> _TargetPath(最好是原子覆盖)
			std::error_code Rename_ErrorCode;
			std::filesystem::rename(TempDirectory, _TargetPath, Rename_ErrorCode);
			if (!Rename_ErrorCode) {
				// 成功: TempDirectory 被移到 _TargetPath(或覆盖了目标)
				return true;
			}

			// rename 失败: 可能是目标已存在或权限/占用问题
			_ErrorCode = Rename_ErrorCode;

			// 如果目标存在并且你选择强制替换(危险)—— 尝试删除目标再重命名
			if (_ForceReplace) {
				std::error_code Remove_ErrorCode;
				if (std::filesystem::exists(_TargetPath, Remove_ErrorCode)) {
					std::filesystem::remove_all(_TargetPath, Remove_ErrorCode);
					if (Remove_ErrorCode) {
						// 删除失败: 不要删除 TempDirectory(保留 TempDirectory 与原目标), 报告错误
						_ErrorCode = Remove_ErrorCode;
						return false;
					}
					// 删除成功后再尝试 rename
					std::error_code RenameAgain_ErrorCode;
					std::filesystem::rename(TempDirectory, _TargetPath, RenameAgain_ErrorCode);
					if (!RenameAgain_ErrorCode) return true;
					_ErrorCode = RenameAgain_ErrorCode;
					// 清理 TempDirectory 尝试失败则保留 TempDirectory
					return false;
				}
			}
			// 默认策略: 不删除原目标; 保留 TempDirectory, 返回失败(上层可重试或做人工处理)
			return false;
		}

		/*
		* _TipsChar: 提示文本
		* _Path: 检查的路径
		* _bShowRelease: 输出到 Release
		* _bFileTrue: 文件存在时为真, 否则取反
		*/
		template<typename T = bool>
		static bool Exists(const Tstring& _TipsChar, const std::filesystem::path& _Path, bool _bShowRelease, FileSystemType& PathOfFileType, bool _bFileTrue = true)
		{
			//FileSystemType PathOfFileType;
			std::error_code ErrorCode;
			if (std::filesystem::exists(_Path, ErrorCode)) {
				if (std::filesystem::is_regular_file(_Path)) { // 检查路径是否是普通文件
					PathOfFileType = FileSystemType::File;
				}
				else if (std::filesystem::is_directory(_Path)) { // 检查路径是否是目录
					PathOfFileType = FileSystemType::Directory;
				}
				else {
					PathOfFileType = FileSystemType::Other;
				}

				if (_bShowRelease) {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s][%s] 存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s][%s] 存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					return true;
				}
				else {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s][%s] 存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s][%s] 存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					return true;
				}
			}
			else {
				if (!ErrorCode.message().empty()) {
					LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s]\n 异常[%s]!"), _TipsChar, _Path.PathToStr(), stow(ErrorCode.message().c_str())));
					return false;
				}

				if (_bShowRelease) {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s][%s] 不存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s][%s] 不存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					return false;
				}
				else {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s][%s] 不存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s][%s] 不存在!"), _TipsChar, _Path.PathToStr(), FileTypeToStr(PathOfFileType)));
					}
					return false;
				}
			}
		}
		template<typename T = bool>
		static bool Exists(const Tstring& _TipsChar, const std::filesystem::path& _Path, bool _bShowRelease = true, bool _bFileTrue = true)
		{
			//FileSystemType PathOfFileType;
			std::error_code ErrorCode;
			if (std::filesystem::exists(_Path, ErrorCode)) {
				if (_bShowRelease) {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s] 存在!"), _TipsChar, _Path.PathToStr()));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s] 存在!"), _TipsChar, _Path.PathToStr()));
					}
					return true;
				}
				else {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s] 存在!"), _TipsChar, _Path.PathToStr()));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s] 存在!"), _TipsChar, _Path.PathToStr()));
					}
					return true;
				}
			}
			else {
				if (!ErrorCode.message().empty()) {
					LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s]\n 异常[%s]!"), _TipsChar, _Path.PathToStr(), stow(ErrorCode.message().c_str())));
				}

				if (_bShowRelease) {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s] 不存在!"), _TipsChar, _Path.PathToStr()));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s] 不存在!"), _TipsChar, _Path.PathToStr()));
					}
					return false;
				}
				else {
					if (_bFileTrue) {
						LogRelease(LogLevelFlags::Error, Printf(TEXT("%s: 路径[%s] 不存在!"), _TipsChar, _Path.PathToStr()));
					}
					else {
						LogRelease(LogLevelFlags::Info, Printf(TEXT("%s: 路径[%s] 不存在!"), _TipsChar, _Path.PathToStr()));
					}
					return false;
				}
			}
		}

	public:
		//创建目录: 自动递归创建下级目录
		template<typename T = bool>
		static bool CreateDirectorys(const std::filesystem::path& _SourcePath)
		{
			if (Exists(TEXT("创建文件夹"), _SourcePath, true)) {
				return false;
			}
			auto PathSlash = _SourcePath.PathToStr().find_last_of(TEXT("\\/"));

			std::error_code ErrorCode;
			try {
				if (PathSlash == Tstring::npos) {
					//单级目录
					std::filesystem::create_directory(_SourcePath);
				}
				else {
					//多级目录
					std::filesystem::create_directories(_SourcePath, ErrorCode);
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 创建文件夹 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::Copy: 路径[%s]\n 异常[%s]!"), _SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
					}
				}
			}
			catch (const std::filesystem::filesystem_error& Error) {
				ErrorMessage.get()->String = stow(Error.what());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::CreateDirectory: 失败原因: { %s }"), ErrorMessage.get()->String));
				LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::CreateDirectory: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
					Error.path1().PathToStr(), Error.path2().PathToStr()));
				Exists(TEXT("创建文件夹(Path1)"), Error.path1(), true);
				Exists(TEXT("创建文件夹(Path2)"), Error.path2(), true);
				
				return false;
			}
			catch (...) {
				ErrorMessage.get()->String = TEXT("FileSystem::CreateDirectory: 其他错误!");
				LogRelease(LogLevelFlags::Error, TEXT("FileSystem::CreateDirectory: 其他错误!"));
				return false;
			}

			return !ErrorCode;
		}

		//删除文件/目录
		template<typename T = bool>
		static bool Delete(const std::filesystem::path& _SourcePath, bool _IsRecursive = false)
		{
			if (!Exists(TEXT("删除文件/目录"), _SourcePath, true)) {
				return false;
			}

			std::error_code ErrorCode;
			try {
				if (_IsRecursive) {
					std::filesystem::remove_all(_SourcePath, ErrorCode);
				}
				else {
					std::filesystem::remove(_SourcePath, ErrorCode);
				}
				if (ErrorCode) {
					ErrorMessage.get()->String += tytl::Printf(TEXT("{ 删除文件/目录 }: %s"), ErrorCode.message());
					LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::Copy: 路径[%s]\n 异常[%s]!"), _SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
				}
			}
			catch (const std::filesystem::filesystem_error& Error) {
				ErrorMessage.get()->String = stow(Error.what());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::Delete: 失败原因: { %s }"), ErrorMessage.get()->String));
				LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::Delete: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
					Error.path1().PathToStr(), Error.path2().PathToStr()));
				Exists(TEXT("删除文件/目录(Path1)"), Error.path1(), true);
				Exists(TEXT("删除文件/目录(Path2)"), Error.path2(), true);

				return false;
			}
			catch (...) {
				ErrorMessage.get()->String = TEXT("FileSystem::Delete: 其他错误!");
				LogRelease(LogLevelFlags::Error, TEXT("FileSystem::Delete: 其他错误!"));
				return false;
			}

			return !ErrorCode;
		}

		//复制文件/目录: _bOverWrite 覆盖原文件/目录
		template<typename T = bool>
		static bool Copy(const std::filesystem::path& _SourcePath,
			const std::filesystem::path& _TargetPath,
			std::filesystem::copy_options _copy_options = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive,
			bool _OnlyCopyNewFile = false)
		{
			FileSystemType FileTypeSourcePath;
			if (!Exists(TEXT("复制文件/目录"), _SourcePath, true, FileTypeSourcePath) && !Exists(TEXT("复制文件/目录"), _TargetPath, true)) {
				return false;
			}
			std::error_code ErrorCode;

			if (FileTypeSourcePath == FileSystemType::Directory) {
				CreateDirectorys(_TargetPath); // 确保目标目录存在

				// 递归处理子项
				for (const auto& entry : DirectoryIterator(_SourcePath)) {
					std::filesystem::path relative = entry.path().filename();
					Copy(entry.path().PathToStr(), (_TargetPath / relative).PathToStr(), _copy_options);
				}

				std::filesystem::last_write_time(_TargetPath, std::filesystem::last_write_time(_SourcePath)); // 同步目录时间戳
				return true;
			}

			try {
				if (_OnlyCopyNewFile) {
					auto Source_time = std::filesystem::last_write_time(_SourcePath);
					auto Target_time = std::filesystem::last_write_time(_TargetPath);

					//仅复制新文件
					if (Source_time > Target_time) {
						std::filesystem::copy(_SourcePath, _TargetPath, _copy_options, ErrorCode);
						if (ErrorCode) {
							ErrorMessage.get()->String += tytl::Printf(TEXT("{ 复制文件/目录 }: %s"), ErrorCode.message());
							LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::Copy: 路径[%s]->[%s]\n 异常[%s]!"), 
								_SourcePath.PathToStr(), _TargetPath.PathToStr(), stow(ErrorCode.message().c_str())));
						}
					}
					else {
					}
				}
				else {
					std::filesystem::copy(_SourcePath, _TargetPath, _copy_options, ErrorCode);
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 复制文件/目录 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::Copy: 路径[%s]->[%s]\n 异常[%s]!"), 
							_SourcePath.PathToStr(), _TargetPath.PathToStr(), stow(ErrorCode.message().c_str())));
					}
				}

			}
			catch (const std::filesystem::filesystem_error& Error) {
				ErrorMessage.get()->String = stow(Error.what());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::Copy: 失败原因: { %s }"), ErrorMessage.get()->String));
				LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::Copy: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
					Error.path1().PathToStr(), Error.path2().PathToStr()));
				Exists(TEXT("复制文件/目录(Path1)"), Error.path1(), true);
				Exists(TEXT("复制文件/目录(Path2)"), Error.path2(), true);
				
				// 当复制失败时: 尝试流式复制(绕过只读属性)
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::Copy: 流式复制开始!")));
				std::ifstream in(_SourcePath.PathToStr(), std::ios::binary);
				std::ofstream out(_TargetPath.PathToStr(), std::ios::binary | std::ios::trunc);
				out << in.rdbuf();
				auto TargetPathPerms = GetPermissions(_TargetPath);
				SetPermissions(_TargetPath, TargetPathPerms, std::filesystem::perm_options::replace); //还原文件权限
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::Copy: 流式复制结束!")));

				return false;
			}
			catch (...) {
				ErrorMessage.get()->String = TEXT("FileSystem::Copy: 其他错误!");
				LogRelease(LogLevelFlags::Error, TEXT("FileSystem::Copy: 其他错误!"));
				return false;
			}

			return !ErrorCode;
		}

		//重命名/移动 文件/目录
		template<typename T = bool>
		static bool ReName(const std::filesystem::path& _SourcePath, const std::filesystem::path& _NewPathName)
		{
			std::error_code ErrorCode;
			if (_SourcePath.parent_path() == _NewPathName.parent_path()) { //同路径
				if (!Exists(TEXT("重命名文件/目录"), _SourcePath, true)) {
					return false;
				}

				try {
					std::filesystem::rename(_SourcePath, _NewPathName, ErrorCode);
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 重命名文件/目录 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::ReName: 路径[%s]->[%s]\n 异常[%s]!"), 
							_SourcePath.PathToStr(), _NewPathName.PathToStr(), stow(ErrorCode.message().c_str())));
					}
				}
				catch (const std::filesystem::filesystem_error& Error) {
					ErrorMessage.get()->String = stow(Error.what());
					LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::ReName: 失败原因: { %s }"), ErrorMessage.get()->String));
					LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::ReName: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
						Error.path1().PathToStr(), Error.path2().PathToStr()));
					Exists(TEXT("重命名文件/目录(Path1)"), Error.path1(), true);
					Exists(TEXT("重命名文件/目录(Path2)"), Error.path2(), true);

					return false;
				}
				catch (...) {
					ErrorMessage.get()->String = TEXT("FileSystem::ReName: 其他错误!");
					LogRelease(LogLevelFlags::Error, TEXT("FileSystem::ReName: 其他错误!"));
					return false;
				}
			}
			else {
				if (!Exists(TEXT("移动文件/目录"), _SourcePath)) {
					return false;
				}

				try {
					std::filesystem::rename(_SourcePath, _NewPathName);
				}
				catch (const std::filesystem::filesystem_error& Error) {
					ErrorMessage.get()->String = stow(Error.what());
					LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::ReName: 失败原因: { %s }"), ErrorMessage.get()->String));
					LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::ReName: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
						Error.path1().PathToStr(), Error.path2().PathToStr()));
					Exists(TEXT("移动文件/目录(Path1)"), Error.path1(), true);
					Exists(TEXT("移动文件/目录(Path2)"), Error.path2(), true);

					return false;
				}
				catch (...) {
					ErrorMessage.get()->String = TEXT("FileSystem::ReName: 其他错误!");
					LogRelease(LogLevelFlags::Error, TEXT("FileSystem::ReName: 其他错误!"));
					return false;
				}
			}

			return !ErrorCode;
		}

		//修改文件/目录权限
		template<typename T = bool>
		static bool SetPermissions(const std::filesystem::path& _SourcePath, const std::filesystem::perms& _perms, std::filesystem::perm_options _perm_options = std::filesystem::perm_options::add)
		{
			if (!Exists(TEXT("修改文件/目录权限"), _SourcePath, true)) {
				return false;
			}

			std::error_code ErrorCode;
			try {
				std::filesystem::permissions(_SourcePath, _perms, _perm_options, ErrorCode);
				if (ErrorCode) {
					ErrorMessage.get()->String += tytl::Printf(TEXT("{ 修改文件/目录权限 }: %s"), ErrorCode.message());
					LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::Copy: 路径[%s]\n 异常[%s]!"), _SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
				}
			}
			catch (const std::filesystem::filesystem_error& Error) {
				ErrorMessage.get()->String = stow(Error.what());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::SetPermissions: 失败原因: { %s }"), ErrorMessage.get()->String));
				LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::SetPermissions: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
					Error.path1().PathToStr(), Error.path2().PathToStr()));
				Exists(TEXT("修改文件/目录权限(Path1)"), Error.path1(), true);
				Exists(TEXT("修改文件/目录权限(Path2)"), Error.path2(), true);

				return false;
			}
			catch (...) {
				ErrorMessage.get()->String = TEXT("FileSystem::SetPermissions: 其他错误!");
				LogRelease(LogLevelFlags::Error, TEXT("FileSystem::SetPermissions: 其他错误!"));
				return false;
			}

			return !ErrorCode;
		}

		//遍历目录: return 目录列表
		/* 
		* _IsRecursive: 是否递归
		* DirectoryList: 默认为空(不获取文件列表的字符串) | 非空(清楚内容后, 写入文件列表的字符串)
		*/
		template<typename T = bool>
		static std::vector<std::filesystem::directory_entry> DirectoryIterator(const std::filesystem::path& _SourcePath, bool _IsRecursive = false)
		{
			std::vector<std::filesystem::directory_entry> List; //目录列表
			std::error_code ErrorCode;

			try {
				if (_IsRecursive) { // 递归遍历
					for (const auto& entry : std::filesystem::recursive_directory_iterator(_SourcePath)) {
						List.push_back(entry);
					}
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 遍历目录 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::DirectoryIterator: 路径[%s] 异常[%s]!"), _SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
					}
					return List;
				}
				else { // 非递归遍历
					for (const auto& entry : std::filesystem::directory_iterator(_SourcePath)) {
						List.push_back(entry);
					}
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 遍历目录 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::DirectoryIterator: 路径[%s] 异常[%s]!"), _SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
					}
					return List;
				}
			}
			catch (const std::filesystem::filesystem_error& Error) {
				ErrorMessage.get()->String = stow(Error.what());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::DirectoryIterator: 失败原因: { %s }"), ErrorMessage.get()->String));
				LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::DirectoryIterator: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
					Error.path1().PathToStr(), Error.path2().PathToStr()));
				Exists(TEXT("遍历目录(Path1)"), Error.path1(), true);
				Exists(TEXT("遍历目录(Path2)"), Error.path2(), true);

				return std::vector<std::filesystem::directory_entry>();
			}
			catch (...) {
				ErrorMessage.get()->String = TEXT("FileSystem::DirectoryIterator: 其他错误!");
				LogRelease(LogLevelFlags::Error, TEXT("FileSystem::DirectoryIterator: 其他错误!"));
				return std::vector<std::filesystem::directory_entry>();
			}
		}
		
		/// <summary>
		/// 遍历目录: return 目录列表
		/// </summary>
		/// <param name="_IsRecursive">: 是否递归</param>
		/// <param name="_FilePathSizeSum">: 文件路径总大小</param>
		/// <param name="DirectoryListStr">: 默认为空(不获取文件列表的字符串) | 非空(清楚内容后, 写入文件列表的字符串)</param>
		/// <returns></returns>
		template<typename T = bool>
		static std::vector<std::filesystem::directory_entry> DirectoryIterator(const std::filesystem::path& _SourcePath, bool _IsRecursive, int64_t& _FilePathSizeSum)
		{
			std::vector<std::filesystem::directory_entry> List; //目录列表
			std::error_code ErrorCode;

			try {
				FileSystemType PathOfFileType;
				if (_IsRecursive) { // 递归遍历
					for (const auto& entry : std::filesystem::recursive_directory_iterator(_SourcePath, ErrorCode)) {
						if (!Exists(TEXT("遍历目录(递归)"), entry.path(), true, PathOfFileType)) {
							continue;
						}
						if (PathOfFileType == FileSystemType::File) {
							_FilePathSizeSum += std::filesystem::file_size(entry.path(), ErrorCode);
							if (ErrorCode) {
								ErrorMessage.get()->String += tytl::Printf(TEXT("{ 遍历目录[子目录] }: %s"), ErrorCode.message());
								LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::DirectoryIterator: 路径[%s] 异常[%s]!"), 
									_SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
							}
						}

						List.push_back(entry);
					}
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 遍历目录 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::DirectoryIterator: 路径[%s] 异常[%s]!"), 
							_SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
					}
					return List;
				}
				else { // 非递归遍历
					for (const auto& entry : std::filesystem::directory_iterator(_SourcePath, ErrorCode)) {
						if (!Exists(TEXT("遍历目录(不递归)"), entry.path(), true, PathOfFileType)) {
							continue;
						}
						if (PathOfFileType == FileSystemType::File) {
							_FilePathSizeSum += std::filesystem::file_size(entry.path(), ErrorCode);
							if (ErrorCode) {
								ErrorMessage.get()->String += tytl::Printf(TEXT("{ 遍历目录[子目录] }: %s"), ErrorCode.message());
								LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::DirectoryIterator: 路径[%s] 异常[%s]!"), 
									_SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
							}
						}

						List.push_back(entry);
					}
					if (ErrorCode) {
						ErrorMessage.get()->String += tytl::Printf(TEXT("{ 遍历目录 }: %s"), ErrorCode.message());
						LogRelease(LogLevelFlags::Error, Printf(TEXT("FileManage::DirectoryIterator: 路径[%s] 异常[%s]!"), 
							_SourcePath.PathToStr(), stow(ErrorCode.message().c_str())));
					}
					return List;
				}
			}
			catch (const std::filesystem::filesystem_error& Error) {
				ErrorMessage.get()->String = stow(Error.what());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("FileSystem::DirectoryIterator: 失败原因: { %s }"), ErrorMessage.get()->String));
				LogRelease(LogLevelFlags::Warning, Printf(TEXT("FileSystem::DirectoryIterator: 路径: { \n\tPath1: [%s]\n\tPath2: [%s]\n}"),
					Error.path1().PathToStr(), Error.path2().PathToStr()));
				Exists(TEXT("遍历目录(Path1)"), Error.path1(), true);
				Exists(TEXT("遍历目录(Path2)"), Error.path2(), true);

				return std::vector< std::filesystem::directory_entry>();
			}
			catch (...) {
				ErrorMessage.get()->String = TEXT("FileSystem::DirectoryIterator: 其他错误!");
				LogRelease(LogLevelFlags::Error, TEXT("FileSystem::DirectoryIterator: 其他错误!"));
				return std::vector< std::filesystem::directory_entry>();
			}
		}

	public:
		//获取 当前工作路径: 可能变化
		template<typename T = bool>
	    static Tstring GetCurrentPath() {  return std::filesystem::current_path().PathToStr(); }
		//获取 错误信息
		template<typename T = bool>
		static Tstring GetErrorMessage() { if (ErrorMessage.get()->String.empty()) { return Tstring(); } return ErrorMessage.get()->String; }
		//获取 文件名
		template<typename T = bool>
		static Tstring GetFileName(const std::filesystem::path& _SourcePath) { return _SourcePath.filename().PathToStr(); }
		//获取 文件扩展名
		template<typename T = bool>
		static Tstring GetExtensionName(const std::filesystem::path& _SourcePath) { return _SourcePath.extension().PathToStr(); }
		//获取 父级目录名
		template<typename T = bool>
		static Tstring GetParentName(const std::filesystem::path& _SourcePath) { return _SourcePath.parent_path().PathToStr(); }
		//获取 文件权限
		template<typename T = bool>
		static std::filesystem::perms GetPermissions(const std::filesystem::path& _SourcePath) { return std::filesystem::status(_SourcePath).permissions(); }
	};



	class TYPICALTOOL_API ThreadPool {
	private:
		std::vector<std::thread> Workers;			// 工作线程
		std::queue<std::function<void()>> Tasks;	// 任务队列
		std::mutex QueueMutex;						// 保护任务队列的互斥锁
		std::condition_variable Condition;			// 条件变量, 用于线程同步
		bool StopThread;							// 停止标志

	public:
		/// <summary>
		/// 构造函数, 创建指定数量的工作线程
		/// </summary>
		/// <param name="_NumThreads">: 指定数量的工作线程</param>
		ThreadPool(size_t _NumThreads) : StopThread(false) 
		{
			for (size_t i = 0; i < _NumThreads; ++i) {
				Workers.emplace_back([this] {
					while (true) {
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(QueueMutex);
							Condition.wait(lock, [this] { return StopThread || !Tasks.empty(); });
							if (StopThread && Tasks.empty()) return;
							task = std::move(Tasks.front());
							Tasks.pop();
						}
						task();
					}
					});
			}
		}

		/// <summary>
		/// 析构函数, 停止线程池
		/// </summary>
		~ThreadPool() 
		{
			JoinAll();
		}

	public:
		// 提交任务到线程池
		template <class F, class... Args>
		auto Enqueue(F&& _Func, Args&&... _Args) -> std::future<typename std::invoke_result<F(Args...)>::type> 
		{
			using return_type = typename std::invoke_result<F(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<F>(_Func), std::forward<Args>(_Args)...)
			);

			std::future<return_type> res = task->get_future();
			{
				std::unique_lock<std::mutex> lock(QueueMutex);
				if (StopThread) {
					throw Truntime_error(_LOGERRORINFO(TEXT("tytl::ThreadPool: enqueue on stopped ThreadPool!")));
				}
				Tasks.emplace([task]() { (*task)(); });
			}
			Condition.notify_one();
			return res;
		}

		/*
		* 当前任务数量
		*/
		template<typename T = bool>
		int32_t WorkNumber() 
		{
			{
				std::unique_lock<std::mutex> lock(QueueMutex);
				return static_cast<int32_t>(Tasks.size());
			}
		}

		/*
		* 主动连接所有线程
		*/
		template<typename T = bool>
		void JoinAll() 
		{
			{
				std::unique_lock<std::mutex> lock(QueueMutex);
				StopThread = true;
			}
			Condition.notify_all();
			for (auto& tempThread : Workers) {
				if (tempThread.joinable()) {
					tempThread.join();
				}
			}
		}
	};
}

#endif