#ifndef _JSON_TYPICAL_H
#define _JSON_TYPICAL_H

#include <TypicalTool/Tool/pch.h>
#include <TypicalTool/Tool/Log.h>

#include <jsoncons/json.hpp>
#include <jsoncons/json_options.hpp>


#ifndef UNICODE

#define Tjson jsoncons::json
#else

#ifdef UTF8
#define Tjson jsoncons::json

#else
#define Tjson jsoncons::wjson
#endif
#endif



namespace TypicalTool {

	class TYPICALTOOL_API JsonManage {
	private:
		Tjson Value;
		Tstring JsonFilePath;

	public:
		JsonManage()
		{}
		//Json写入流配置: utf-8非英文字符显示 | 缩进 | 保存文件路径 | 读取并解析Json文件(return bool)
		JsonManage(const Tstring& _JsonFilePath, bool _IsReadJsonFile = false)
		{
			this->Init(_JsonFilePath, _IsReadJsonFile);
		}

		//Json写入流配置: utf-8非英文字符显示 | 缩进 | 保存文件路径 | 读取并解析Json文件(return bool)
		template<typename T = bool>
		bool Init(const Tstring& _JsonFilePath, bool _IsReadJsonFile) 
		{
			this->JsonFilePath = _JsonFilePath; // 设置 JSON 文件路径

			if (_IsReadJsonFile) {
				return ReadStream(this->JsonFilePath, this->Value);
			}
			return true;
		}

	private:
		template<typename T = bool>
		static bool WriteStream(const Tstring& _JsonFilePath, Tjson& _Value, std::ios::ios_base::openmode _StreamOpenMode)
		{
			Tofstream JsonFileOut(_JsonFilePath, _StreamOpenMode);
			if (!JsonFileOut.is_open()) {
				std::error_code ec(errno, std::generic_category());
				Tstring ErrorMessge = stow(ec.message());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::WriteStream: 打开Json文件失败: [%s]"), ErrorMessge));
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::WriteStream: Json文件路径: [%s]"), _JsonFilePath));
				return false;
			}
			
			try {
				// 设置输出格式
				jsoncons::basic_json_options<Tchar> options;
				options.object_array_line_splits(jsoncons::line_split_kind::new_line)
					.indent_size(4)
					.object_object_line_splits(jsoncons::line_split_kind::multi_line);  //缩进/换行
				// 将 _Value 序列化并写入文件: pretty_print/encode_json
				jsoncons::encode_json_pretty(_Value, JsonFileOut, options);
			}
			catch (const jsoncons::ser_error& e) {
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::ReadStream: 序列化 Json失败: [%s]"), e.what()));
			}
			return true;
		}

		template<typename T = bool>
		static bool ReadStream(const Tstring& _JsonFilePath, Tjson& _Value)
		{
			Tifstream JsonFile(_JsonFilePath, Tifstream::binary);
			if (!JsonFile.is_open()) {
				std::error_code ec(errno, std::generic_category());
				Tstring ErrorMessge = stow(ec.message());
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::ReadStream: 打开 Json文件失败: [%s]"), ErrorMessge));
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::ReadStream: Json文件路径: [%s]"), _JsonFilePath));
				return false;
			}
			// 读取文件内容到字符串
			Tstring tempString((std::istreambuf_iterator<Tchar>(JsonFile)),
				std::istreambuf_iterator<Tchar>());
			try {
				_Value = jsoncons::decode_json<Tjson>(tempString); // 尝试解析 JSON 字符串
				return true;
			}
			catch (const jsoncons::ser_error& e) {
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::ReadStream: 解析 Json失败: [%s]"), e.what()));
				LogRelease(LogLevelFlags::Error, Printf(TEXT("JsonManage::ReadStream: Json文件路径: [%s]"), _JsonFilePath));
				return false;
			}
			return true;
		}

	public:
		//读取 Json文件到 _Value
		template<typename T = bool>
		bool ReadJsonFile()
		{
			return ReadStream(this->JsonFilePath, this->Value);
		}
		//读取 Json文件到 _Value
		template<typename T = bool>
		bool ReadJsonFile(const Tstring& _JsonFilePath)
		{
			return ReadStream(_JsonFilePath, this->Value);
		}
		//读取 Json文件到 _Value
		template<typename T = bool>
		static bool ReadJsonFile(const Tstring& _JsonFilePath, Tjson& _JsonValue)
		{
			return ReadStream(_JsonFilePath, _JsonValue);
		}
		//写入 Value到 Json文件
		template<typename T = bool>
		bool WriteJsonFile(std::ios::ios_base::openmode _StreamOpenMode = std::ios::trunc)
		{
			return WriteStream(this->JsonFilePath, this->Value, _StreamOpenMode);
		}
		//写入 Value到 Json文件
		template<typename T = bool>
		bool WriteJsonFile(const Tstring& _JsonFilePath, std::ios::ios_base::openmode _StreamOpenMode = std::ios::trunc)
		{
			return WriteStream(_JsonFilePath, this->Value, _StreamOpenMode);
		}
		//写入 _Value到 Json文件
		template<typename T = bool>
		static bool WriteJsonFile(const Tstring& _JsonFilePath, Tjson& _Value, std::ios::ios_base::openmode _StreamOpenMode = std::ios::trunc)
		{
			return WriteStream(_JsonFilePath, _Value, _StreamOpenMode);
		}

	public:
		template<typename T = bool>
		Tjson GetJsonValue() const
		{
			return this->Value;
		}
		template<typename T = bool>
		void SetJsonValue(Tjson _Value)
		{
			this->Value = _Value;
		}
		template<typename T = bool>
		Tstring GetJsonFilePath() const
		{
			return this->JsonFilePath;
		}
		template<typename T = bool>
		void SetJsonFilePath(Tstring _JsonFilePath)
		{
			this->JsonFilePath = _JsonFilePath;
		}

	public:

		//输出 writeString到 Terr
		template<typename T = bool>
		void ToStreamString()
		{
			LogRelease(LogLevelFlags::Warning, Printf(TEXT("JsonManage::ToStreamString: JsonData to string: ")));
			Tstring JsonString;
			jsoncons::encode_json(this->Value, JsonString, jsoncons::basic_json_encode_options<Tchar>(), jsoncons::indenting::indent);
			LogRelease(LogLevelFlags::LineFeed, JsonString.c_str());

		}
	};
}






#endif