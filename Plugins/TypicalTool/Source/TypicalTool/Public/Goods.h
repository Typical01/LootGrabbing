// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Gacha.h"

#include "Goods.generated.h"

/**
 * 
 */
UCLASS()
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
	UGoods(const FObjectInitializer&) {}

	void Init(FString _Name, FString _Quility, GoodsSlot _Slot, int32 _SlotLength, int64 _Value);
	void Copy(const UGoods* Data);
	void Output();

public:
	FString GetName() const { return Name; }
	FString GetQuility() const { return Quility; }
	int64 GetValue() const { return Value; }
	GoodsSlot GetSlot() const { return Slot; }
	int32 GetSlotLength() const { return SlotLength; }
	PutState GetPutState() const { return State; }
	int32 GetCurrentSlotIndex() const { return CurrentSlotIndex; }
	UGoods* GetSlotOccupGoodsPtr() const { return SlotOccupGoodsPtr; }

	void SetName(FString _Name) { Name = _Name; }
	void SetQuility(FString _Quility) { Quility = _Quility; }
	void SetValue(int64 _Value) { Value = _Value; }
	void SetSlot(GoodsSlot _Slot) { Slot = _Slot; }
	void SetSlotLength(int32 _SlotLength) { SlotLength = _SlotLength; }
	void SetPutState(PutState _State) { State = _State; }
	void SetCurrentSlotIndex(int32 _CurrentSlotIndex) { CurrentSlotIndex = _CurrentSlotIndex; }
	void SetSlotOccupGoodsPtr(UGoods* _SlotOccupGoodsPtr) { SlotOccupGoodsPtr = _SlotOccupGoodsPtr; }

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
