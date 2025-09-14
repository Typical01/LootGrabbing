// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Gacha.h"

#include "Goods.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TYPICALTOOL_API UGoods : public UObject
{
	GENERATED_BODY()

private:
	FString Name; //物品名
	FString Quility; //品质
	GoodsSlot Slot; //格数
	int32 SlotLength; //格数: 长
	int64 Value; //价值
	int32 CurrentSlotIndex; //当前所在格数索引
	PutState State; //摆放状态
	UPROPERTY()
	UGoods* SlotOccupGoodsPtr;

public:
	UGoods() {}
	UGoods(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	void Init(FString _Name, FString _Quility, GoodsSlot _Slot, int32 _SlotLength, int64 _Value);
	void Copy(const UGoods* Data);
	void Output();

public:
	UFUNCTION(BlueprintCallable, Category = "Goods")
	FString GetName() const { return Name; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	FString GetQuility() const { return Quility; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	int64 GetValue() const { return Value; }
	GoodsSlot GetSlot() const { return Slot; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	int32 GetSlotLength() const { return SlotLength; }
	PutState GetPutState() const { return State; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	int32 GetCurrentSlotIndex() const { return CurrentSlotIndex; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	UGoods* GetSlotOccupGoodsPtr() const { return SlotOccupGoodsPtr; }

	UFUNCTION(BlueprintCallable, Category = "Goods")
	void SetName(FString _Name) { Name = _Name; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	void SetQuility(FString _Quility) { Quility = _Quility; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	void SetValue(int64 _Value) { Value = _Value; }
	void SetSlot(GoodsSlot _Slot) { Slot = _Slot; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	void SetSlotLength(int32 _SlotLength) { SlotLength = _SlotLength; }
	void SetPutState(PutState _State) { State = _State; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	void SetCurrentSlotIndex(int32 _CurrentSlotIndex) { CurrentSlotIndex = _CurrentSlotIndex; }
	UFUNCTION(BlueprintCallable, Category = "Goods")
	void SetSlotOccupGoodsPtr(UGoods* _SlotOccupGoodsPtr) { SlotOccupGoodsPtr = _SlotOccupGoodsPtr; }

	/// <summary>
	/// 返回品质索引: -1(其他) / 0(空) / 1~7(品质)
	/// </summary>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, Category = "Goods")
	int32 QuilityTransformInt();

	/// <summary>
	/// 容器槽位是否有效: 根据当前容器剩余槽位, 判断物品摆放状态是否合法
	/// </summary>
	/// <param name="_Goods">物品</param>
	/// <param name="_ContainerArray">容器数组</param>
	/// <param name="_ContainerWidth">容器宽度</param>
	/// <param name="_PutState">摆放状态</param>
	static bool IsContainerSlotValid(UGoods& _Goods, TArray<int32>& _ContainerArray, int32 _ContainerWidth, const PutState& _PutState = PutState::Horizontal);

	static void GachaGame_Goods_Test(Gacha& game);
};
