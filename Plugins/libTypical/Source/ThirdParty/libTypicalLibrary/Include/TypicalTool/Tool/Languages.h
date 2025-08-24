#pragma once
#ifndef _LANGUAGES_H
#define _LANGUAGES_H


#include <TypicalTool/Tool/pch.h>
#include <TypicalTool/Tool/Log.h>
#include <TypicalTool/Tool/Json_Typical.h>
#include <TypicalTool/Tool/FileManage.h>


namespace TypicalTool {

	class TYPICALTOOL_API Languages {
	private:
		Tstring RootLanguagesFilePath; //根语言文件路径
		Tstring RootLanguagesParentDirectoryPath; //根语言文件的父目录路径
		Tstring DefaultLanguage; //默认语言
		Tstring CurrentLanguage; //当前语言
		Tjson RootLanguages;
		Tjson LanguageList;
		Tjson CurrentLanguageValue;
		tytl::JsonManage JsonManageLanguages;

	public:
		Languages() { }

        template<typename T = bool>
        void Initialize(const Tstring& LanguagesJsonFilePath)
		{
			RootLanguagesFilePath = LanguagesJsonFilePath;

            LoadData();
		}

    private:
        template<typename T = bool>
        void LoadData()
        {
            //获取父目录
            RootLanguagesParentDirectoryPath = tytl::FileSystem::GetParentName(RootLanguagesFilePath);

            //创建父目录
            if (!tytl::FileSystem::CreateDirectorys(RootLanguagesParentDirectoryPath)) {
                LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Initialize: 文件夹[%s]已存在!"), RootLanguagesParentDirectoryPath).ToString());
            }

            //创建/读取: Languages.json
            //文件不存在时，创建
            if (!JsonManageLanguages.Init(Printf(TEXT("%s"), RootLanguagesFilePath).ToString(), true)) {
                LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Initialize: 创建 Json文件[%s]!"), JsonManageLanguages.GetJsonFilePath()).ToString());
                LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Initialize: 配置文件初始化[%s]"), JsonManageLanguages.GetJsonFilePath()).ToString());

                // 语言设置
                Tjson jsonLanguages;
                jsonLanguages[TEXT("DefaultLanguage")] = TEXT("English");

                Tjson jsonLanguageList;

                // 中文(简体)
                Tjson jsonLanguageSimplifiedChinese;
                jsonLanguageSimplifiedChinese[TEXT("SelectLanguage")] = TEXT("选择语言");
                jsonLanguageSimplifiedChinese[TEXT("Confirm")] = TEXT("确定");
                jsonLanguageSimplifiedChinese[TEXT("Cancel")] = TEXT("取消");
                jsonLanguageSimplifiedChinese[TEXT("Save")] = TEXT("保存");
                jsonLanguageSimplifiedChinese[TEXT("Exit")] = TEXT("退出");
                jsonLanguageList[TEXT("中文(简体)")] = jsonLanguageSimplifiedChinese;

                // English
                Tjson jsonLanguageEnglish;
                jsonLanguageEnglish[TEXT("SelectLanguage")] = TEXT("Select Language");
                jsonLanguageEnglish[TEXT("Confirm")] = TEXT("Confirm");
                jsonLanguageEnglish[TEXT("Cancel")] = TEXT("Cancel");
                jsonLanguageEnglish[TEXT("Save")] = TEXT("Save");
                jsonLanguageEnglish[TEXT("Exit")] = TEXT("Exit");
                jsonLanguageList[TEXT("English")] = jsonLanguageEnglish;

                // Español
                Tjson jsonLanguageSpanish;
                jsonLanguageSpanish[TEXT("SelectLanguage")] = TEXT("Seleccionar idioma");
                jsonLanguageSpanish[TEXT("Confirm")] = TEXT("Confirmar");
                jsonLanguageSpanish[TEXT("Cancel")] = TEXT("Cancelar");
                jsonLanguageSpanish[TEXT("Save")] = TEXT("Guardar");
                jsonLanguageSpanish[TEXT("Exit")] = TEXT("Salir");
                jsonLanguageList[TEXT("Español")] = jsonLanguageSpanish;

                // Français
                Tjson jsonLanguageFrench;
                jsonLanguageFrench[TEXT("SelectLanguage")] = TEXT("Choisir la langue");
                jsonLanguageFrench[TEXT("Confirm")] = TEXT("Confirmer");
                jsonLanguageFrench[TEXT("Cancel")] = TEXT("Annuler");
                jsonLanguageFrench[TEXT("Save")] = TEXT("Enregistrer");
                jsonLanguageFrench[TEXT("Exit")] = TEXT("Quitter");
                jsonLanguageList[TEXT("Français")] = jsonLanguageFrench;

                // Deutsch
                Tjson jsonLanguageGerman;
                jsonLanguageGerman[TEXT("SelectLanguage")] = TEXT("Sprache auswählen");
                jsonLanguageGerman[TEXT("Confirm")] = TEXT("Bestätigen");
                jsonLanguageGerman[TEXT("Cancel")] = TEXT("Abbrechen");
                jsonLanguageGerman[TEXT("Save")] = TEXT("Speichern");
                jsonLanguageGerman[TEXT("Exit")] = TEXT("Beenden");
                jsonLanguageList[TEXT("Deutsch")] = jsonLanguageGerman;

                // 日本語
                Tjson jsonLanguageJapanese;
                jsonLanguageJapanese[TEXT("SelectLanguage")] = TEXT("言語を選択");
                jsonLanguageJapanese[TEXT("Confirm")] = TEXT("確認");
                jsonLanguageJapanese[TEXT("Cancel")] = TEXT("キャンセル");
                jsonLanguageJapanese[TEXT("Save")] = TEXT("保存");
                jsonLanguageJapanese[TEXT("Exit")] = TEXT("終了");
                jsonLanguageList[TEXT("日本語")] = jsonLanguageJapanese;

                // 한국어
                Tjson jsonLanguageKorean;
                jsonLanguageKorean[TEXT("SelectLanguage")] = TEXT("언어 선택");
                jsonLanguageKorean[TEXT("Confirm")] = TEXT("확인");
                jsonLanguageKorean[TEXT("Cancel")] = TEXT("취소");
                jsonLanguageKorean[TEXT("Save")] = TEXT("저장");
                jsonLanguageKorean[TEXT("Exit")] = TEXT("종료");
                jsonLanguageList[TEXT("한국어")] = jsonLanguageKorean;

                // Português
                Tjson jsonLanguagePortuguese;
                jsonLanguagePortuguese[TEXT("SelectLanguage")] = TEXT("Selecionar idioma");
                jsonLanguagePortuguese[TEXT("Confirm")] = TEXT("Confirmar");
                jsonLanguagePortuguese[TEXT("Cancel")] = TEXT("Cancelar");
                jsonLanguagePortuguese[TEXT("Save")] = TEXT("Salvar");
                jsonLanguagePortuguese[TEXT("Exit")] = TEXT("Sair");
                jsonLanguageList[TEXT("Português")] = jsonLanguagePortuguese;

                // Русский
                Tjson jsonLanguageRussian;
                jsonLanguageRussian[TEXT("SelectLanguage")] = TEXT("Выбрать язык");
                jsonLanguageRussian[TEXT("Confirm")] = TEXT("Подтвердить");
                jsonLanguageRussian[TEXT("Cancel")] = TEXT("Отмена");
                jsonLanguageRussian[TEXT("Save")] = TEXT("Сохранить");
                jsonLanguageRussian[TEXT("Exit")] = TEXT("Выход");
                jsonLanguageList[TEXT("Русский")] = jsonLanguageRussian;

                // العربية
                Tjson jsonLanguageArabic;
                jsonLanguageArabic[TEXT("SelectLanguage")] = TEXT("اختر اللغة");
                jsonLanguageArabic[TEXT("Confirm")] = TEXT("تأكيد");
                jsonLanguageArabic[TEXT("Cancel")] = TEXT("إلغاء");
                jsonLanguageArabic[TEXT("Save")] = TEXT("حفظ");
                jsonLanguageArabic[TEXT("Exit")] = TEXT("خروج");
                jsonLanguageList[TEXT("العربية")] = jsonLanguageArabic;

                // हिन्दी
                Tjson jsonLanguageHindi;
                jsonLanguageHindi[TEXT("SelectLanguage")] = TEXT("भाषा चुनें");
                jsonLanguageHindi[TEXT("Confirm")] = TEXT("पुष्टि करें");
                jsonLanguageHindi[TEXT("Cancel")] = TEXT("रद्द करें");
                jsonLanguageHindi[TEXT("Save")] = TEXT("सहेजें");
                jsonLanguageHindi[TEXT("Exit")] = TEXT("बाहर निकलें");
                jsonLanguageList[TEXT("हिन्दी")] = jsonLanguageHindi;

                // Türkçe
                Tjson jsonLanguageTurkish;
                jsonLanguageTurkish[TEXT("SelectLanguage")] = TEXT("Dil Seçin");
                jsonLanguageTurkish[TEXT("Confirm")] = TEXT("Onayla");
                jsonLanguageTurkish[TEXT("Cancel")] = TEXT("İptal");
                jsonLanguageTurkish[TEXT("Save")] = TEXT("Kaydet");
                jsonLanguageTurkish[TEXT("Exit")] = TEXT("Çıkış");
                jsonLanguageList[TEXT("Türkçe")] = jsonLanguageTurkish;

                jsonLanguages[TEXT("LanguageList")] = jsonLanguageList;

                JsonManageLanguages.SetJsonValue(jsonLanguages);
                JsonManageLanguages.WriteJsonFile();
            }
            else {
                LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Initialize: 读取文件[%s]成功!"), JsonManageLanguages.GetJsonFilePath()));
                LogRelease(LogLevelFlags::Info, TEXT("Languages::Initialize: JsonManageLanguages ToStreamString Start:"));
                JsonManageLanguages.ToStreamString();
                LogRelease(LogLevelFlags::Info, TEXT("Languages::Initialize: JsonManageLanguages ToStreamString End!"));

                RootLanguages = JsonManageLanguages.GetJsonValue();
                if (RootLanguages.is_object()) {
                    if (RootLanguages.contains(TEXT("DefaultLanguage"))) {
                        DefaultLanguage = RootLanguages[TEXT("DefaultLanguage")].as<Tstring>();
                    }
                    if (RootLanguages.contains(TEXT("LanguageList"))) {
                        LanguageList = RootLanguages[TEXT("LanguageList")];
                    }

                    if (LanguageList.is_object() && LanguageList.contains(DefaultLanguage)) {
                        CurrentLanguageValue = LanguageList[DefaultLanguage];
                        CurrentLanguage = LanguageList[DefaultLanguage].as<Tstring>();
                    }
                }
                else {
                    LogRelease(LogLevelFlags::Error, TEXT("Languages::Initialize: RootLanguages 中没有 Json 对象!"));
                }

                Output();
            }
        }

        template<typename T = bool>
        void DataToString(Tstring& DataName, Tstring& DataString)
        {
            if (!CurrentLanguageValue.is_object()) {
                return;
            }

            for (auto& member : CurrentLanguageValue.object_range())
            {
                DataName += Printf(TEXT("%s\n%s"), DataName, member.key());
                DataString += Printf(TEXT("%s\n%s"), DataString, member.value().as<Tstring>());
                LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::SavaData: DataName     [%s]"), member.key()).ToString());
                LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::SavaData: DataString   [%s]"), member.value().as<Tstring>()).ToString());
            }
        }

        template<typename T = bool>
        bool StringToData(const Tstring& LanguageName, const Tstring& DataName, const Tstring& DataString) {
            if (!LanguageList.is_object()) {
                return false;
            }

            // 分割 DataName 和 DataString 为列表
            std::vector<Tstring> DataNameList = tytl::StringManage::SplitString(DataName, TEXT("\n"));
            std::vector<Tstring> DataStringList = tytl::StringManage::SplitString(DataString, TEXT("\n"));

            if (DataNameList.size() != DataStringList.size()) {
                LogRelease(LogLevelFlags::Info, Printf(TEXT("StringToData: DataNameList size [%s] != DataStringList size [%s]"),
                    DataNameList.size(), DataStringList.size()).ToString());
                return false;
            }

            Tjson LanguageValue;
            for (size_t i = 0; i < DataNameList.size(); ++i) {
                const Tstring& Key = DataNameList[i];
                const Tstring& Value = DataStringList[i];
                LanguageValue[Key] = Value;
            }
            LanguageList[LanguageName] = LanguageValue;

            return true;
        }

        template<typename T = bool>
        void WriteLanguagesFile(const Tjson& Root = jsoncons::json_type::object_value)
        {
            if (!Root.empty()) {
                JsonManageLanguages.SetJsonFilePath(Root.as<Tstring>());
            }
            else {
                JsonManageLanguages.SetJsonFilePath(this->RootLanguages.as<Tstring>());
            }
            JsonManageLanguages.WriteJsonFile();
        }

        template<typename T = bool>
        void Output()
        {
            LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Output: DefaultLanguage    [%s]"), DefaultLanguage).ToString());
            LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Output: CurrentLanguage    [%s]"), CurrentLanguage).ToString());
            LogRelease(LogLevelFlags::Info, Printf(TEXT("Languages::Output: LanguageList Size  [%s]"), ToStr(LanguageList.size())).ToString());
        }

	public:
        template<typename T = bool>
        bool SetCurrentLanguage(const Tchar* SelectLanguage)
        {
            if (!LanguageList.is_object()) {
                return false;
            }
            if (!LanguageList.contains(SelectLanguage)) {
                return false;
            }

            CurrentLanguageValue = LanguageList[SelectLanguage];
            return true;
        }

        template<typename T = bool>
		Tjson GetRootLanguages() const { return this->RootLanguages; }
        template<typename T = bool>
		Tjson GetLanguageList() const { return this->LanguageList; }
        template<typename T = bool>
		Tjson GetCurrentLanguageValue() const { return this->CurrentLanguageValue; }
        template<typename T = bool>
		tytl::JsonManage GetJsonManage() const { return this->JsonManageLanguages; }
        template<typename T = bool>
		Tstring GetLanguagesFilePath() const { return this->RootLanguagesFilePath; }
        template<typename T = bool>
		Tstring GetLanguagesParentDirectoryPath() const { return this->RootLanguagesParentDirectoryPath; }
        template<typename T = bool>
		Tstring GetDefaultLanguage() const { return this->DefaultLanguage; }
        template<typename T = bool>
		Tstring GetCurrentLanguage() const { return this->CurrentLanguage; }
	};
}



#endif