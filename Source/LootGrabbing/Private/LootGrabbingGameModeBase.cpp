// Fill out your copyright notice in the Description page of Project Settings.


#include "LootGrabbingGameModeBase.h"

#include "TypicalToolBPLib.h"
#include "TypicalTool.h"



void ALootGrabbingGameModeBase::EndPlay(const EEndPlayReason::Type)
{
    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: 游戏实例关闭开始!")));

    /*if (!GameConfig.IsValid()) {
        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: GameConfig 无效!")), FColor::Red);
    }
    else {
        GameConfig.Reset();
    }*/

    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::Shutdown: 游戏实例已关闭!")));
}

void ALootGrabbingGameModeBase::BeginPlay()
{
    //this->SetInputMode(FInputModeGameAndUI());  
    ////this->SetInputMode(FInputModeUIOnly());   // 仅UI模式（鼠标不会影响游戏）
    //this->bShowMouseCursor = true;          // 启用鼠标显示
    //this->bEnableClickEvents = true;        // 允许鼠标点击事件
    //this->bEnableMouseOverEvents = true;    // 允许鼠标悬停事件

    //World = GetWorld();
    //if (!IsValid(World)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: World 无效!")), FColor::Red);
    //}

    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: ")));
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: 游戏开始!")));

    ////初始化程序数据
    //if (!UEtytl::CreateDirectory(TEXT("Config"))) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: 文件夹创建失败/已创建!")), FColor::Red);
    //}

    //FString ErrorMessage;
    //if (!UEtytl::ReadJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
    //    if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
    //        CreateConfigFile();
    //    }
    //    else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
    //        UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: Json格式错误!")));

    //        return;
    //    }
    //}
    //UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::BeginPlay: 配置文件读取成功!")), FColor::Yellow);

    //UEtytl::DebugLog(FString::Printf(TEXT("USettingWidget::LoadSettingConfigFile: 休眠[%f]秒!"), 3.f));
    //FPlatformProcess::Sleep(3.0f); // 休眠 3 秒
}


//void ALootGrabbingGameModeBase::CreateConfigFile()
//{
//    GameConfig = MakeShareable(new FJsonObject());
//
//    //创建对象: 基本设置
//    TSharedPtr<FJsonObject> ItemObject_BaseSetting = MakeShareable(new FJsonObject());
//    //ItemObject_BaseSetting->SetBoolField(TEXT("注册表开机自启动"), false);
//
//    //添加对象到 GameConfig: 基本设置, 其他
//    GameConfig->SetObjectField(TEXT("基本设置"), ItemObject_BaseSetting);
//
//    FString ErrorMessage;
//    if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
//        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
//            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
//        }
//        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
//            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: Json格式错误!")), FColor::Red);
//        }
//        return;
//    }
//
//    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
//}
//
//void ALootGrabbingGameModeBase::SaveConfigFile()
//{
//    FString ErrorMessage;
//    if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
//        if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
//            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
//        }
//        else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
//            UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: Json格式错误!")), FColor::Red);
//        }
//
//        return;
//    }
//    UEtytl::DebugLog(FString::Printf(TEXT("APlayerControllerTools::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
//}
