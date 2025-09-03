// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TypicalTool/Public/ListViewBaseDataManage.h"
#include "GoodsUserWidget.h"
#include "TypicalTool/Public/Gacha.h"
//#include "TypicalTool/Public/Goods.h"
#include "LootGrabbingGameModeBase.h"

#include "ContainerQueueUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOTGRABBING_API UContainerQueueUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UGoodsUserWidget> GoodsUserWidget; //物品UI

	UPROPERTY()
	TObjectPtr<ALootGrabbingGameModeBase> LootGrabbingGameModeBase;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProperty; //文本框: 哈夫币

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityOrange; //文本框: 橙
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityRed; //文本框: 红
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityGold; //文本框: 金
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityPurple; //文本框: 紫
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityBlue; //文本框: 蓝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityGreen; //文本框: 绿
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlockProbabilityWhite; //文本框: 白

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTileView* TileViewContainerSlot; //瓦片视图: 容器(保险箱)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileViewDataManage* TileViewDataManageContainerSlot; //瓦片视图数据处理: 容器
	TArray<int32> TArrayContainerSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContainerSlotWidth = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContainerSlotHigth = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContainerSlotRandRangeMaxSize = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTileView* TileViewWareHouseSlot; //瓦片视图: 仓库
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTileViewDataManage* TileViewDataManageWareHouseSlot; //瓦片视图数据处理: 仓库
	TArray<int32> TArrayWareHouseSlot; //仓库: 占用情况
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WareHouseSlotSize = 1000; //仓库: 大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WareHouseSlotWidth = 10; //仓库: 宽度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WareHouseSlotEndGoodsIndex = 0; //仓库: 最后添加物品的索引

public:
	TSharedPtr<FJsonObject> GameConfig;
	Gacha* GachaGame;
	TArray<double> ProbabilitiesArray;

	UPROPERTY()
	TArray<UGoods*> OrangeGoodsArray;
	UPROPERTY()
	TArray<UGoods*> RedGoodsArray;
	UPROPERTY()
	TArray<UGoods*> GoldGoodsArray;
	UPROPERTY()
	TArray<UGoods*> PurpleGoodsArray;
	UPROPERTY()
	TArray<UGoods*> BlueGoodsArray;
	UPROPERTY()
	TArray<UGoods*> GreenGoodsArray;
	UPROPERTY()
	TArray<UGoods*> WhiteGoodsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Property = 1000000; //哈夫币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 WareHouseProperty = 0; //哈夫币: 仓库价值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 ExpendProperty = 200000; //哈夫币: 抽奖消耗

public:
	UFUNCTION(BlueprintCallable, Category = "Gacha")
	bool Init();
	UFUNCTION(BlueprintCallable, Category = "Gacha")
	bool FillListView();
	
	//计算物品占用的格子
	TArray<int32> OccupySlot(UGoods* _Goods, int32 ContainerWidth, int32 ContainerHeight);
	//移动物品
	bool MoveGoods(UGoods* _Goods, FVector2D _MoveSlot, TArray<int32>& _OccupySlot, int32 ContainerWidth, int32 ContainerHeight);
	//填充物品到容器
	void FillSlot(UGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _TArrayContainerSlot, UTileViewDataManage* _TileViewDataManage);

	//根据奖励分配对应品质的物品
	UGoods* PrizeAllocation(GachaProbability _Goods, int32 _SlotIndex);

	bool HorizontalPut(UGoods* _Goods, TArray<int32>& _ContainerArray, bool _bIsQuit = true);
	bool VerticalPut(UGoods* _Goods, TArray<int32>& _ContainerArray, bool _bIsQuit = true);

	bool IsContainerSlotValid(UGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _ContainerArray);

public:
	bool ExtractProbabilities(const TSharedPtr<FJsonObject>& _GameConfig, TArray<double>& OutProbabilities);
	bool ExtractItemsByQuality(const TSharedPtr<FJsonObject>& _GameConfig, const FString& Quality, TArray<UGoods*>& OutItems);

	void LoadSettingConfigFile();
	void CreateConfigFile();
	void SaveConfigFile();
};
