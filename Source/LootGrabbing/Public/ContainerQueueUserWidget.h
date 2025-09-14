// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LootGrabbingGameInstance.h"
#include "GoodsUserWidget.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ULootGrabbingGameInstance> LootGrabbingGameInstance; //游戏实例

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
	TArray<int32> TArrayContainerSlot;
	TArray<double> ProbabilitiesArray;
	Gacha* GachaGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GachaNum = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContainerSlotWidth = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContainerSlotHigth = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContainerSlotRandRangeMaxSize = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndSlot = 0; //索引: 添加最后一个物品的位置

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTileView* TileViewWareHouseSlot; //瓦片视图: 仓库
	TArray<int32> TArrayWareHouseSlot; //仓库: 占用情况
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WareHouseSlotSize = 1000; //仓库: 大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WareHouseSlotWidth = 10; //仓库: 宽度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WareHouseSlotEndGoodsIndex = 0; //仓库: 最后添加物品的索引

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Property = 1000000; //哈夫币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 WareHouseProperty = 0; //哈夫币: 仓库价值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 ExpendProperty = 200000; //哈夫币: 抽奖消耗

public:
	UFUNCTION(BlueprintCallable, Category = "Gacha")
	bool Init();
	//容器初始化
	UFUNCTION(BlueprintCallable, Category = "Gacha")
	void InitContainer();
	//开始抽卡
	UFUNCTION(BlueprintCallable, Category = "Gacha")
	int32 StartGacha();
	//填充列表视图
	UFUNCTION(BlueprintCallable, Category = "Gacha")
	bool FillListView(int32 _Num);
	
	//计算物品占用的格子
	TArray<int32> OccupySlot(UTGoods* _Goods, int32 ContainerWidth, int32 ContainerHeight);
	//移动物品
	bool MoveGoods(UTGoods* _Goods, FVector2D _MoveSlot, TArray<int32>& _OccupySlot, int32 ContainerWidth, int32 ContainerHeight);
	//填充物品到容器
	void FillSlot(UTGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _TArrayContainerSlot, UTileViewDataManage* _TileViewDataManage);
	//根据奖励分配对应品质的物品
	UTGoods* PrizeAllocation(GachaProbability _Goods, int32 _SlotIndex);
	//容器格子是否有效
	bool IsContainerSlotValid(UTGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _ContainerArray);

public:
	void LoadSettingConfigFile();
};
