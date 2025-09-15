// Fill out your copyright notice in the Description page of Project Settings.


#include "LootGrabbingGameInstance.h"
#include "ContainerQueueUserWidget.h"

#include "TypicalToolBPLib.h"



void ULootGrabbingGameInstance::Init()
{
	//初始化程序数据
	if (!UEtytl::CreateDirectory(TEXT("Config"))) {
		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: 文件夹创建失败/已创建!")), FColor::Red);
	}

	FString ErrorMessage;
	if (!UEtytl::ReadJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
		if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
			CreateConfigFile();
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: 创建配置文件!")));
		}
		else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: Json格式错误!")));
			return;
		}
	}
	UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: 配置文件读取成功!")), FColor::Yellow);

	//容器视图
	TileViewDataManageContainerSlot = NewObject<UTileViewDataManage>();
	TileViewDataManageWareHouseSlot = NewObject<UTileViewDataManage>();

	//媒体资源初始化
	//颜色索引：0 为空, 1..7 为下面顺序
	//TArray<FString> ColorNames = {
	//	TEXT(""), // 0 空
	//	TEXT("白"),
	//	TEXT("绿"),
	//	TEXT("蓝"),
	//	TEXT("紫"),
	//	TEXT("金"),
	//	TEXT("红"),
	//	TEXT("橙")
	//};

	//const int32 Num = ColorNames.Num(); // 8
	//FileSources.SetNum(Num);
	//MediaPlayers.SetNum(Num);
	//MediaTextures.SetNum(Num);

	//for (int32 i = 0; i < Num; ++i) {
	//	if (i == 0) {
	//		FileSources[i] = nullptr;
	//		MediaPlayers[i] = nullptr;
	//		MediaTextures[i] = nullptr;
	//		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: 品质[空]填充为空指针！")));
	//		continue;
	//	}

	//	// 资源路径格式：/Game/Movies/<Name>.<Name>
	//	FString AssetPath = FString::Printf(TEXT("/Game/Movies/%s.%s"), *ColorNames[i], *ColorNames[i]);
	//	ColorNames[i] = AssetPath;

	//	// 静态加载 FileMediaSource（如果该 asset 确实存在于 Content）
	//	UObject* Loaded = StaticLoadObject(UFileMediaSource::StaticClass(), nullptr, *AssetPath);
	//	if (!Loaded) {
	//		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: 加载媒体文件 [%s] 失败！%s 填充为空指针!"), *AssetPath, *AssetPath));
	//		FileSources[i] = nullptr;
	//		MediaPlayers[i] = nullptr;
	//		MediaTextures[i] = nullptr;
	//		continue;
	//	}

	//	UFileMediaSource* FileSource = Cast<UFileMediaSource>(Loaded);
	//	FileSources[i] = FileSource;

	//	// 1) 创建 MediaPlayer（用 this 作为 Outer 保持生命周期）
	//	UMediaPlayer* MediaPlayer = NewObject<UMediaPlayer>(this);
	//	if (MediaPlayer) {
	//		// 注意：绑定回调（请确保回调签名与 AddDynamic 匹配）
	//		//MediaPlayer->OnMediaOpened.AddDynamic(this, &UGoodsUserWidget::HandleMediaOpened);
	//		//MediaPlayer->OnMediaOpenFailed.AddDynamic(this, &UGoodsUserWidget::HandleMediaOpenFailed);
	//		MediaPlayers[i] = MediaPlayer;
	//	}
	//	else {
	//		MediaPlayers[i] = nullptr;
	//	}

	//	// 2) 创建 MediaTexture 并绑定到 Player（若需要显示）
	//	UMediaTexture* MediaTexture = NewObject<UMediaTexture>(this);
	//	if (MediaTexture && MediaPlayer) {
	//		MediaTexture->SetMediaPlayer(MediaPlayer);
	//		// UpdateResource 可使渲染资源更新，但通常在设置材质时也会被调用
	//		//MediaTexture->UpdateResource();
	//		MediaTextures[i] = MediaTexture;
	//	}
	//	else {
	//		MediaTextures[i] = nullptr;
	//		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: MediaTexture 并绑定到 Player [%s] 失败！MediaTextures 填充为空指针!"), *AssetPath));
	//	}
	//	UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::Init: 媒体初始化成功[%s]!"), *AssetPath));
	//}

	////FString FilePath = FString::Printf(TEXT(""), *_ImageName, *_ImageName);
	////UObject* Loaded = StaticLoadObject(UFileMediaSource::StaticClass(), nullptr, *FilePath);
	////if (!Loaded) {
	////	UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMediaTexture: 加载媒体文件[%s]失败!"), *FilePath));
	////	_Image->SetRenderOpacity(0.f);
	////	return;
	////}

	////// 1) 创建 MediaPlayer
	////MediaPlayerQualityLoadAnimation = NewObject<UMediaPlayer>(this);
	////if (MediaPlayerQualityLoadAnimation) {
	////	MediaPlayerQualityLoadAnimation->OnMediaOpened.AddDynamic(this, &UGoodsUserWidget::HandleMediaOpened);
	////	MediaPlayerQualityLoadAnimation->OnMediaOpenFailed.AddDynamic(this, &UGoodsUserWidget::HandleMediaOpenFailed);
	////}

	////// 2) 创建 MediaTexture 并绑定到 Player
	////MediaTextureQualityLoadAnimation = NewObject<UMediaTexture>(this);
	////MediaTextureQualityLoadAnimation->SetMediaPlayer(MediaPlayerQualityLoadAnimation);
	////MediaTextureQualityLoadAnimation->UpdateResource();

	////// 3) 创建 FileMediaSource 并设置路径
	//////FileSourceQualityLoadAnimation = NewObject<UFileMediaSource>();
	//////FileSourceQualityLoadAnimation->SetFilePath(FilePath); // 绝对路径或项目内相对路径
	////FileSourceQualityLoadAnimation = Cast<UFileMediaSource>(Loaded);
}

bool ULootGrabbingGameInstance::ExtractProbabilities(const TSharedPtr<FJsonObject>& _GameConfig, TArray<double>& OutProbabilities)
{
	OutProbabilities.Empty();
	if (!_GameConfig.IsValid() || !_GameConfig->HasField(TEXT("基本设置")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractProbabilities: Json对象没有[基本设置]!")), FColor::Red);
		return false;
	}

	TSharedPtr<FJsonObject> BaseSetting = _GameConfig->GetObjectField(TEXT("基本设置"));
	if (!BaseSetting.IsValid() || !BaseSetting->HasField(TEXT("概率")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractProbabilities: Json对象没有[概率]!")), FColor::Red);
		return false;
	}

	// 读取数组
	TArray<TSharedPtr<FJsonValue>> ProbArray = BaseSetting->GetArrayField(TEXT("概率"));
	int32 Index = 0;
	for (const TSharedPtr<FJsonValue>& Val : ProbArray)
	{
		if (!Val.IsValid()) {
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractProbabilities: Json对象没有[概率][%d]!"),
				Index), FColor::Red);
		}
		else {
			OutProbabilities.Add(Val->AsNumber());
		}
		Index++;
	}

	return true;
}

bool ULootGrabbingGameInstance::ExtractItemsByQuality(const TSharedPtr<FJsonObject>& _GameConfig, const FString& Quality, TArray<UTGoods*>& OutItems, UUserWidget* Widget)
{
	OutItems.Empty();
	if (!_GameConfig.IsValid() || !_GameConfig->HasField(TEXT("基本设置")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: Json对象没有[基本设置]!")), FColor::Red);
		return false;
	}

	TSharedPtr<FJsonObject> BaseSetting = _GameConfig->GetObjectField(TEXT("基本设置"));
	if (!BaseSetting.IsValid() || !BaseSetting->HasField(TEXT("物品")))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: Json对象没有[物品]!")), FColor::Red);
		return false;
	}

	TSharedPtr<FJsonObject> GoodsObject = BaseSetting->GetObjectField(TEXT("物品"));
	if (!GoodsObject.IsValid() || !GoodsObject->HasField(Quality))
	{
		UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: Json对象[物品]没有[%s]!"), *Quality), FColor::Red);
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
			/*UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: 物品名称[%s]"),
				*GoodsName), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品格数")))
		{
			GoodsSlotNum = FMath::RoundToInt32(ItemObj->GetNumberField(TEXT("物品格数")));
			/*UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: 物品格数[%lld]"),
				GoodsSlotNum), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品格数长")))
		{
			GoodsSlotLength = FMath::RoundToInt32(ItemObj->GetNumberField(TEXT("物品格数长")));
			/*UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: 物品格数长[%d]"),
				GoodsSlotLength), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品摆放")))
		{
			GoodsPutState = static_cast<PutState>(FMath::RoundToInt32(ItemObj->GetNumberField(TEXT("物品摆放"))));
			/*UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: 物品摆放[%d]"),
				GoodsPutState), FColor::Green);*/
		}
		if (ItemObj->HasField(TEXT("物品价值")))
		{
			GoodsValue = FMath::RoundToInt64(ItemObj->GetNumberField(TEXT("物品价值")));
			/*UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: 物品价值[%lld]"),
				GoodsValue), FColor::Green);*/
		}

		UTGoods* _Goods = NewObject<UTGoods>(this);
		_Goods->Init(GoodsName, Quality, static_cast<GoodsSlot>(GoodsSlotNum), GoodsSlotLength, GoodsValue);
		_Goods->SetPutState(GoodsPutState);
		_Goods->ContainerQueueUserWidget = Cast<UContainerQueueUserWidget>(Widget);

		/*UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::ExtractItemsByQuality: TGoods[%p] | Name[%s] | class[%s]"),
			_Goods->ContainerQueueUserWidget, *_Goods->UTGoods::GetName(), *GetNameSafe(_Goods->ContainerQueueUserWidget)), FColor::Green);*/

		OutItems.Add(_Goods);
	}

	return true;
}

void ULootGrabbingGameInstance::CreateConfigFile()
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
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::CreateConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
		}
		else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::CreateConfigFile: Json格式错误!")), FColor::Red);
		}
		return;
	}

	UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::CreateConfigFile: 文件创建成功!")), FColor::Yellow);
}

void ULootGrabbingGameInstance::SaveConfigFile()
{
	FString ErrorMessage;
	if (!UEtytl::WriteJsonFile(*FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Config/Config.json")), GameConfig, ErrorMessage)) {
		if (ErrorMessage.Equals(TEXT("文件错误"))) { //没有文件
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::SaveConfigFile: 写入文件[Config.json]错误!")), FColor::Red);
		}
		else if (ErrorMessage.Equals(TEXT("非文件错误"))) { //Json格式错误
			UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::SaveConfigFile: Json格式错误!")), FColor::Red);
		}

		return;
	}
	UEtytl::DebugLog(FString::Printf(TEXT("ULootGrabbingGameInstance::SaveConfigFile: 文件创建成功!")), FColor::Yellow);
}
