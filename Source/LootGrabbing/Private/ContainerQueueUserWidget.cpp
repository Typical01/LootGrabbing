// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerQueueUserWidget.h"
#include "LootGrabbingGameInstance.h"

#include "TypicalToolBPLib.h"



bool UContainerQueueUserWidget::Init()
{
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: ")));
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 游戏开始!")));

	auto* tempGameInstance = GetWorld()->GetGameInstance();
	if (!IsValid(tempGameInstance)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 游戏实例获取失败!")), FColor::Red);
		return false;
	}
	LootGrabbingGameInstance = Cast<ULootGrabbingGameInstance>(tempGameInstance);
	if (!IsValid(LootGrabbingGameInstance)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 游戏实例转换<ULootGrabbingGameInstance>[%s]失败!"),
		*tempGameInstance->GetClass()->GetPathName()), FColor::Red);
		return false;
	}

    if (!IsValid(TileViewContainerSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewContainerSlot 无效!")), FColor::Red);
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileView失败!")), FColor::Red);
        return false;
    }
    if (!IsValid(LootGrabbingGameInstance->TileViewDataManageContainerSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewDataManageContainerSlot 无效!")), FColor::Red);
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileViewContainerSlot失败!")), FColor::Red);
        return false;
    }
	LootGrabbingGameInstance->TileViewDataManageContainerSlot->SetView(TileViewContainerSlot); //绑定 到ListView
    //UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileView成功!")), FColor::Green);

    if (!IsValid(TileViewWareHouseSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewWareHouseSlot 无效!")), FColor::Red);
        return false;
    }
    if (!IsValid(LootGrabbingGameInstance->TileViewDataManageWareHouseSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewDataManageWareHouseSlot 无效!")), FColor::Red);
        return false;
    }
	LootGrabbingGameInstance->TileViewDataManageWareHouseSlot->SetView(TileViewWareHouseSlot); //绑定 到ListView
    //UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileView成功!")), FColor::Green);

	GachaGame = new Gacha();
	TArrayWareHouseSlot.Init(0, WareHouseSlotSize);
	LoadSettingConfigFile();
	LootGrabbingGameInstance->ExtractProbabilities(LootGrabbingGameInstance->GameConfig, ProbabilitiesArray);

	int64 Probabilities = GachaGame->Initialize(ProbabilitiesArray);
	UEtytl::DebugLog(FString::Printf(TEXT("总概率: %lld"),
		Probabilities));
	if (Probabilities != 1LL) return false;

	TextBlockProbabilityWhite->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[0] * 100.f)));
	TextBlockProbabilityGreen->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[1] * 100.f)));
	TextBlockProbabilityBlue->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[2] * 100.f)));
	TextBlockProbabilityPurple->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[3] * 100.f)));
	TextBlockProbabilityGold->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[5] * 100.f)));
	TextBlockProbabilityRed->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[6] * 100.f)));
	TextBlockProbabilityOrange->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"),
		GachaGame->GetProbabilityDistributionGather()[7] * 100.f)));

    return true;
}

void UContainerQueueUserWidget::InitContainer()
{
	//重置状态: 格子索引/列表视图
	TArrayContainerSlot.Init(0, ContainerSlotWidth * ContainerSlotWidth);
	LootGrabbingGameInstance->TileViewDataManageContainerSlot->Empty();
	EndSlot = 0;
	GachaNum = 0;

	//填充空
	for (int32 Index = 0; Index < TArrayContainerSlot.Num(); Index++) {
		UTGoods* GoodsNull = NewObject<UTGoods>(this);
		GoodsNull->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
		LootGrabbingGameInstance->TileViewDataManageContainerSlot->Add(GoodsNull);
	}
}

int32 UContainerQueueUserWidget::StartGacha()
{
	if (!GachaGame) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::StartGacha: GachaGame无效!")), FColor::Red);
		return -1;
	}

	if (EndSlot - 1 >= GachaGame->GetRawPrize().Num()) {
		return 0;
	}

	if (Property >= ExpendProperty) {
		Property -= ExpendProperty; //消耗哈夫币
		/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::StartGacha: 抽奖成功, 剩余资产[%d]."),
			Property), FColor::Green);*/
	}
	else {
		return 0;
	}

	//int32 _Num = FMath::RandRange(1, ContainerSlotRandRangeMaxSize);
	int32 _Num = 1;
	//GachaGame->PrintProbability(); //打印概率
	GachaGame->StratGacha(_Num); //开始抽卡
	//GachaGame->PrintCountOnce();
	//GachaGame->PrintPrizeCountRaw();
	GachaNum += _Num;

	return _Num;
}

bool UContainerQueueUserWidget::FillListView(int32 _Num)
{
	if (_Num < 1) return false;

	TArray<UTGoods*> TempWareHouseSlot;
	
    for (int32 i = 0; i < _Num; i++) {
		EndSlot = i;
        UTGoods* GoodsObjectPtr = PrizeAllocation(static_cast<GachaProbability>(GachaGame->GetRawPrize()[i]), i); //根据品质分配奖励
		if (!GoodsObjectPtr) {
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: 分配物品[%d]失败!"),
				i), FColor::Red);
			continue;
		}
		/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: 分配物品[%s]成功!"),
			*GoodsObjectPtr->GetName()), FColor::Green);*/

		//仓库
		TempWareHouseSlot.Add(GoodsObjectPtr);

		for (int32 SlotIndex = i; SlotIndex < TArrayContainerSlot.Num() - i;) {
			GoodsObjectPtr->SetCurrentSlotIndex(SlotIndex);
			TArray<int32> GoodsOccupySlot = OccupySlot(GoodsObjectPtr, ContainerSlotWidth, ContainerSlotHigth);
			int32 MoveX = SlotIndex % ContainerSlotWidth;
			int32 MoveY = SlotIndex / ContainerSlotWidth;
			if (MoveGoods(GoodsObjectPtr, FVector2D(MoveX, MoveY), GoodsOccupySlot, ContainerSlotWidth, ContainerSlotHigth)) {
				if (IsContainerSlotValid(GoodsObjectPtr, GoodsOccupySlot, TArrayContainerSlot)) {
					FillSlot(GoodsObjectPtr, GoodsOccupySlot, TArrayContainerSlot, LootGrabbingGameInstance->TileViewDataManageContainerSlot);

					Property += GoodsObjectPtr->GetValue();
					WareHouseProperty += GoodsObjectPtr->GetValue();
					/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: 资产增加[%d]."),
						GoodsObjectPtr->GetValue()), FColor::Green);*/

					PutState PutState = GoodsObjectPtr->GetPutState();
					if (PutState == PutState::Vertical) {
						SlotIndex += GoodsObjectPtr->GetSlot() / GoodsObjectPtr->GetSlotLength();
					}
					else {
						SlotIndex += GoodsObjectPtr->GetSlotLength();
					}
					SlotIndex++;
					break;
				}
				else {
					//容器占用
					/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: 容器占用[%s]|位置[%d]!"),
						*GoodsObjectPtr->GetName(), GoodsObjectPtr->GetCurrentSlotIndex()), FColor::Red);*/
				}
			}
			else {
				//移动物品失败
				/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: 移动物品[%s]|位置[%d]失败!"),
					*GoodsObjectPtr->GetName(), GoodsObjectPtr->GetCurrentSlotIndex()), FColor::Red);*/
			}
			SlotIndex++;
		}
    }

	//填充空
	//for (int32 Index = LootGrabbingGameInstance->TileViewDataManageWareHouseSlot->Num(); Index < TArrayWareHouseSlot.Num(); Index++) {
	//	UTGoods* GoodsNull = NewObject<UTGoods>(this);
	//	GoodsNull->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
	//	LootGrabbingGameInstance->TileViewDataManageWareHouseSlot->Add(GoodsNull);
	//}
	////填充仓库
	//for (int32 i = 0; i < TempWareHouseSlot.Num(); i++) {
	//	UTGoods* GoodsObjectPtr = TempWareHouseSlot[i];

	//	for (int32 SlotIndex = i + WareHouseSlotEndGoodsIndex; SlotIndex < TArrayWareHouseSlot.Num() - i;) {
	//		GoodsObjectPtr->SetCurrentSlotIndex(SlotIndex);
	//		TArray<int32> GoodsOccupySlot = OccupySlot(GoodsObjectPtr, WareHouseSlotWidth, WareHouseSlotSize / WareHouseSlotWidth);
	//		int32 MoveX = SlotIndex % WareHouseSlotWidth;
	//		int32 MoveY = SlotIndex / WareHouseSlotWidth;
	//		if (MoveGoods(GoodsObjectPtr, FVector2D(MoveX, MoveY), GoodsOccupySlot, WareHouseSlotWidth, WareHouseSlotSize / WareHouseSlotWidth)) {
	//			if (IsContainerSlotValid(GoodsObjectPtr, GoodsOccupySlot, TArrayWareHouseSlot)) {
	//				FillSlot(GoodsObjectPtr, GoodsOccupySlot, TArrayWareHouseSlot, LootGrabbingGameInstance->TileViewDataManageWareHouseSlot);
	//				PutState PutState = GoodsObjectPtr->GetPutState();
	//				if (PutState == PutState::Vertical) {
	//					SlotIndex += GoodsObjectPtr->GetSlot() / GoodsObjectPtr->GetSlotLength();
	//				}
	//				else {
	//					SlotIndex += GoodsObjectPtr->GetSlotLength();
	//				}
	//				SlotIndex++;
	//				break;
	//			}
	//			else {
	//				//容器占用
	//				/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: [仓库]容器占用[%s]|位置[%d]!"),
	//					*GoodsObjectPtr->GetName(), GoodsObjectPtr->GetCurrentSlotIndex()), FColor::Red);*/
	//			}
	//		}
	//		else {
	//			//移动物品失败
	//			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: [仓库]移动物品[%s]|位置[%d]失败!"),
	//				*GoodsObjectPtr->GetName(), GoodsObjectPtr->GetCurrentSlotIndex()), FColor::Red);*/
	//		}
	//		SlotIndex++;

	//		if (SlotIndex >= TArrayWareHouseSlot.Num()) //末尾
	//			WareHouseSlotEndGoodsIndex = GoodsOccupySlot[0];
	//	}
	//}
	/*if (TileViewDataManageWareHouseSlot->GetList().IsEmpty()) {
		for (int32 i = 0; i < WareHouseSlotSize; i++) {
			UTGoods* GoodsSpace = NewObject<UTGoods>(this);
			GoodsSpace->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
			TileViewDataManageWareHouseSlot->Add(GoodsSpace);
		}
	}*/
	return true;
}

TArray<int32> UContainerQueueUserWidget::OccupySlot(UTGoods* _Goods, int32 ContainerWidth, int32 ContainerHeight)
{
	TArray<int32> OccupySlotArray;
	int32 SlotLength = _Goods->GetSlotLength();
	GoodsSlot GoodsSlot = _Goods->GetSlot();
	PutState PutState = _Goods->GetPutState();
	int32 SlotWidth = GoodsSlot / _Goods->GetSlotLength();

	switch (GoodsSlot)
	{
	case Slot_1: {
		OccupySlotArray.Add(0);
		break;
	}
	default: {
		if (PutState == PutState::Vertical) {
			for (int32 RowIndex = 0; RowIndex < SlotLength; RowIndex++) { //循环行
				for (int32 ColumnIndex = 0; ColumnIndex < SlotWidth; ColumnIndex++) { //循环列
					OccupySlotArray.Add(RowIndex * ContainerWidth + ColumnIndex);
				}
			}
		}
		else {
			for (int32 RowIndex = 0; RowIndex < SlotWidth; RowIndex++) { //循环行
				for (int32 ColumnIndex = 0; ColumnIndex < SlotLength; ColumnIndex++) { //循环列
					OccupySlotArray.Add(RowIndex * ContainerWidth + ColumnIndex);
				}
			}
		}
		break;
	}
	}

	for (int32 Index = 0; Index < OccupySlotArray.Num(); Index++) {
		//UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::OccupySlot: 占用格子[%d]."), OccupySlotArray[Index]), FColor::Red);
	}

	return OccupySlotArray;
}

bool UContainerQueueUserWidget::MoveGoods(UTGoods* _Goods, FVector2D _MoveSlot, TArray<int32>& _OccupySlot, int32 ContainerWidth, int32 ContainerHeight)
{
	if(_MoveSlot.Equals(FVector2D(0, 0))) { return true; }

	int32 SlotLength = _Goods->GetSlotLength();
	GoodsSlot GoodsSlot = _Goods->GetSlot();
	PutState PutState = _Goods->GetPutState();
	int32 SlotWidth = GoodsSlot / _Goods->GetSlotLength();

	/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::MoveGoods: 移动(X:[%f] | Y:[%f])."),
		_MoveSlot.X, _MoveSlot.Y), FColor::Red);*/

	//边界检查: 容器长/宽 - 当前物品长/宽 = 可移动的距离
	if (PutState == PutState::Vertical) {
		//列: 当前索引 + 除开索引剩余的宽度 + 移动的距离(每移动1行 * ContainerWidth)
		if ((_OccupySlot[0] + (SlotLength - 1) + _MoveSlot.Y * ContainerWidth) / 5 >= ContainerHeight) {
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::MoveGoods: Vertical 超出列边界[%d]|Max[%d]."),
				_OccupySlot[0] + (SlotLength - 1) + _MoveSlot.Y * ContainerWidth, ContainerHeight), FColor::Red);*/
			return false;
		}
		//行: 当前索引 + 除开索引剩余的宽度 + 移动的距离
		if ((_OccupySlot[0] + (SlotWidth - 1) + _MoveSlot.X) >= ContainerWidth) {
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::MoveGoods: Vertical 超出行边界[%d]|Max[%d]."),
				_OccupySlot[0] + (SlotWidth - 1) + _MoveSlot.X, ContainerWidth), FColor::Red);*/
			return false;
		}
	}
	else {
		//行: 当前索引 + 除开索引剩余的宽度 + 移动的距离
		if ((_OccupySlot[0] + (SlotLength - 1) + _MoveSlot.X) >= ContainerWidth) {
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::MoveGoods: Horizontal 超出行边界[%d]|Max[%d]."),
				_OccupySlot[0] + (SlotLength - 1) + _MoveSlot.X, ContainerWidth), FColor::Red);*/
			return false;
		}
		//列: 当前索引 + 除开索引剩余的宽度 + 移动的距离(每移动1行 * ContainerWidth)
		if ((_OccupySlot[0] + (SlotWidth - 1) + _MoveSlot.Y * ContainerWidth) / 5 >= ContainerHeight) {
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::MoveGoods: Horizontal 超出列边界[%d]|Max[%d]."), 
				_OccupySlot[0] + (SlotWidth - 1) + _MoveSlot.Y * ContainerWidth, ContainerHeight), FColor::Red);*/
			return false;
		}
	}

	for (int32 Index = 0; Index < _OccupySlot.Num(); Index++) { //循环数组: 移动元素
		_OccupySlot[Index] += _MoveSlot.X + _MoveSlot.Y * ContainerWidth;
		/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::MoveGoods: 移动到[%f]."),
			_MoveSlot.X + _MoveSlot.Y * ContainerWidth), FColor::Red);*/
	}
	return true;
}

void UContainerQueueUserWidget::FillSlot(UTGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _TArrayContainerSlot, UTileViewDataManage* _TileViewDataManage)
{
	if (_OccupySlot[0] >= _TileViewDataManage->Num() || _OccupySlot[0] >= _TArrayContainerSlot.Num()) {
		/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillSlot: 超出容器范围[%d]."),
			_OccupySlot[0]), FColor::Red);*/
		return;
	}

	_TileViewDataManage->Set(_OccupySlot[0], _Goods);
	_TArrayContainerSlot[_OccupySlot[0]] = 1;
	/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillSlot: 填充物品[%d]."),
		_OccupySlot[0]), FColor::Red);*/
	for (int32 Index = 1; Index < _OccupySlot.Num(); Index++) {
		UTGoods* GoodsSpace = NewObject<UTGoods>(this);
		GoodsSpace->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
		_TileViewDataManage->Set(_OccupySlot[Index], GoodsSpace);
		_TArrayContainerSlot[_OccupySlot[Index]] = 1;
		/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillSlot: 填充空[%d]."),
			_OccupySlot[Index]), FColor::Red);*/
	}
}

UTGoods* UContainerQueueUserWidget::PrizeAllocation(GachaProbability _GoodsGachaProbability, int32 _SlotIndex)
{
	UTGoods* GoodsObject = NewObject<UTGoods>(this);
		
	switch (_GoodsGachaProbability) {
	case GachaProbability::White: {
		GoodsObject->Copy(LootGrabbingGameInstance->WhiteGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->WhiteGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	case GachaProbability::Green: {
		GoodsObject->Copy(LootGrabbingGameInstance->GreenGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->GreenGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	case GachaProbability::Blue: {
		GoodsObject->Copy(LootGrabbingGameInstance->BlueGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->BlueGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	case GachaProbability::Purple: {
		GoodsObject->Copy(LootGrabbingGameInstance->PurpleGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->PurpleGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	case GachaProbability::Gold: {
		GoodsObject->Copy(LootGrabbingGameInstance->GoldGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->GoldGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	case GachaProbability::Red: {
		GoodsObject->Copy(LootGrabbingGameInstance->RedGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->RedGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	case GachaProbability::Orange: {
		GoodsObject->Copy(LootGrabbingGameInstance->OrangeGoodsArray[FMath::RandRange(0, LootGrabbingGameInstance->OrangeGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		break;
	}
	}
	/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LootGrabbingGameInstance->ExtractItemsByQuality: TGoods[%p] | Name[%s] | class[%s]"),
		GoodsObject->ContainerQueueUserWidget, *GoodsObject->UTGoods::GetName(), *GetNameSafe(GoodsObject->ContainerQueueUserWidget)), FColor::Green);*/
	return GoodsObject;
}

bool UContainerQueueUserWidget::IsContainerSlotValid(UTGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _ContainerArray)
{
	for (auto& temp : _OccupySlot) {
		if (temp >= _ContainerArray.Num()) {
			return false;
		}

		if (_ContainerArray[temp] == 1) {
			//UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::IsContainerSlotValid: 已占用索引[%d]."), 
			// temp), FColor::Red);
			return false;
		}
	}
	return true;
}



void UContainerQueueUserWidget::LoadSettingConfigFile()
{
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载设置的配置文件!")));

	TSharedPtr<FJsonObject> BaseSettingObject = LootGrabbingGameInstance->GameConfig->GetObjectField(TEXT("基本设置")); // 读取基本设置
	TSharedPtr<FJsonObject> GoodsObject = BaseSettingObject->GetObjectField(TEXT("物品")); // 读取备份项
	TSharedPtr<FJsonObject> QualityObject = GoodsObject->GetObjectField(TEXT("品质")); // 读取备份项

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("橙"), LootGrabbingGameInstance->OrangeGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[橙]品质物品失败!")));
	}
	//for (auto& tempGoods : OrangeGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [橙]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("红"), LootGrabbingGameInstance->RedGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[红]品质物品失败!")));
	}
	//for (auto& tempGoods : RedGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [红]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("金"), LootGrabbingGameInstance->GoldGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[金]品质物品失败!")));
	}
	//for (auto& tempGoods : GoldGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [金]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("紫"), LootGrabbingGameInstance->PurpleGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[紫]品质物品失败!")));
	}
	//for (auto& tempGoods : PurpleGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [紫]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("蓝"), LootGrabbingGameInstance->BlueGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[蓝]品质物品失败!")));
	}
	//for (auto& tempGoods : BlueGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [蓝]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("绿"), LootGrabbingGameInstance->GreenGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[绿]品质物品失败!")));
	}
	//for (auto& tempGoods : GreenGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [绿]品质物品: %s!"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}

	if (!LootGrabbingGameInstance->ExtractItemsByQuality(LootGrabbingGameInstance->GameConfig, TEXT("白"), LootGrabbingGameInstance->WhiteGoodsArray, this)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[白]品质物品失败!")));
	}
	//for (auto& tempGoods : WhiteGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [白]品质物品: %s!"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
}