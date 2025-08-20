// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools.h"

Tools::Tools()
{
	Init();
}

Tools::~Tools()
{
}

void Tools::Init()
{
	// 确保目录存在
	FPaths::MakeStandardFilename(LogFilePath); // 标准化路径
	FString Directory = FPaths::GetPath(LogFilePath);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*Directory)) { //路径不存在时, 创建路径树
		if (PlatformFile.CreateDirectoryTree(*Directory)) { //创建文件目录树
			UE_LOG(LogTemp, Error, TEXT("Tools::Init: 创建文件目录树失败[%s]!"), *Directory);
			UE_LOG(LogTemp, Error, TEXT("Tools::Init: 目录[%s]!"), *LogFilePath);
		}
	}

	// 截断文件
	FString FullPath = FPaths::Combine(LogFilePath, LogFileName);
	if (!PlatformFile.DeleteFile(*FullPath)) { 
		UE_LOG(LogTemp, Error, TEXT("Tools::Init: 无法截断文件[%s]!"), *FullPath);
		FFileHelper::SaveStringToFile(FString::Printf(TEXT("Tools::Init: 无法截断文件[%s]!\n"), *FullPath), *FullPath, FFileHelper::EEncodingOptions::ForceUTF8,
			&IFileManager::Get(), FILEWRITE_Append);
		return;
	}
	bInit = true;

	FFileHelper::SaveStringToFile(TEXT("Tools::Init: 初始化成功!\n"), *FullPath, FFileHelper::EEncodingOptions::ForceUTF8,
		&IFileManager::Get(), FILEWRITE_Append);
}

bool Tools::WriteTextToFile(const FString& Text)
{
	FScopeLock Lock(&FileWriteMutex); // 加锁

	// 写入文件（覆盖模式）
	return FFileHelper::SaveStringToFile(FString::Printf(TEXT("%s\n"), *Text), *FPaths::Combine(LogFilePath, LogFileName), FFileHelper::EEncodingOptions::ForceUTF8,
		&IFileManager::Get(), FILEWRITE_Append);
}
