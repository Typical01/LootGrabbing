// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "Tools.generated.h"

/**
 * 
 */
class Tools
{
public:
	FString LogFilePath { FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config")) };
	FString LogFileName { TEXT("Log_TypicalTool.log") };
	bool bInit = false;
	FCriticalSection FileWriteMutex;
	bool bShowScreen = false; //显示到屏幕上

public:
	Tools();
	~Tools();

public:
	void Init();

	bool WriteTextToFile(const FString& Text);
};
