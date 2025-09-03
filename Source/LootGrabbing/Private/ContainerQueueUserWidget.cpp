// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerQueueUserWidget.h"

#include "TypicalToolBPLib.h"



bool UContainerQueueUserWidget::Init()
{
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: ")));
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 游戏开始!")));

	//初始化程序数据
	if (!UEtytl::CreateDirectory(TEXT("Config"))) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 文件夹创建失败/已创建!")), FColor::Red);
	}

	FString ErrorMessage;
	if (!UEtytl::ReadJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
		if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
			CreateConfigFile();
		}
		else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: Json格式错误!")));
			return false;
		}
	}
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 配置文件读取成功!")), FColor::Yellow);

    TileViewDataManageContainerSlot = NewObject<UTileViewDataManage>();
    if (!IsValid(TileViewContainerSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewContainerSlot 无效!")), FColor::Red);
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileView失败!")), FColor::Red);
        return false;
    }
    if (!IsValid(TileViewDataManageContainerSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewDataManageContainerSlot 无效!")), FColor::Red);
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileViewContainerSlot失败!")), FColor::Red);
        return false;
    }
    TileViewDataManageContainerSlot->SetView(TileViewContainerSlot); //绑定 到ListView
    //UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileView成功!")), FColor::Green);

    TileViewDataManageWareHouseSlot = NewObject<UTileViewDataManage>();
    if (!IsValid(TileViewWareHouseSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewWareHouseSlot 无效!")), FColor::Red);
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileViewWareHouseSlot失败!")), FColor::Red);
        return false;
    }
    if (!IsValid(TileViewDataManageWareHouseSlot)) {
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: TileViewDataManageWareHouseSlot 无效!")), FColor::Red);
        UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileViewWareHouseSlot失败!")), FColor::Red);
        return false;
    }
    TileViewDataManageWareHouseSlot->SetView(TileViewWareHouseSlot); //绑定 到ListView
    //UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::Initialize: 绑定 TileView成功!")), FColor::Green);

	GachaGame = new Gacha();
	TArrayWareHouseSlot.Init(0, WareHouseSlotSize);
	LoadSettingConfigFile();
	ExtractProbabilities(GameConfig, ProbabilitiesArray);

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

bool UContainerQueueUserWidget::FillListView()
{
	if (Property >= ExpendProperty) {
		Property -= ExpendProperty; //消耗哈夫币
	}
	else {
		return false;
	}
	TArrayContainerSlot.Init(0, ContainerSlotWidth * ContainerSlotWidth);
	TileViewDataManageContainerSlot->Empty();
	TArray<UGoods*> TempWareHouseSlot;

	//填充空白
	for (int32 Index = 0; Index < TArrayContainerSlot.Num(); Index++) {
		UGoods* GoodsNull = NewObject<UGoods>(this);
		GoodsNull->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
		TileViewDataManageContainerSlot->Add(GoodsNull);
	}

	int32 _Num = FMath::RandRange(1, ContainerSlotRandRangeMaxSize);
	//GachaGame->PrintProbability(); //打印概率
	GachaGame->StratGacha(_Num); //开始抽卡
	//GachaGame->PrintCountOnce();
	//GachaGame->PrintPrizeCountRaw();
	
	auto tempPrizeArray = GachaGame->GetRawPrize();
	int32 EndSlot = 0;
    for (int32 i = 0; i < _Num; i++) {
        UGoods* GoodsObjectPtr = PrizeAllocation(static_cast<GachaProbability>(tempPrizeArray[i]), i); //根据品质分配奖励
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
			EndSlot = SlotIndex;
			GoodsObjectPtr->SetCurrentSlotIndex(SlotIndex);
			TArray<int32> GoodsOccupySlot = OccupySlot(GoodsObjectPtr, ContainerSlotWidth, ContainerSlotHigth);
			int32 MoveX = SlotIndex % ContainerSlotWidth;
			int32 MoveY = SlotIndex / ContainerSlotWidth;
			if (MoveGoods(GoodsObjectPtr, FVector2D(MoveX, MoveY), GoodsOccupySlot, ContainerSlotWidth, ContainerSlotHigth)) {
				if (IsContainerSlotValid(GoodsObjectPtr, GoodsOccupySlot, TArrayContainerSlot)) {
					FillSlot(GoodsObjectPtr, GoodsOccupySlot, TArrayContainerSlot, TileViewDataManageContainerSlot);

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

	//填充空白
	for (int32 Index = TileViewDataManageWareHouseSlot->Num(); Index < TArrayWareHouseSlot.Num(); Index++) {
		UGoods* GoodsNull = NewObject<UGoods>(this);
		GoodsNull->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
		TileViewDataManageWareHouseSlot->Add(GoodsNull);
	}
	//填充仓库
	for (int32 i = 0; i < TempWareHouseSlot.Num(); i++) {
		UGoods* GoodsObjectPtr = TempWareHouseSlot[i];

		for (int32 SlotIndex = i + WareHouseSlotEndGoodsIndex; SlotIndex < TArrayWareHouseSlot.Num() - i;) {
			EndSlot = SlotIndex;
			GoodsObjectPtr->SetCurrentSlotIndex(SlotIndex);
			TArray<int32> GoodsOccupySlot = OccupySlot(GoodsObjectPtr, WareHouseSlotWidth, WareHouseSlotSize / WareHouseSlotWidth);
			int32 MoveX = SlotIndex % WareHouseSlotWidth;
			int32 MoveY = SlotIndex / WareHouseSlotWidth;
			if (MoveGoods(GoodsObjectPtr, FVector2D(MoveX, MoveY), GoodsOccupySlot, WareHouseSlotWidth, WareHouseSlotSize / WareHouseSlotWidth)) {
				if (IsContainerSlotValid(GoodsObjectPtr, GoodsOccupySlot, TArrayWareHouseSlot)) {
					FillSlot(GoodsObjectPtr, GoodsOccupySlot, TArrayWareHouseSlot, TileViewDataManageWareHouseSlot);
					WareHouseSlotEndGoodsIndex = GoodsOccupySlot[0];
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
					/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: [仓库]容器占用[%s]|位置[%d]!"),
						*GoodsObjectPtr->GetName(), GoodsObjectPtr->GetCurrentSlotIndex()), FColor::Red);*/
				}
			}
			else {
				//移动物品失败
				/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillListView: [仓库]移动物品[%s]|位置[%d]失败!"),
					*GoodsObjectPtr->GetName(), GoodsObjectPtr->GetCurrentSlotIndex()), FColor::Red);*/
			}
			SlotIndex++;
		}
	}
	/*if (TileViewDataManageWareHouseSlot->GetList().IsEmpty()) {
		for (int32 i = 0; i < WareHouseSlotSize; i++) {
			UGoods* GoodsSpace = NewObject<UGoods>(this);
			GoodsSpace->Init(TEXT("空"), TEXT("空"), GoodsSlot::Slot_1, 1, -1);
			TileViewDataManageWareHouseSlot->Add(GoodsSpace);
		}
	}*/
	return true;
}

TArray<int32> UContainerQueueUserWidget::OccupySlot(UGoods* _Goods, int32 ContainerWidth, int32 ContainerHeight)
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

bool UContainerQueueUserWidget::MoveGoods(UGoods* _Goods, FVector2D _MoveSlot, TArray<int32>& _OccupySlot, int32 ContainerWidth, int32 ContainerHeight)
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

void UContainerQueueUserWidget::FillSlot(UGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _TArrayContainerSlot, UTileViewDataManage* _TileViewDataManage)
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
		UGoods* GoodsSpace = NewObject<UGoods>(this);
		GoodsSpace->Init(TEXT("空白"), TEXT("空白"), GoodsSlot::Slot_1, 1, -1);
		_TileViewDataManage->Set(_OccupySlot[Index], GoodsSpace);
		_TArrayContainerSlot[_OccupySlot[Index]] = 1;
		/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::FillSlot: 填充空白[%d]."),
			_OccupySlot[Index]), FColor::Red);*/
	}
}

UGoods* UContainerQueueUserWidget::PrizeAllocation(GachaProbability _GoodsGachaProbability, int32 _SlotIndex)
{
	UGoods* GoodsObject = NewObject<UGoods>(this);
	switch (_GoodsGachaProbability) {
	case GachaProbability::White: {
		GoodsObject->Copy(WhiteGoodsArray[FMath::RandRange(0, WhiteGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	case GachaProbability::Green: {
		GoodsObject->Copy(GreenGoodsArray[FMath::RandRange(0, GreenGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	case GachaProbability::Blue: {
		GoodsObject->Copy(BlueGoodsArray[FMath::RandRange(0, BlueGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	case GachaProbability::Purple: {
		GoodsObject->Copy(PurpleGoodsArray[FMath::RandRange(0, PurpleGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	case GachaProbability::Gold: {
		GoodsObject->Copy(GoldGoodsArray[FMath::RandRange(0, GoldGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	case GachaProbability::Red: {
		GoodsObject->Copy(RedGoodsArray[FMath::RandRange(0, RedGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	case GachaProbability::Orange: {
		GoodsObject->Copy(OrangeGoodsArray[FMath::RandRange(0, OrangeGoodsArray.Num() - 1)]);
		GoodsObject->SetCurrentSlotIndex(_SlotIndex);
		return GoodsObject;
	}
	}
	return GoodsObject;
}

bool UContainerQueueUserWidget::IsContainerSlotValid(UGoods* _Goods, TArray<int32>& _OccupySlot, TArray<int32>& _ContainerArray)
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

// 提取 概率 数组
// 返回 true 表示成功并写入 OutProbabilities（按顺序）
bool UContainerQueueUserWidget::ExtractProbabilities(const TSharedPtr<FJsonObject>& _GameConfig, TArray<double>& OutProbabilities)
{
	OutProbabilities.Empty();
	if (!_GameConfig.IsValid() || !_GameConfig->HasField(TEXT("基本设置")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractProbabilities: Json对象没有[基本设置]!")), FColor::Red);
		return false;
	}

	TSharedPtr<FJsonObject> BaseSetting = _GameConfig->GetObjectField(TEXT("基本设置"));
	if (!BaseSetting.IsValid() || !BaseSetting->HasField(TEXT("概率")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractProbabilities: Json对象没有[概率]!")), FColor::Red);
		return false;
	}

	// 读取数组
	TArray<TSharedPtr<FJsonValue>> ProbArray = BaseSetting->GetArrayField(TEXT("概率"));
	int32 Index = 0;
	for (const TSharedPtr<FJsonValue>& Val : ProbArray)
	{
		if (!Val.IsValid()) {
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractProbabilities: Json对象没有[概率][%d]!"),
				Index), FColor::Red);
		}
		else {
			OutProbabilities.Add(Val->AsNumber());
		}
		Index++;
	}

	return true;
}

// 提取指定品质的物品数组
// 返回 true 并把该品质的物品以 FItemInfo 填入 OutItems
bool UContainerQueueUserWidget::ExtractItemsByQuality(const TSharedPtr<FJsonObject>& _GameConfig, const FString& Quality, TArray<UGoods*>& OutItems)
{
	OutItems.Empty();
	if (!_GameConfig.IsValid() || !_GameConfig->HasField(TEXT("基本设置")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: Json对象没有[基本设置]!")), FColor::Red);
		return false;
	}

	TSharedPtr<FJsonObject> BaseSetting = _GameConfig->GetObjectField(TEXT("基本设置"));
	if (!BaseSetting.IsValid() || !BaseSetting->HasField(TEXT("物品")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: Json对象没有[物品]!")), FColor::Red);
		return false;
	}

	TSharedPtr<FJsonObject> GoodsObject = BaseSetting->GetObjectField(TEXT("物品"));
	if (!GoodsObject.IsValid() || !GoodsObject->HasField(Quality))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: Json对象[物品]没有[%s]!"), *Quality), FColor::Red);
		return false;
	}
	//TSharedPtr<FJsonObject> QualityObject = GoodsObject->GetObjectField(TEXT("品质"));
	TArray<TSharedPtr<FJsonValue>> ItemArray = GoodsObject->GetArrayField(Quality);
	for (const TSharedPtr<FJsonValue>& Val : ItemArray)
	{
		if (!Val.IsValid() || !Val->AsObject().IsValid()) continue;

		TSharedPtr<FJsonObject> ItemObj = Val->AsObject();
		FString GoodsName;
		int32 GoodsSlotNum = 0;
		int32 GoodsSlotLength = 0;
		int64 GoodsValue = 0;
		PutState GoodsPutState = PutState::None;

		if (ItemObj->HasField(TEXT("物品名称")))
		{
			GoodsName = ItemObj->GetStringField(TEXT("物品名称")); 
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: 物品名称[%s]"), 
				*GoodsName), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品格数")))
		{
			GoodsSlotNum = FMath::RoundToInt32(ItemObj->GetNumberField(TEXT("物品格数")));
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: 物品格数[%lld]"),
				GoodsSlotNum), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品格数长")))
		{
			GoodsSlotLength = FMath::RoundToInt32(ItemObj->GetNumberField(TEXT("物品格数长")));
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: 物品格数长[%d]"),
				GoodsSlotLength), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品摆放")))
		{
			GoodsPutState = static_cast<PutState>(FMath::RoundToInt32(ItemObj->GetNumberField(TEXT("物品摆放"))));
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: 物品摆放[%d]"),
				GoodsPutState), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品价值")))
		{
			GoodsValue = FMath::RoundToInt64(ItemObj->GetNumberField(TEXT("物品价值")));
			/*UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::ExtractItemsByQuality: 物品价值[%lld]"),
				GoodsValue), FColor::Green);*/
		}

		UGoods* _Goods = NewObject<UGoods>(this);
		_Goods->Init(GoodsName, Quality, static_cast<GoodsSlot>(GoodsSlotNum), GoodsSlotLength, GoodsValue);
		_Goods->SetPutState(GoodsPutState);
		OutItems.Add(_Goods);
	}

	return true;
}

void UContainerQueueUserWidget::LoadSettingConfigFile()
{
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载设置的配置文件!")));

	TSharedPtr<FJsonObject> BaseSettingObject = GameConfig->GetObjectField(TEXT("基本设置")); // 读取基本设置
	TSharedPtr<FJsonObject> GoodsObject = BaseSettingObject->GetObjectField(TEXT("物品")); // 读取备份项
	TSharedPtr<FJsonObject> QualityObject = GoodsObject->GetObjectField(TEXT("品质")); // 读取备份项
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("橙"), OrangeGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[橙]品质物品失败!")));
	}
	//for (auto& tempGoods : OrangeGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [橙]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("红"), RedGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[红]品质物品失败!")));
	}
	//for (auto& tempGoods : RedGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [红]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("金"), GoldGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[金]品质物品失败!")));
	}
	//for (auto& tempGoods : GoldGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [金]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("紫"), PurpleGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[紫]品质物品失败!")));
	}
	//for (auto& tempGoods : PurpleGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [紫]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("蓝"), BlueGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[蓝]品质物品失败!")));
	}
	//for (auto& tempGoods : BlueGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [蓝]品质物品: %s"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("绿"), GreenGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[绿]品质物品失败!")));
	}
	//for (auto& tempGoods : GreenGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [绿]品质物品: %s!"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
	
	if (!ExtractItemsByQuality(GameConfig, TEXT("白"), WhiteGoodsArray)) {
		UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: 加载[白]品质物品失败!")));
	}
	//for (auto& tempGoods : WhiteGoodsArray) {
	//	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::LoadSettingConfigFile: [白]品质物品: %s!"), *tempGoods->GetName()));
	//	//tempGoods->Output();
	//}
}

void UContainerQueueUserWidget::CreateConfigFile()
{
	GameConfig = MakeShareable(new FJsonObject());

	// 品质对象（每个键 "红"/"金" 对应一个数组）
	TSharedPtr<FJsonObject> QualityObject = MakeShared<FJsonObject>();

	// 橙 品质数组
	TArray<TSharedPtr<FJsonValue>> OrangeArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("非洲之心"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 13000000);
		OrangeArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("海洋之泪"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 20000000);
		OrangeArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("橙"), OrangeArray);

	// 红 品质数组
	TArray<TSharedPtr<FJsonValue>> RedArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("万足金条"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_2);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品摆放"), PutState::Vertical);
		item->SetNumberField(TEXT("物品价值"), 350000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("名贵机械表"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 300000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("主战坦克模型"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_9);
		item->SetNumberField(TEXT("物品格数长"), 3);
		item->SetNumberField(TEXT("物品价值"), 3000000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("步战车模型"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_6);
		item->SetNumberField(TEXT("物品格数长"), 3);
		item->SetNumberField(TEXT("物品摆放"), PutState::Horizontal);
		item->SetNumberField(TEXT("物品价值"), 1500000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("滑膛枪展示品"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_4);
		item->SetNumberField(TEXT("物品格数长"), 4);
		item->SetNumberField(TEXT("物品摆放"), PutState::Horizontal);
		item->SetNumberField(TEXT("物品价值"), 800000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("化石"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_2);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品摆放"), PutState::Horizontal);
		item->SetNumberField(TEXT("物品价值"), 400000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("黄金瞪羚"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_4);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品价值"), 450000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("克劳迪乌斯半身像"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_6);
		item->SetNumberField(TEXT("物品格数长"), 3);
		item->SetNumberField(TEXT("物品摆放"), PutState::Vertical);
		item->SetNumberField(TEXT("物品价值"), 1800000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("雷斯的留声机"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_6);
		item->SetNumberField(TEXT("物品格数长"), 3);
		item->SetNumberField(TEXT("物品摆放"), PutState::Vertical);
		item->SetNumberField(TEXT("物品价值"), 1600000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("赛伊德的怀表"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 200000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("天圆地方"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_4);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品价值"), 850000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("万金泪冠"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_9);
		item->SetNumberField(TEXT("物品格数长"), 3);
		item->SetNumberField(TEXT("物品价值"), 2500000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("纵横"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_9);
		item->SetNumberField(TEXT("物品格数长"), 3);
		item->SetNumberField(TEXT("物品价值"), 3000000);
		RedArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("红"), RedArray);

	// 金 品质数组
	TArray<TSharedPtr<FJsonValue>> GoldArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("阿萨拉特色酒杯"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 80000);
		GoldArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("亮闪闪的海盗金币"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 80000);
		GoldArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("勋章"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 80000);
		GoldArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("发条八音盒"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 80000);
		GoldArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("金"), GoldArray);

	// 紫 品质数组
	TArray<TSharedPtr<FJsonValue>> PurpleArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("牛角"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_2);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品摆放"), PutState::Horizontal);
		item->SetNumberField(TEXT("物品价值"), 30000);
		PurpleArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("后妃耳环"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 20000);
		PurpleArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("海盗弯刀"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 20000);
		PurpleArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("紫"), PurpleArray);

	// 蓝 品质数组
	TArray<TSharedPtr<FJsonValue>> BlueArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("跳舞的女郎"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_2);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品摆放"), PutState::Vertical);
		item->SetNumberField(TEXT("物品价值"), 15000);
		BlueArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("古怪的海盗银币"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 10000);
		BlueArray.Add(MakeShared<FJsonValueObject>(item));
	}
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("古老的海盗望远镜"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_2);
		item->SetNumberField(TEXT("物品格数长"), 2);
		item->SetNumberField(TEXT("物品摆放"), PutState::Vertical);
		item->SetNumberField(TEXT("物品价值"), 15000);
		BlueArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("蓝"), BlueArray);

	// 绿 品质数组
	TArray<TSharedPtr<FJsonValue>> GreenArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("锈迹斑斑的海盗铜币"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 5000);
		GreenArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("绿"), GreenArray);

	// 白 品质数组
	TArray<TSharedPtr<FJsonValue>> WhiteArray;
	{
		TSharedPtr<FJsonObject> item = MakeShared<FJsonObject>();
		item->SetStringField(TEXT("物品名称"), TEXT("酸奶"));
		item->SetNumberField(TEXT("物品格数"), GoodsSlot::Slot_1);
		item->SetNumberField(TEXT("物品格数长"), 1);
		item->SetNumberField(TEXT("物品价值"), 30);
		WhiteArray.Add(MakeShared<FJsonValueObject>(item));
	}
	QualityObject->SetArrayField(TEXT("白"), WhiteArray);

	TSharedPtr<FJsonObject> ItemObject_BaseSetting = MakeShareable(new FJsonObject());
	ItemObject_BaseSetting->SetArrayField(TEXT("概率"),
		{
			MakeShareable(new FJsonValueNumber(0.02)),	//白
			MakeShareable(new FJsonValueNumber(0.02)),	//绿
			MakeShareable(new FJsonValueNumber(0.30)),	//蓝
			MakeShareable(new FJsonValueNumber(0.41)),	//紫
			MakeShareable(new FJsonValueNumber(0.00)),	//粉
			MakeShareable(new FJsonValueNumber(0.23)),	//金
			MakeShareable(new FJsonValueNumber(0.0198)),//红
			MakeShareable(new FJsonValueNumber(0.0002))	//橙
		});
	ItemObject_BaseSetting->SetObjectField(TEXT("物品"), QualityObject);

	//添加对象到 GameConfig: 基本设置, 其他
	GameConfig->SetObjectField(TEXT("基本设置"), ItemObject_BaseSetting);

	FString ErrorMessage;
	if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
		if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
		}
		else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::CreateConfigFile: Json格式错误!")), FColor::Red);
		}
		return;
	}

	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}

void UContainerQueueUserWidget::SaveConfigFile()
{
	FString ErrorMessage;
	if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
		if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
		}
		else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
			UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::CreateConfigFile: Json格式错误!")), FColor::Red);
		}

		return;
	}
	UEtytl::DebugLog(FString::Printf(TEXT("UContainerQueueUserWidget::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}
