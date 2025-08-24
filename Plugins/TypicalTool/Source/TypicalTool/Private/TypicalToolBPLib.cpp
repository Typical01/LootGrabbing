// Fill out your copyright notice in the Description page of Project Settings.


#include "TypicalToolBPLib.h"


void UTypicalToolBPLib::DebugLogs(const FString& Text, FColor TextColor)
{
	DebugLog(Text, TextColor, -1.f);
}

void UTypicalToolBPLib::DebugLog(const FString& Text, FColor TextColor, int32 Key, float LogShowTime, bool bNewOnTop, FVector2D TextScale)
{
	if (LogShowTime == -1.f) {
		LogShowTime = 3600 * 24; //显示 1 天
	}

	FString temp = FString::Printf(TEXT("[%s]%s"), *FDateTime::Now().ToString(), *Text);
	if (GEngine) {
		if (ToolsPtr.bShowScreen) {
			GEngine->AddOnScreenDebugMessage(Key, LogShowTime, TextColor, Text, bNewOnTop, TextScale);
		}

		if (TextColor == FColor::Red) {
			UE_LOG(LogTemp, Error, TEXT("%s"), *temp);
		}
		else if (TextColor == FColor::Yellow) {
			UE_LOG(LogTemp, Warning, TEXT("%s"), *temp);
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("%s"), *temp);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UTypicalToolBPLib::DebugLog: GEngine 无效!"));
	}

	if (!ToolsPtr.bInit) {
		UE_LOG(LogTemp, Error, TEXT("UTypicalToolBPLib::DebugLog: ToolsPtr 初始化失败!"));
		return;
	}
	if (!ToolsPtr.WriteTextToFile(temp)) {
		UE_LOG(LogTemp, Error, TEXT("UTypicalToolBPLib::DebugLog: 文本保存失败[%s]!"), *temp);
	}
}

bool UTypicalToolBPLib::GetCommandLineArgument(const FString& Param, FString* ParamValue)
{
	const TCHAR* CommandLine = FCommandLine::Get();
	DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::GetCommandLineArgument: 当前命令行参数 [%s]"), CommandLine), FColor::Red);

	bool bIsSwitchParam = Param.StartsWith(TEXT("-")); // 是否以 "-" 开头（开关参数格式）
	FString SearchParam = Param;

	if (bIsSwitchParam) { // 移除 "-" 前缀
		SearchParam = Param.RightChop(1); // 去掉开头的 "-"
	}

	bool bParamExists = false;  // 参数是否存在（无论哪种形式）
	bool bHasValue = false;     // 是否是键值参数（带 "=" 的格式）
	FString FoundValue;         // 存储提取的参数值

	if (bIsSwitchParam) { //检查开关参数(- Param)
		bParamExists = FParse::Param(CommandLine, *SearchParam);
		if (bParamExists) {
			DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::GetCommandLineArgument: 开关参数 [%s]"), *Param), FColor::Green);
		}
	}

	// 检查键值参数（Param=Value 或 -Param=Value）
	bHasValue = FParse::Value(CommandLine, *(SearchParam + "="), FoundValue);
	if (bHasValue) {
		bParamExists = true; // 标记参数存在
		DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::GetCommandLineArgument: 键值参数 [%s=%s]"), *SearchParam, *FoundValue), FColor::Green);

		// 如果调用方传入了 ParamValue，则填充找到的值
		if (ParamValue) {
			*ParamValue = FoundValue;
		}
	}

	// 如果参数完全不存在，记录警告
	if (!bParamExists) {
		DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::GetCommandLineArgument: 未找到参数 [%s]！"), *Param), FColor::Red);
		return false;
	}

	// 返回参数是否存在（无论是否有值）
	return bParamExists;
}

bool UTypicalToolBPLib::ReadJsonFile(const TCHAR* JsonFilePath, TSharedPtr<FJsonObject>& ParsedJsonObject, FString& ErrorMessage)
{
	// 读取文件内容
	FString InputString;
	if (!FFileHelper::LoadFileToString(InputString, JsonFilePath)) {
		ErrorMessage = TEXT("文件错误");
		DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::ReadJsonFile: 读取 JSON 文件失败! 路径: [%s]"), JsonFilePath));
		return false;
	}

	// 反序列化 JSON 字符串
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputString);
	if (!FJsonSerializer::Deserialize(Reader, ParsedJsonObject)) {
		ErrorMessage = TEXT("非文件错误");
		DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::ReadJsonFile: 解析 JSON 字符串失败! 错误: {%s}"), *Reader->GetErrorMessage()));
		return false;
	}

	return true;
}

bool UTypicalToolBPLib::WriteJsonFile(const TCHAR* JsonFilePath, TSharedPtr<FJsonObject>& ParsedJsonObject, FString& ErrorMessage)
{
	//序列化
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (!FJsonSerializer::Serialize(ParsedJsonObject.ToSharedRef(), Writer)) {
		ErrorMessage = TEXT("非文件错误");
		return false;
	}

	// 保存到文件
	if (!FFileHelper::SaveStringToFile(OutputString, JsonFilePath)) {
		ErrorMessage = TEXT("文件错误");
		UE_LOG(LogTemp, Error, TEXT("UTypicalToolBPLib::WriteJsonFile: 写入 JSON 文件失败! 路径: [%s]"), JsonFilePath);
		return false;
	}

	return true;
}

void UTypicalToolBPLib::ConsoleCommand_CloseToneMapping(APlayerController* _PlayerController) {
	if (!_PlayerController) {
		DebugLog(TEXT("ConsoleCommand_CloseToneMapping: 玩家控制器为空!"));
	}
	else {
		FString Command = TEXT("ShowFlag.Tonemapper 0");
		_PlayerController->ConsoleCommand(*Command);
		Command = TEXT("ShowFlag.Tonemapper 0");
		_PlayerController->ConsoleCommand(*Command);
		Command = TEXT("r.TonemapperGamma 0");
		_PlayerController->ConsoleCommand(*Command);
		Command = TEXT("r.TonemapperFilm 0");
		_PlayerController->ConsoleCommand(*Command);
		Command = TEXT("r.Tonemapper.Sharpen 0");
		_PlayerController->ConsoleCommand(*Command);
		Command = TEXT("r.Tonemapper.GrainQuantization 0");
		_PlayerController->ConsoleCommand(*Command);
		Command = TEXT("r.Tonemapper.Quality 0");
		_PlayerController->ConsoleCommand(*Command);
	}
}

void UTypicalToolBPLib::WindowShowAndRender(bool _bShowWindow, int32 _RenderFrameRate, bool bShowLog)
{
	// 主窗口隐藏/显示
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->GetWindow().IsValid()) {
		TSharedPtr<SWindow> MainWindow = GEngine->GameViewport->GetWindow();

		if (MainWindow.IsValid()) {
			if (_bShowWindow) {
				MainWindow->ShowWindow();
				if (bShowLog) {
					DebugLog(TEXT("UTypicalToolBPLib::WindowShowAndRender: 显示窗口!"));
				}
			}
			else {
				MainWindow->HideWindow();
				if (bShowLog) {
					DebugLog(TEXT("UTypicalToolBPLib::WindowShowAndRender: 隐藏窗口!"));
				}
			}
		}
	}

	// 暂停渲染
	static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.SetRenderingEnabled"));
	if (CVar) {
		CVar->Set(_RenderFrameRate); // 0 表示暂停渲染
	}

	// 降低帧率以减少性能消耗
	//if (GEngine)
	//{
	//    GEngine->SetMaxFPS(60); // 将帧率限制为10 FPS
	//}
}

TSharedPtr<SWindow> UTypicalToolBPLib::GetWindow()
{
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->GetWindow().IsValid()) {
		return GEngine->GameViewport->GetWindow();
	}

	return MakeShareable<SWindow>(nullptr);
}

void UTypicalToolBPLib::SetWindowResolution(int32 _Width, int32 _Height, EWindowMode::Type _Fullscreen)
{
	if (GEngine) {
		// 获取游戏用户设置
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings) {
			UserSettings->SetScreenResolution(FIntPoint(_Width, _Height)); // 设置分辨率
			UserSettings->SetFullscreenMode(_Fullscreen); // 设置全屏模式
			UserSettings->ApplySettings(false); // 应用设置(不检查命令行参数)

			DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::SetWindowResolution: Width[%d]/Height[%d]/ScreenMode[%d]!"), _Width, _Height, _Fullscreen));
		}
		else {
			DebugLog(TEXT("UTypicalToolBPLib::SetWindowResolution: 游戏用户设置无效!"));
		}
	}
}

void UTypicalToolBPLib::QuitGame(FString CallStack)
{
	//// 记录退出原因（可选）
	//UEtytl::DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::ExitGame: 请求退出游戏: [%s]"), *CallStack));

	if (GEngine) GEngine->DeferredCommands.Add(TEXT("QUIT"));

	//// 检查是否已请求退出（避免重复调用）
	//if (GIsRequestingExit) {
	//	UE_LOG(LogExit, Warning, TEXT("UTypicalToolBPLib::ExitGame: 已请求退出!"));
	//	return;
	//}
	//GIsRequestingExit = true;

	//// 安全退出（参数 false = 正常退出）
	//FGenericPlatformMisc::RequestExit(false);
}

#if defined(CreateDirectory)
#   undef CreateDirectory
#endif
#if defined(DeleteFile)
#   undef DeleteFile
#endif

bool UTypicalToolBPLib::CreateDirectory(FString _CreateFolderName, FString _TargetFolderPath)
{
	// 组合成完整的路径: 获取内容目录的路径 + 创建的文件夹名称
	FString NewFolderPath = FPaths::Combine(_TargetFolderPath, _CreateFolderName);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile(); // 获取平台文件管理器

	// 检查文件夹是否存在
	if (!PlatformFile.DirectoryExists(*NewFolderPath)) {
		bool bSuccess = (PlatformFile.CreateDirectory)(*NewFolderPath); // 创建文件夹
		if (bSuccess) {
			UEtytl::DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::CreateDirectory: 成功创建文件夹：[%s]"), *NewFolderPath), FColor::Green);

			return true;
		}
		else {
			UEtytl::DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::CreateDirectory: 无法创建文件夹：[%s]"), *NewFolderPath), FColor::Yellow);
			return false;
		}
	}
	else {
		UEtytl::DebugLog(FString::Printf(TEXT("UTypicalToolBPLib::CreateDirectory: 文件夹已存在：[%s]"), *NewFolderPath));
		return true;
	}
}
