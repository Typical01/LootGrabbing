// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Gacha.h"
#include "TGoods.h"
#include "ListViewBaseDataManage.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"

#include "LootGrabbingGameInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LOOTGRABBING_API ULootGrabbingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;

public:
	TSharedPtr<FJsonObject> GameConfig;

	UPROPERTY()
	TArray<UTGoods*> OrangeGoodsArray;
	UPROPERTY()
	TArray<UTGoods*> RedGoodsArray;
	UPROPERTY()
	TArray<UTGoods*> GoldGoodsArray;
	UPROPERTY()
	TArray<UTGoods*> PurpleGoodsArray;
	UPROPERTY()
	TArray<UTGoods*> BlueGoodsArray;
	UPROPERTY()
	TArray<UTGoods*> GreenGoodsArray;
	UPROPERTY()
	TArray<UTGoods*> WhiteGoodsArray;

	// 数组: 0(空), 1-7(颜色)
	/*UPROPERTY()
	TArray<UFileMediaSource*> FileSources;
	UPROPERTY()
	TArray<UMediaPlayer*> MediaPlayers;
	UPROPERTY()
	TArray<UMediaTexture*> MediaTextures;*/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileViewDataManage* TileViewDataManageContainerSlot; //瓦片视图数据处理: 容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileViewDataManage* TileViewDataManageWareHouseSlot; //瓦片视图数据处理: 仓库

public:
	// 提取 概率 数组
	// 返回 true 表示成功并写入 OutProbabilities（按顺序）
	bool ExtractProbabilities(const TSharedPtr<FJsonObject>& _GameConfig, TArray<double>& OutProbabilities);
	// 提取指定品质的物品数组
	// 返回 true 并把该品质的物品以 FItemInfo 填入 OutItems
	bool ExtractItemsByQuality(const TSharedPtr<FJsonObject>& _GameConfig, const FString& Quality, TArray<UTGoods*>& OutItems, UUserWidget* Widget);
	
public:
	void CreateConfigFile();
	void SaveConfigFile();
};
