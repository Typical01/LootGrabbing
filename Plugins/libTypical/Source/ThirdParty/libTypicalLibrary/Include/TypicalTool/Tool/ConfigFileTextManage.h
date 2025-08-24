#pragma once

//配置文件的字符处理
#ifndef _CONFIGFILETEXTMANAGE_H
#define _CONFIGFILETEXTMANAGE_H

#include <TypicalTool/Tool/Log.h>
#include <TypicalTool/Tool/FileManage.h>


namespace TypicalTool {
	namespace StringManage {

		//* Encode: 编码
		//写入文件
		template<class T = bool>
		bool WriteFile(const Tstring& _ConfigFilePath, std::vector<Tstring>& _WriteText)
		{
			Tofstream WriteFileStream(_ConfigFilePath, std::ios::out);

			if (!WriteFileStream) {
				LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("WriteFile: [%s]打开文件失败!"), _ConfigFilePath));
				return false;
			}
			LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("WriteFile: [%s]打开文件成功!"), _ConfigFilePath));

			for (const Tstring& tempStr : _WriteText) {
				WriteFileStream << tempStr;
			}
			LogRelease(LogLevelFlags::Info, TEXT("WriteFile: 写入完成!"));

			return true;
		}
		//* Encode: 编码
		//读取文件
		template<class T = bool>
		bool ReadFile(const Tstring& _ConfigFilePath, std::vector<Tstring>& _ReadText)
		{
			Tifstream ReadFileStream(_ConfigFilePath, std::ios::binary);

			if (!ReadFileStream) {
				LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ReadFile: [%s]打开文件失败!"), _ConfigFilePath));
				return false;
			}
			LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ReadFile: [%s]打开文件成功!"), _ConfigFilePath));

			Tstring line;
			//获取行
			while (std::getline(ReadFileStream, line)) {
				_ReadText.push_back(line); //添加到 _Text
			}
			LogRelease(LogLevelFlags::LineFeed, TEXT("ReadFile: 读取完成!"));

			return true;
		}




		/* 配置字符串格式, 基础结构:
		*	[config]\n
		*	key=value\n
		*	key2=value2\n
		*/
		enum ConfigStringFormat
		{
			Config, //配置
			ConfigItem, //配置项
			OtherChar, //控制字符: 不可显示字符
		};

		// 配置文件的字符处理
		class TYPICALTOOL_API ConfigFileTextManage
		{
		private:
			Tstring ConfigFilePath; //配置文件路径

			std::vector<Tstring> TextCache; //文本缓存

			std::map<Tstring, std::map<Tstring, Tstring>> ConfigMap; //总配置
			std::map<int32_t, Tstring> OtherCharMap; //其他非配置项

			bool ShowManageLog = false; //显示配置处理过程日志
			bool FormatText = false;

		public:
			template<typename T = bool>
			void README() const
			{
				LogRelease(LogLevelFlags::Info, TEXT("编码问题:"));
				LogRelease(LogLevelFlags::Info, TEXT("Windows: 使用之前先统一编码[Unicode(UTF-8)]: 文件编码, 控制台/其它显示输出程序 编码, 源文件编码"));
			}

			/* 输出: CMD
			* ShowManageLog: 显示配置处理过程日志
			*/
			ConfigFileTextManage(bool _ShowManageLog = true)
				: ShowManageLog(_ShowManageLog)
			{
			}

			~ConfigFileTextManage()
			{
			}

		public:
			/* 初始化
			* _Analyze: 解析文本为 配置格式
			*/
			template<typename T = bool>
			bool Init(const Tstring& _ConfigFilePath, bool _Analyze = true) {
				this->ConfigFilePath = _ConfigFilePath; //保存路径

				if (_Analyze) {
					if (!ReadConfigFile()) { //读取配置文件
						LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::Init: 读取配置文件失败!"));
						return false;
					}
					else {
						//解析文本
						if (!this->Analyze()) {
							return false;
						}
					}
				}
				else { //不解析
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::Init: 不解析文本!"));
					}
					return false;
				}

				return true;
			}
			/* 初始化
			* _ReadText: 只读取文本
			*/
			template<typename T = bool>
			bool InitText(const Tstring& _ConfigFilePath, std::vector<Tstring>& _ReadText) {
				this->ConfigFilePath = _ConfigFilePath; //保存路径

				if (!ReadConfigFile(_ReadText)) { //读取配置文件
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::InitText: 读取配置文件失败!"));
					return false;
				}

				return true;
			}

			//设置 显示处理过程的日志
			template<typename T = bool>
			void SetShowManageLog(bool _IsShowManageLog) {	this->ShowManageLog = _IsShowManageLog; }

			//获取 配置文件路径
			template<typename T = bool>
			Tstring GetConfigFilePath() { return this->ConfigFilePath; }

			//获取 其他非配置项
			template<typename T = bool>
			std::map<int32_t, Tstring> GetOtherCharMap() { return this->OtherCharMap; }

			void Clear() {
				this->ConfigFilePath.clear();
				this->TextCache.clear();
				this->ConfigMap.clear();
				this->OtherCharMap.clear();
			}

		public:
			//获取 配置项 Value
			template<typename T = bool>
			Tstring GetConfigItemValue(const Tstring& _Config, const Tstring& _ConfigItem) {
				auto tempConfig = this->ConfigMap.find(_Config);
				if (tempConfig != this->ConfigMap.end()) {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::GetConfigItemValue: 获取配置"));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::GetConfigItemValue:  配置[%s]"), _Config));
					}

					auto tempConfigItem = tempConfig->second.find(_ConfigItem);
					if (tempConfigItem != tempConfig->second.end()) {
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::GetConfigItemValue: 配置项 Key  [%s]"), _ConfigItem));
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::GetConfigItemValue: 配置项 Value[%s]"), tempConfigItem->second));
						}

						return tempConfigItem->second;
					}
					else {
						LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::GetConfigItemValue: 没有找到对应配置项!"));
						return Tstring();
					}
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::GetConfigItemValue: 没有找到对应配置!"));
					return Tstring();
				}
			}
			template<typename T = bool>
			Tstring GetConfigItemValue(std::map<Tstring, Tstring>& _Config, const Tstring& _ConfigItem) {
				auto tempConfigItem = _Config.find(_ConfigItem);
				if (tempConfigItem != _Config.end()) {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::GetConfigItemValue: 配置项 Key  [%s]"), _ConfigItem));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::GetConfigItemValue: 配置项 Value[%s]"), tempConfigItem->second));
					}

					return tempConfigItem->second;
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::GetConfigItemValue: 没有找到对应配置项!"));
					return Tstring();
				}
			}

			//获取 配置项
			template<typename T = bool>
			std::map<Tstring, Tstring> GetConfigItem(const Tstring& _Config) {
				auto tempConfig = this->ConfigMap.find(_Config);
				if (tempConfig != this->ConfigMap.end()) {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::GetConfigItem: 获取配置"));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::GetConfigItem:  配置[%s]"), _Config));
					}

					auto& tempConfigItemMap = tempConfig->second;

					//返回
					return std::map<Tstring, Tstring>(tempConfigItemMap.begin(), tempConfigItemMap.end());
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::GetConfigItem: 没有找到对应配置!"));
					return std::map<Tstring, Tstring>();
				}
			}
			//template<typename T = bool>
			//ConfigValue operator[](const Tstring& _Config) {
			//	auto tempConfig = this->ConfigMap.find(_Config);
			//	if (tempConfig != this->ConfigMap.end()) {
			//		if (this->ShowManageLog) {
			//			LogRelease(LogLevelFlags::LineFeed, TEXT("operator[]: 获取配置"));
			//			LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("  配置[%s]"), _Config));
			//		}

			//		auto& tempConfigItemMap = tempConfig->second;

			//		//返回
			//		return ConfigValue(std::make_shared<ConfigFileTextManage>(this), std::map<Tstring, Tstring>(tempConfigItemMap.begin(), tempConfigItemMap.end()));
			//	}
			//	else {
			//		LogRelease(LogLevelFlags::Warning, TEXT("operator[]: 没有找到对应配置!"));
			//		return ConfigValue();
			//	}
			//}

			//获取 全部配置
			template<typename T = bool>
			std::map<Tstring, std::map<Tstring, Tstring>> GetConfigMap() {
				return this->ConfigMap;
			}
			//输出 配置文件所有内容
			template<class T = bool>
			void OutConfigFile_All() const
			{
				LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::OutConfigFile_All: 输出内容 [%s] 开始..."), ConfigFilePath));
				for (auto tempConfig = this->ConfigMap.begin(); tempConfig != this->ConfigMap.end(); tempConfig++) {
					LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::OutConfigFile_All:  配置[%s]"), tempConfig->first));
					for (auto tempConfigText = tempConfig->second.begin(); tempConfigText != tempConfig->second.end(); tempConfigText++) {
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::OutConfigFile_All: 配置项 Key  [%s]"), tempConfigText->first));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::OutConfigFile_All: 配置项 Value[%s]"), tempConfigText->second));
					}
				}
				LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::OutConfigFile_All: 输出内容 [%s] 结束!"), ConfigFilePath));
			}

			/* 添加 配置
			* 设置格式: "config"
			* 设置项格式: "key=value"
			*/
			template<typename T = bool>
			bool AddConfig(const Tstring& _Config, std::vector<Tstring>& _ConfigItem)
			{
				//先解析为 map
				std::map<Tstring, Tstring> AnalyzeLaterConfigItemMap; //解析后的配置项 Map
				Tstring tempAnalyzeLaterConfigItem_Key; //解析后的配置项 Key
				Tstring tempAnalyzeLaterConfigItem_Value; //解析后的配置项 Value
				for (auto tempConfigItem = _ConfigItem.begin(); tempConfigItem != _ConfigItem.end(); tempConfigItem++) {
					if (!this->ConfigItemTextManage(*tempConfigItem, tempAnalyzeLaterConfigItem_Key, tempAnalyzeLaterConfigItem_Value)) {
						LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::AddConfig: 配置项文本处理失败!"));
						LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig:配置 [%s]"), _Config));
						return false;
					}
					else { //成功处理后, 临时保存
						AnalyzeLaterConfigItemMap.insert(std::make_pair(tempAnalyzeLaterConfigItem_Key, tempAnalyzeLaterConfigItem_Value));
					}
				}

				std::map<Tstring, Tstring> RepeatConfigItemMap; //重复配置 Map
				//添加有匹配的配置时, 不添加重复配置项
				auto tempConfig = this->ConfigMap.find(_Config);
				if (tempConfig != this->ConfigMap.end()) { //重复配置
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 重复配置[%s]添加配置项开始..."), _Config));
					}

					//找重复的配置项
					auto tempConfigItem = tempConfig->second;
					for (auto& ConfigItem : AnalyzeLaterConfigItemMap) {
						auto Result = tempConfigItem.find(ConfigItem.first);
						if (Result != tempConfigItem.end()) { //找到了重复项
							AnalyzeLaterConfigItemMap.erase(ConfigItem.first); //不进入之后的添加流程
						}
					}
				}
				else {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置[%s]添加配置项开始..."), _Config));
					}
				}

				//临时存放
				std::map<Tstring, Tstring> tempAddConfigItemMap;
				for (auto& ConfigItem : AnalyzeLaterConfigItemMap) {
					tempAddConfigItemMap.insert(std::make_pair(ConfigItem.first, ConfigItem.second));

					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置项 Key  [%s]"), ConfigItem.first));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置项 Value[%s]"), ConfigItem.second));
					}
				}

				//添加到 总配置
				std::pair<std::map<Tstring, std::map<Tstring, Tstring>>::iterator, bool> tempPair \
					= this->ConfigMap.emplace(std::make_pair(_Config, tempAddConfigItemMap));
				if (!tempPair.second) {
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置[%s]添加失败!"), _Config));
					return false;
				}
				else {
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置[%s]添加成功!"), _Config));
				}

				return true;
			}
			template<typename T = bool>
			bool AddConfig(const Tstring& _Config, std::map<Tstring, Tstring>& _ConfigItem)
			{
				std::map<Tstring, Tstring> RepeatConfigItemMap; //重复配置Map
				//添加有匹配的配置时, 不添加重复配置项
				auto tempConfig = this->ConfigMap.find(_Config);
				if (tempConfig != this->ConfigMap.end()) { //重复配置
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 重复配置[%s]添加配置项开始..."), _Config));
					}

					//找重复的配置项
					auto tempConfigItem = tempConfig->second;
					for (auto& ConfigItem : _ConfigItem) {
						auto Result = tempConfigItem.find(ConfigItem.first);
						if (Result != tempConfigItem.end()) { //找到了重复项
							_ConfigItem.erase(ConfigItem.first); //不进入之后的添加流程
						}
					}
				}
				else {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置[%s]添加配置项开始..."), _Config));
					}
				}

				//临时存放
				std::map<Tstring, Tstring> tempAddConfigItemMap;
				for (auto& ConfigItem : _ConfigItem) {
					tempAddConfigItemMap.insert(std::make_pair(ConfigItem.first, ConfigItem.second));

					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置项 Key  [%s]添加成功!"), ConfigItem.first));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置项 Value[%s]添加成功!"), ConfigItem.second));
					}
				}

				//添加到 总配置
				std::pair<std::map<Tstring, std::map<Tstring, Tstring>>::iterator, bool> tempPair \
					= this->ConfigMap.emplace(std::make_pair(_Config, tempAddConfigItemMap));
				if (!tempPair.second) {
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置[%s]添加失败!"), _Config));
					return false;
				}
				else {
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AddConfig: 配置[%s]添加成功!"), _Config));
				}

				return true;
			}

			//删除 配置项: 删除项时为 true, 否则为 false
			template<typename T = bool>
			bool DeleteConfigItem(const Tstring& _Config, const Tstring& _ConfigItem)
			{
				//需要删除的配置项是否匹配
				auto temp = this->ConfigMap.find(_Config); //对应配置
				if (temp != this->ConfigMap.end()) {
					if (temp->second.erase(_ConfigItem)) { //删除对应配置项
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::DeleteConfigItem: 删除配置项"));
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::DeleteConfigItem: 配置[%s]"), _Config));
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::DeleteConfigItem: 配置项[%s]"), _ConfigItem));
						}
						return true;
					}
					else {
						LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::DeleteConfigItem: 没有找到对应配置项!"));
						return false;
					}
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::DeleteConfigItem: 没有找到对应配置!"));
					return false;
				}
			}
			//删除 配置: 删除配置时为 true, 否则为 false; 需要二次确认
			template<typename T = bool>
			bool DeleteConfig(const Tstring& _Config, bool _IsDelete)
			{
				if (_IsDelete) {
					if (this->ConfigMap.erase(_Config)) {  //删除对应配置
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::DeleteConfig: 删除配置"));
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::DeleteConfig: 配置[%s]"), _Config));
						}
						return true;
					}
					else {
						LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::DeleteConfig: 没有找到对应配置!"));
						return false;
					}
				}
				else {
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::DeleteConfig: 未进行二次确认[%s]"), _Config));
					return false;
				}
			}

			//修改 配置项 Value
			template<typename T = bool>
			bool SetConfigItemValue(const Tstring& _Config, const Tstring& _ConfigItem_Key, const Tstring& _ConfigItem_Value)
			{
				auto tempConfig = this->ConfigMap.find(_Config); //对应配置
				if (tempConfig != this->ConfigMap.end()) {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::SetConfigItemValue: 修改配置项 Value"));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemValue: 配置[%s]"), _Config));
					}
					auto tempConfigItem = tempConfig->second.find(_ConfigItem_Key); //对应配置
					if (tempConfigItem != tempConfig->second.end()) {
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemValue: 配置项 Key  [%s]"), tempConfigItem->first));
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemValue: 配置项 Value[%s]"), tempConfigItem->second));
						}

						tempConfig->second.at(_ConfigItem_Key) = _ConfigItem_Value; //修改配置项

						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::SetConfigItemValue: 修改后..."));
							LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemValue: 配置项 Value[%s]"), _ConfigItem_Value));
						}
					}

					return true;
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::SetConfigItemValue: 没有找到对应配置项!"));
					return false;
				}
			}
			//修改 配置项 Key
			template<typename T = bool>
			bool SetConfigItemKey(const Tstring& _Config, const Tstring& _ConfigItem_Key, const Tstring& _ConfigItem_NewKey)
			{
				auto tempConfig = this->ConfigMap.find(_Config); //对应配置
				if (tempConfig != this->ConfigMap.end()) {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::SetConfigItemKey: 修改配置项 Key"));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemKey: 配置[%s]"), _Config));
					}
					auto tempConfigItem = tempConfig->second.find(_ConfigItem_Key); //对应配置
					if (tempConfigItem != tempConfig->second.end()) {
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemKey: 配置项 Key  [%s]"), tempConfigItem->first));
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemKey: 配置项 Value[%s]"), tempConfigItem->first));
						}

						auto tempConfigItem_OldValue = tempConfigItem->second;
						//修改配置项
						if (tempConfig->second.erase(_ConfigItem_Key)) {
							tempConfig->second.insert(std::make_pair(_ConfigItem_NewKey, tempConfigItem_OldValue));

							if (this->ShowManageLog) {
								LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::SetConfigItemKey: 修改后..."));
								LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::SetConfigItemKey: 配置项 Key[%s]"), _ConfigItem_NewKey));
							}
						}
						else {
							LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::SetConfigItemKey: 删除旧配置项 [Key] 失败! 添加新配置项 [Key] 操作被跳过!"));
							return false;
						}
					}

					return true;
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::SetConfigItemKey: 没有找到对应配置项!"));
					return false;
				}
			}
			//修改 配置
			template<typename T = bool>
			bool SetConfig(const Tstring& _Config, const Tstring& _NewConfig)
			{
				auto tempConfig = this->ConfigMap.find(_Config); //对应配置
				if (tempConfig != this->ConfigMap.end()) {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::SetConfig: 修改配置"));
						LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::SetConfig: 配置[%s]"), _Config));
					}

					//保存旧配置的配置项
					auto OldConfigMap = tempConfig->second;

					if (this->ConfigMap.erase(_Config)) { //删除旧配置
						this->ConfigMap.insert(std::make_pair(_NewConfig, OldConfigMap)); //添加新配置和保存的旧配置项

						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::Info, tytl::Printf(TEXT("ConfigFileTextManage::SetConfig: 修改配置[%s]"), _Config));
						}
					}
					else {
						LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::SetConfig: 删除旧配置[%s]"), _Config));
						return false;
					}

					return true;
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::SetConfig: 没有找到对应配置项!"));
					return false;
				}
			}

			//创建文件
#undef CreateFile
			template<typename T = bool>
			bool CreateFile()
			{
				if (!WriteFile(this->ConfigFilePath, this->TextCache)) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::CreateFile: 创建文件失败!"));
					return false;
				}

				LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::CreateFile: 创建文件成功!"));
				return true;
			}
			template<class T = bool>
			bool CreateFile(const Tstring& _NewFilePath)
			{
				if (!WriteFile(_NewFilePath, this->TextCache)) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::CreateFile: 创建文件失败!"));
					return false;
				}

				LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::CreateFile: 创建文件成功!"));
				return true;
			}

		public:
			/* 写入文件 : 格式化后的配置
			* _IsSureWrite: 强制写入
			*/
			template<class T = bool>
			bool WriteConfigFile(bool _IsSureWrite = true)
			{
				if (!this->FormatText) {
					if (!_IsSureWrite) {
						std::vector<Tstring> tempWriteConfig_Vec;
						if (this->Printf(tempWriteConfig_Vec)) {
							if (!WriteFile(this->ConfigFilePath, tempWriteConfig_Vec)) {
								LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::WriteConfigFile: 可能没有对应的文件, 或文件正在被使用!"));
								return false;
							}

							if (this->ShowManageLog) {
								if (tempWriteConfig_Vec.size() > 2)
								{
									int32_t tempOtherCharLine_Count = 1;
									LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::WriteConfigFile: 写入过程..."));
									for (auto tempBegin = tempWriteConfig_Vec.begin(); tempBegin != tempWriteConfig_Vec.end(); tempBegin++)
									{
										auto tempOtherChar = this->OtherCharMap.find(tempOtherCharLine_Count);
										if (tempOtherChar != this->OtherCharMap.end()) {
											LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::WriteConfigFile: 写入其他字符[%s]"), tempOtherChar->second));
										}
										LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::WriteConfigFile: 写入字符[%s]"), *tempBegin));

										tempOtherCharLine_Count++;
									}
								}

								LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::WriteConfigFile: 配置写入文件成功!"));
							}

							this->FormatText = true; //已经格式化
							this->TextCache = tempWriteConfig_Vec;

							return true;
						}
						else {
							LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::WriteConfigFile: 格式化配置文本失败!"));
							return false;
						}
					}
					else { //强制写入
						std::vector<Tstring> tempWriteConfig_Vec;
						this->Printf(tempWriteConfig_Vec); //无视格式化是否成功
						if (!WriteFile(this->ConfigFilePath, tempWriteConfig_Vec)) {
							LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::WriteConfigFile: 可能没有对应的文件, 或文件正在被使用!"));
							return false;
						}

						if (this->ShowManageLog) {
							if (tempWriteConfig_Vec.size() > 2)
							{
								int32_t tempOtherCharLine_Count = 1;
								LogRelease(LogLevelFlags::LineFeed, TEXT("ConfigFileTextManage::WriteConfigFile: 写入过程..."));
								for (auto tempBegin = tempWriteConfig_Vec.begin(); tempBegin != tempWriteConfig_Vec.end(); tempBegin++)
								{
									auto tempOtherChar = this->OtherCharMap.find(tempOtherCharLine_Count);
									if (tempOtherChar != this->OtherCharMap.end()) {
										LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::WriteConfigFile: 写入其他字符[%s]"), tempOtherChar->second));
									}
									LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::WriteConfigFile: 写入字符[%s]"), *tempBegin));

									tempOtherCharLine_Count++;
								}
							}

							LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::WriteConfigFile: 配置写入文件成功!"));
						}

						this->FormatText = true; //已经格式化
						this->TextCache = tempWriteConfig_Vec;

						return true;
					}
				}
				else {
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::WriteConfigFile: 已格式化配置文本!"));
					}

					if (!WriteFile(this->ConfigFilePath, this->TextCache)) {
						LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::WriteConfigFile: 可能没有对应的文件, 或文件正在被使用!"));
						return false;
					}

					return true;
				}
			}
			//写入文件: 文本
			template<class T = bool>
			bool WriteTextFile(std::vector<Tstring>& _Text)
			{
				if (!WriteFile(this->ConfigFilePath, _Text)) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::WriteTextFile: 可能没有对应的文件, 或文件正在被使用!"));
					return false;
				}
				else {
					if (this->ShowManageLog) {
						for (auto tempBegin = _Text.begin(); tempBegin != _Text.end(); tempBegin++)
						{
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::WriteTextFile: 写入字符[%s]"), *tempBegin));
						}
					}

					LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::WriteTextFile: 文本写入文件成功!"));
					return true;
				}
			}
			//读取文件: 配置
			template<class T = bool>
			bool ReadConfigFile()
			{
				//读取配置文件
				if (!ReadFile(this->ConfigFilePath, this->TextCache)) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::ReadConfigFile: 可能没有对应的文件, 或文件正在被使用!"));
					return false;
				}

				if (this->ShowManageLog) {
					for (auto tempBegin = this->TextCache.begin(); tempBegin != this->TextCache.end(); tempBegin++)
					{
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::ReadConfigFile: 读取字符[%s]"), *tempBegin));
					}
				}

				LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::ReadConfigFile: 文件读取配置成功!"));
				return true;
			}
			//读取文件: 文本
			template<class T = bool>
			bool ReadConfigFile(std::vector<Tstring>& _Text)
			{
				//读取配置文件
				if (!ReadFile(this->ConfigFilePath, _Text)) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::ReadConfigFile: 可能没有对应的文件, 或文件正在被使用!"));

					return false;
				}

				if (this->ShowManageLog) {
					for (auto tempBegin = _Text.begin(); tempBegin != _Text.end(); tempBegin++)
					{
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::ReadConfigFile: 读取字符[%s]"), *tempBegin));
					}
				}

				LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::ReadConfigFile: 文件读取配置成功!"));
				return true;
			}

		private:
			//将文本格式化为配置
			template<class T = bool>
			bool Printf(std::vector<Tstring>& _FormatMap)
			{
				bool FirstLineText = true; //第一行文本
				//int32_t TextLine = 1; //文本行数

				//示例文本: 总是在配置文件的开头
				/*auto tempFormatSampleText = this->OtherCharMap.find(0);
				if (tempFormatSampleText != this->OtherCharMap.end()) {
					_FormatMap.push_back(tempFormatSampleText->second);
				}*/

				if (!this->ConfigMap.empty())
				{
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::tytl::Printf: 格式化文本输出开始..."));
					}
					for (auto tempConfig = this->ConfigMap.begin(); tempConfig != this->ConfigMap.end(); tempConfig++)
					{
						//其他字符
						/*auto tempOtherChar = this->OtherCharMap.find(TextLine);
						if (tempOtherChar != this->OtherCharMap.end()) {
							_FormatMap.push_back(tempOtherChar->second);
							TextLine++;
						}*/

						//配置
						auto tempConfigText = TEXT("[") + tempConfig->first + TEXT("]");
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, tempConfigText.c_str());
						}
						if (FirstLineText) { //第一个配置不需要
							_FormatMap.push_back(tempConfigText + TEXT("\n")); //带换行符
							FirstLineText = false;
						}
						else { //每一个配置中间使用换行隔开
							_FormatMap.push_back(TEXT("\n"));
							_FormatMap.push_back(tempConfigText + TEXT("\n")); //带换行符
						}

						//配置项
						for (auto tempConfigItem = tempConfig->second.begin(); tempConfigItem != tempConfig->second.end(); tempConfigItem++)
						{
							auto tempText = tempConfigItem->first + TEXT("=") + tempConfigItem->second;
							if (this->ShowManageLog) {
								LogRelease(LogLevelFlags::LineFeed, tempText.c_str());
							}
							if (tempConfigItem->second.find('\n') == Tstring::npos)
							{
								_FormatMap.push_back(tempText + TEXT("\n")); //带换行符
							}
							else //有换行符
							{
								_FormatMap.push_back(tempText);
							}
						}
					}
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::tytl::Printf: 格式化文本输出结束!"));
					}

					return true;
				}
				else //空配置
				{
					_FormatMap.push_back(TEXT(""));

					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::tytl::Printf: 配置项不足以形成格式!"));
					//ConfigFormatSample();
					return false;
				}
			}
			/*
			* 解析文本内容
			* Analyze: 默认去掉换行符, 即str.size()-1
			* UTF-8 无法 str.find('\n') -> return == str::npos
			*/
			template<class T = bool>
			bool Analyze()
			{
				//配置项非空
				if (!(this->TextCache.size() < 2)) {
					Tstring AnalyzeLaterConfigItem_Key; //解析后配置项 键
					Tstring AnalyzeLaterConfigItem_Value; //解析后配置项 值
					std::map<Tstring, Tstring> AnalyzeLaterConfigItem; //解析后配置项
					Tstring NewConfig; //新的配置: 永远是最新的
					Tstring OldConfig; //旧的配置

					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::Analyze: 解析内容开始..."));
					}

					/* 配置/配置项 划分 */
					for (int32_t i = 0; i < this->TextCache.size(); i++) {
						Tstring tempAnalyzeText = this->TextCache[i]; //解析的文本
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: 内容[%s]"), tempAnalyzeText));
						}

						ConfigStringFormat StrFormat = AnalyzeFormat(tempAnalyzeText); //解析格式
						//非控制字符

						if (StrFormat == ConfigStringFormat::Config) { // 配置

							//是否到下一个Config
							if (OldConfig != NewConfig) {
								if (!AnalyzeLaterConfigItem.empty()) {
									this->ConfigMap.insert(std::make_pair(NewConfig, AnalyzeLaterConfigItem)); //保存到 配置map
									OldConfig = NewConfig; //新的配置被保存, 变成旧的配置
									AnalyzeLaterConfigItem.clear(); //保存后刷新
									NewConfig.clear();
								}
							}

							//配置格式处理
							if (this->ConfigTextManage(tempAnalyzeText)) {
								NewConfig = tempAnalyzeText;
								if (this->ShowManageLog) {
									LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: 配置[%s]"), tempAnalyzeText));
								}
							}
						}
						else if (StrFormat == ConfigStringFormat::ConfigItem) { //配置项
							if (!NewConfig.empty()) { //有配置
								//配置项处理
								if (this->ConfigItemTextManage(tempAnalyzeText, AnalyzeLaterConfigItem_Key, AnalyzeLaterConfigItem_Value)) {
									if (AnalyzeLaterConfigItem_Key.empty()) { //Key为空
										LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::Analyze: 传入的配置项[Key]格式有误!"));
										LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: Key  [%s]"), AnalyzeLaterConfigItem_Key));
									}
									if (AnalyzeLaterConfigItem_Value.empty()) { //Value为空
										LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::Analyze: 传入的配置项[Value]格式有误!"));
										LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: Value[%s]"), AnalyzeLaterConfigItem_Value));
									}

									AnalyzeLaterConfigItem.insert(std::make_pair(AnalyzeLaterConfigItem_Key, AnalyzeLaterConfigItem_Value)); //保存到 配置项map
									if (this->ShowManageLog) {
										LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: 配置项 Key  [%s]"), AnalyzeLaterConfigItem_Key));
										LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: 配置项 Value[%s]"), AnalyzeLaterConfigItem_Value));
									}
								}
							}
							else {
								//记录 其他字符
								OtherCharMap.insert(std::make_pair(i + 1, tempAnalyzeText));

								if (this->ShowManageLog) {
									LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: *其他字符串行[%s]"), ToStr(i)));
								}
							}
						}
						else if (StrFormat == ConfigStringFormat::OtherChar) {
							//记录 其他字符
							OtherCharMap.insert(std::make_pair(i + 1, tempAnalyzeText));

							if (this->ShowManageLog) {
								LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::Analyze: *其他字符串行[%s]"), ToStr(i)));
							}
						}
					}
					//最后一个配置
					this->ConfigMap.insert(std::make_pair(NewConfig, AnalyzeLaterConfigItem)); //保存到 配置map
					this->TextCache.clear();

					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::Analyze: 解析完成!"));
					}

					return true;
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::Analyze: 传入的配置项不足以形成格式!"));
					return false;
				}
			}
			/* 解析格式
			* 除最后一行没有换行符以外
			* 其他每行删除 '\n' 的字节数(1)
			*/
			template<class T = bool>
			ConfigStringFormat AnalyzeFormat(Tstring& _AnalyzeConfig)
			{
				if (!_AnalyzeConfig.empty()) {
					//首字符符号
					Tchar tempConfigCharBegin = _AnalyzeConfig[0];
					Tchar tempConfigCharEnd = TEXT(' ');
					if (_AnalyzeConfig.size() >= 3) { // Unix("[]\n") | Windows("[]\r")
						tempConfigCharEnd = _AnalyzeConfig[_AnalyzeConfig.size() - 2];
					}

					//字符串是否非法
					if (tempConfigCharBegin == TEXT('[') && tempConfigCharEnd == TEXT(']')) { //配置
						return ConfigStringFormat::Config;
					}
					else if (tempConfigCharBegin >= 0 && tempConfigCharBegin <= 31) { //控制字符: 0~31
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AnalyzeFormat: OtherChar[%s]"), _AnalyzeConfig));
						}
						return ConfigStringFormat::OtherChar;
					}
					else if (tempConfigCharBegin == 127) { //控制字符
						if (this->ShowManageLog) {
							LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::AnalyzeFormat: OtherChar[%s]"), _AnalyzeConfig));
						}
						return ConfigStringFormat::OtherChar;
					}
					else { //其他字符: 英文/中文/...
						return ConfigStringFormat::ConfigItem;
					}
				}
				else {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::AnalyzeFormat: 传入为空字符串!"));
					return ConfigStringFormat::OtherChar;
				}
			}

		private:
			//配置文本处理: 分离字符串中的中括号
			template<class T = bool>
			bool ConfigTextManage(Tstring& _Config)
			{
				//格式是否正确
				size_t tempConfigFormat_Right = _Config.find(TEXT(']')); //右括号
				if (tempConfigFormat_Right == Tstring::npos) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::ConfigTextManage: 格式错误!"));
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::ConfigTextManage: 配置文本[%s]"), _Config));

					return false;
				}

				//保存配置
				Tstring tempConfig(_Config.begin() + 1, _Config.end() - 2);

				_Config = tempConfig;

				return true;
			}
			//配置项文本处理: 分离字符串中的等号; 默认去掉换行符, 即字符串.size() - 1
			template<class T = bool>
			bool ConfigItemTextManage(Tstring& _ConfigItem, Tstring& _AnalyzeLaterConfigItem_Key, Tstring& _AnalyzeLaterConfigItem_Value)
			{
				//格式是否正确
				size_t tempEqualSign = _ConfigItem.find(TEXT('=')); //找到 "=" 号的下标
				if (tempEqualSign == Tstring::npos) {
					LogRelease(LogLevelFlags::Warning, TEXT("ConfigFileTextManage::ConfigItemTextManage: 格式错误!"));
					LogRelease(LogLevelFlags::Warning, tytl::Printf(TEXT("ConfigFileTextManage::ConfigItemTextManage: 配置项[%s]"), _ConfigItem));
					return false;
				}

				// Windows 换行处理为: \r\n
				// Unix 换行处理为: \n

				//删除文本中的换行符 '\n'
#ifdef _WINDOWS
				if (*(_ConfigItem.end() - 1) == TEXT('\n') || *(_ConfigItem.end() - 1) == TEXT('\r')) { //Windows 找到 '\n' || '\r'
					_ConfigItem.erase(_ConfigItem.end() - 1, _ConfigItem.end());
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::ConfigItemTextManage: 删除换行"));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::ConfigItemTextManage: Text[%s]"), _ConfigItem));
					}
				}
#else
				if (*(_ConfigItem.end() - 1) == '\n') { //Unix 找到 '\n'
					_ConfigItem.erase(_ConfigItem.end() - 1, _ConfigItem.end());
					if (this->ShowManageLog) {
						LogRelease(LogLevelFlags::Info, TEXT("ConfigFileTextManage::ConfigItemTextManage: 删除换行"));
						LogRelease(LogLevelFlags::LineFeed, tytl::Printf(TEXT("ConfigFileTextManage::ConfigItemTextManage: Text[%s]"), _ConfigItem));
					}
				}
#endif

				//保存 键
				Tstring tempKey(_ConfigItem.begin(), _ConfigItem.begin() + tempEqualSign); //初始化: 等于号之前
				//保存 值
				Tstring tempValue(_ConfigItem.begin() + tempEqualSign + 1, _ConfigItem.end()); //初始化: 等于号之后

				_AnalyzeLaterConfigItem_Key = tempKey;
				_AnalyzeLaterConfigItem_Value = tempValue;

				return true;
			}
		};

}
}
#endif


/*
* FileMode:
std::ios::out
以输出模式打开文件._FileText会被清除(默认行为)
std::ios::app
以追加模式打开文件.所有写入操作将追加到文件末尾, 不会清除文件的现有内容。
std::ios::trunc
以截断模式打开文件.如果文件已存在, _FileText会被清除.这个模式通常与 std::ios::out 一起使用。
std::ios::binary
以二进制模式打开文件.避免对_FileText进行文本转换(如换行符处理)
*/

/*r 以只读方式打开文件，该文件必须存在。
r+ 以可读写方式打开文件，该文件必须存在。
rb+ 读写打开一个二进制文件，只允许读写数据。
w 打开只写文件，若文件存在则文件长度清为0，即该_FileText会消失。若文件不存在则建立该文件。
w+ 打开可读写文件，若文件存在则文件长度清为0，即该_FileText会消失。若文件不存在则建立该文件。
a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
wb 只写打开或新建一个二进制文件；只允许写数据。
wb+ 读写打开或建立一个二进制文件，允许读和写。
ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
rt+ 读写打开一个文本文件，允许读和写。
wt+ 读写打开或着建立一个文本文件；允许读写。
at+ 读写打开一个文本文件，允许读或在文本末追加数据.
*/