// Fill out your copyright notice in the Description page of Project Settings.


#include "Goods.h"

#include "TypicalTool/Public/TypicalToolBPLib.h"
//#include "TypicalTool/Tool.h"


void UGoods::Init(FString _Name, FString _Quility, GoodsSlot _Slot, int32 _SlotLength, int64 _Value)
{
	Name = _Name;
	Quility = _Quility;
	Slot = _Slot;
	SlotLength = _SlotLength;
	Value = _Value;
	CurrentSlotIndex = 0;
	State = PutState::None;
	SlotOccupGoodsPtr = nullptr;
}

void UGoods::Copy(const UGoods* Data)
{
	if (!IsValid(Data)) {
		UEtytl::DebugLog(TEXT("UGoods::Copy: Data指针无效!"));
		return;
	}

	Name = Data->Name;
	Quility = Data->Quility;
	Slot = Data->Slot;
	SlotLength = Data->SlotLength;
	Value = Data->Value;
	CurrentSlotIndex = Data->CurrentSlotIndex;
	State = Data->State;
	SlotOccupGoodsPtr = Data->SlotOccupGoodsPtr;
}

void UGoods::Output()
{
	UEtytl::DebugLog(FString::Printf(TEXT("UGoods::Output: \n")));
	UEtytl::DebugLog(FString::Printf(TEXT("GoodsName: \t\t[%s]"),
		*Name));
	UEtytl::DebugLog(FString::Printf(TEXT("Quility: \t\t\t[%s]"),
		*Quility));
	UEtytl::DebugLog(FString::Printf(TEXT("Slot: \t\t\t[%s]"),
		*TransformGoodsSlotToString(Slot)));
	UEtytl::DebugLog(FString::Printf(TEXT("SlotLength: \t\t[%d]"),
		SlotLength));
	UEtytl::DebugLog(FString::Printf(TEXT("Value: \t\t\t[%d]"),
		Value));
	UEtytl::DebugLog(FString::Printf(TEXT("CurrentSlotIndex: [%d]"),
		CurrentSlotIndex));
	UEtytl::DebugLog(FString::Printf(TEXT("State: \t\t\t[%d]"),
		State));
}

bool UGoods::IsContainerSlotValid(UGoods& _Goods, TArray<int32>& _ContainerArray, int32 _ContainerWidth, const PutState& _PutState)
{
	//_ContainerWidth -= 1;
	//GoodsSlot Slot = _Goods.GetSlot();

	//switch (Slot) {
	//case GoodsSlot::Slot_1: {
	//	if (_ContainerArray[_Goods.GetCurrentSlotIndex()] != 1) { //空余
	//		_ContainerArray[_Goods.GetCurrentSlotIndex()] = 1;
	//	}
	//	else {//占用
	//		return false;
	//	}
	//}
	//case GoodsSlot::Slot_2: {
	//	if (_PutState == PutState::Horizontal) { //水平摆放: 横向2格
	//		if (_ContainerArray[_Goods.GetCurrentSlotIndex() + 1] != 1) { //空余
	//			_ContainerArray[_Goods.GetCurrentSlotIndex()] = 1;
	//			_ContainerArray[_Goods.GetCurrentSlotIndex() + 1] = 1;
	//		}
	//		else {//占用
	//			return false;
	//		}
	//	}
	//	else if (_PutState == PutState::Vertical) { //垂直摆放: 纵向2格
	//		if (_ContainerArray[_Goods.GetCurrentSlotIndex() + _ContainerWidth] != 1) { //空余
	//			_ContainerArray[_Goods.GetCurrentSlotIndex()] = 1;
	//			_ContainerArray[_Goods.GetCurrentSlotIndex() + _ContainerWidth] = 1;
	//		}
	//		else {//占用
	//			return false;
	//		}
	//	}
	//}
	//}

	return false;
}

void UGoods::GachaGame_Goods_Test(Gacha& game)
{
	game.PrintProbability(); //打印概率
	UEtytl::DebugLog(FString::Printf(TEXT("总概率: %lld"),
		game.Initialize({ 0.3, 0.3, 0.2, 0.1, 0.05, 0.045, 0.049, 0.001 })));

	int GachaCount = 10; //单次抽卡次数

	game.StratGacha(GachaCount); //开始抽卡
	game.PrintCountOnce();
	game.PrintPrizeCountRaw();
	auto tempArray = game.GetRawPrize();
	for (int32_t i = 0; i < tempArray.Num(); i++) {
		auto& tempGoods = tempArray[i];
		if (tempGoods == GachaProbability::White) {
			//std::cout << TEXT("\t酸奶 $30");
		}
		else if (tempGoods == GachaProbability::Green) {
			//std::cout << TEXT("\t铜币 $2000");
		}
		else if (tempGoods == GachaProbability::Blue) {
			//std::cout << TEXT("\t银币 $10000");
		}
		else if (tempGoods == GachaProbability::Purple) {
			//std::cout << TEXT("\t牛角 $25000");
		}
		else if (tempGoods == GachaProbability::Pink) {
			//std::cout << TEXT("\t仪式匕首 $100000");
		}
		else if (tempGoods == GachaProbability::Gold) {
			//std::cout << TEXT("\t金币 $60000");
		}
		else if (tempGoods == GachaProbability::Red) {
			//std::cout << TEXT("\t劳力士 $300000");
		}
		else if (tempGoods == GachaProbability::Orange) {
			//std::cout << TEXT("\t非洲之心 $120000000");
		}

		if (i == 5) {
			//std::cout << TEXT("\n");
		}
	}
}