// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodsUserWidget.h"
#include "Engine/Texture2D.h"

#include "TypicalToolBPLib.h"
#include "TGoods.h"
#include "LootGrabbingGameInstance.h"



void UGoodsUserWidget::NativeConstruct()
{


    Super::NativeConstruct();
}

void UGoodsUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (!IsValid(ListItemObject))
    {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::NativeOnListItemObjectSet: ListItemObject 无效!"), FColor::Red);
        return;
    }

    UTGoods* GoodsObject = Cast<UTGoods>(ListItemObject);
    MediaSourceIndex = GoodsObject->QuilityTransformInt();
    Slot = GoodsObject->GetSlot(); // 格数
    SlotWidth = GoodsObject->GetSlot() / GoodsObject->GetSlotLength(); // 格数: 高
    HorizontalPutState = FVector2D(130.8f * GoodsObject->GetSlotLength(), 130.8f * SlotWidth); // 水平显示大小
    VerticalPutState = FVector2D(130.8f * SlotWidth, 130.8f * GoodsObject->GetSlotLength()); // 垂直显示大小

    //UEtytl::DebugLog(TEXT("UGoodsUserWidget::NativeOnListItemObjectSet: 转换<UTGoods>成功!"), FColor::Green);
    if (MediaSourceIndex == -1) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::NativeOnListItemObjectSet: ListItemObject 为其他类型 | 非: 空/物品!"), FColor::Red);
        return; //其他
    }

    if (MediaSourceIndex == 0) {
        TextBlockGoodsName->SetText(FText::FromString(TEXT("")));
    }
    else {
        TextBlockGoodsName->SetText(FText::FromString(GoodsObject->GetName()));

        //格式化: 搜索背景
        if (GoodsObject->GetPutState() != PutState::Vertical) {
            if (Slot == GoodsSlot::Slot_4) {
                if (GoodsObject->UGoods::GetName().Equals(TEXT("滑膛枪展示品"))) {
                    SlotString = FString::Printf(TEXT("%dH"), Slot);
                }
            }
            SlotString = FString::Printf(TEXT("%d"), Slot);
        }
        else {
            SlotString = FString::Printf(TEXT("%dV"), Slot);
        }
    }

    LoadGoodsImage(GoodsObject);

    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGoodsUserWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{


    IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGoodsUserWidget::LoadGoodsImage(UTGoods* _Goods)
{
    if (!IsValid(ImageGoods)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageGoods 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageGoodsQualityColor)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageGoodsQualityColor 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageSlotColor)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageSlotColor 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageSlotBackgroundColor)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageSlotBackgroundColor 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(TextBlockGoodsName)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: TextBlockGoodsName 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageLoadAnimation)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageLoadAnimation 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageLoadAnimationIcon)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageLoadAnimationIcon 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageLoadAnimationQuality)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageLoadAnimationQuality 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ImageLoadAnimationQualityBorder)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ImageLoadAnimationQualityBorder 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(SizeBoxGoods)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: SizeBoxGoods 无效!"), FColor::Red);
        return;
    }
    if (!IsValid(ButtonGoods)) {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::LoadGoodsImage: ButtonGoods 无效!"), FColor::Red);
        return;
    }

    if (MediaSourceIndex != 0) {
        FLinearColor QuilityColor(1.f, 1.f, 1.f, 1.f);
        FLinearColor QuilityHighlightColor(1.f, 1.f, 1.f, 1.f);
        if (MediaSourceIndex == 1) { // 1: 白
            QuilityColor = FLinearColor(51.f / 255.f, 51.f / 255.f, 51.f / 255.f, 0.28f);
        }
        else if (MediaSourceIndex == 2) { // 2: 绿
            QuilityColor = FLinearColor(5.1f / 255.f, 25.5f / 255.f, 5.1f / 255.f, 0.28f);
        }
        else if (MediaSourceIndex == 3) { // 3: 蓝
            QuilityColor = FLinearColor(14.f / 255.f, 51.f / 255.f, 102.f / 255.f, 0.28f);
        }
        else if (MediaSourceIndex == 4) { // 4: 紫
            QuilityColor = FLinearColor(34.425f / 255.f, 14.f / 255.f, 51.f / 255.f, 0.28f);
            QuilityHighlightColor = FLinearColor(34.425f / 255.f, 14.f / 255.f, 51.f / 255.f, 1.f);
        }
        else if (MediaSourceIndex == 5) { // 5: 金
            QuilityColor = FLinearColor(204.f / 255.f, 102.f / 255.f, 14.f / 255.f, 0.28f);
            QuilityHighlightColor = FLinearColor(204.f / 255.f, 102.f / 255.f, 14.f / 255.f, 1.f);
        }
        else if (MediaSourceIndex >= 6) { // 6~7: 红/橙
            QuilityColor = FLinearColor(255.f / 255.f, 14.f / 255.f, 14.f / 255.f, 0.28f);
            QuilityHighlightColor = FLinearColor(255.f / 255.f, 14.f / 255.f, 14.f / 255.f, 1.f);
        }

        if (_Goods->GetPutState() != PutState::Vertical) {
            SizeBoxGoods->SetWidthOverride(HorizontalPutState.X);
            SizeBoxGoods->SetHeightOverride(HorizontalPutState.Y);
        }
        else {
            SizeBoxGoods->SetWidthOverride(VerticalPutState.X);
            SizeBoxGoods->SetHeightOverride(VerticalPutState.Y);
        }

        if (_Goods->bIsPlay) {
            SetTexture(ImageLoadAnimation, _Goods, FString(TEXT("/Game/Image/Search/{0}.{1}")),
                SlotString, 1.25f);
            SetTexture(ImageLoadAnimationIcon, _Goods, FString(TEXT("/Game/Image/Search/Icon/{0}.{1}")),
                SlotString, 1.25f);
            SetTexture(ImageLoadAnimationQuality, _Goods, FString(TEXT("/Game/Image/Search/VFX/{0}.{1}")),
                TEXT("Default"), 0.f, true, QuilityHighlightColor);
            SetTexture(ImageLoadAnimationQualityBorder, _Goods, FString(TEXT("/Game/Image/Search/VFX/{0}.{1}")),
                TEXT("Border"), 0.f, true, QuilityHighlightColor);
        }
        else {
            ImageLoadAnimationIcon->SetRenderOpacity(0.f);
            ImageLoadAnimationQuality->SetRenderOpacity(0.f);
            ImageLoadAnimationQualityBorder->SetRenderOpacity(0.f);
            ImageLoadAnimation->SetRenderOpacity(0.f);
        }
        SetTexture(ImageGoods, _Goods, FString(TEXT("/Game/Image/Goods/{0}.{1}")), _Goods->GetName(), 1.25f);
        SetTexture(ImageGoodsQualityColor, _Goods, FString(TEXT("/Game/Image/Quility/{0}.{1}")), TEXT("Default"), 1.f, true, QuilityColor);
        ButtonGoods->SetVisibility(ESlateVisibility::Visible); //可见占用布局
    }
    else {
        ImageLoadAnimationIcon->SetRenderOpacity(0.f);
        ImageLoadAnimationQuality->SetRenderOpacity(0.f);
        ImageLoadAnimationQualityBorder->SetRenderOpacity(0.f);
        ImageLoadAnimation->SetRenderOpacity(0.f);
        ImageGoods->SetRenderOpacity(0.f);
        ImageGoodsQualityColor->SetRenderOpacity(0.f);
        ButtonGoods->SetVisibility(ESlateVisibility::Collapsed); //不可见不占用布局
    }
}

void UGoodsUserWidget::SetTexture(UImage* _Image, UTGoods* _Goods, FString _TexttureForm, FString _ImageName, float _Opacity, bool bIsZoom, FLinearColor LinearColor)
{
    if (!_TexttureForm.Equals(TEXT(""))) {
        FString TexturePath = FString::Format(*_TexttureForm, { *_ImageName, *_ImageName });
        UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, *TexturePath);
        if (Texture) {
            FSlateBrush NewBrush;
            NewBrush.SetResourceObject(Texture); // 绑定纹理资源
            if (bIsZoom) {
                if (_Goods->GetPutState() == PutState::Vertical) {
                    NewBrush.ImageSize = VerticalPutState;
                }
                else {
                    NewBrush.ImageSize = HorizontalPutState;
                }
            }
            else {
                if (_Goods->GetPutState() == PutState::Vertical) {
                    NewBrush.ImageSize = VerticalPutState;
                }
                else {
                    NewBrush.ImageSize = HorizontalPutState;
                }
            }
            NewBrush.TintColor = FSlateColor(LinearColor);
            _Image->SetBrush(NewBrush); // 将 brush 应用到 UImage
            _Image->SetRenderOpacity(_Opacity);
            //UEtytl::DebugLog(TEXT("UGoodsUserWidget::SetTexture: 加载物品图像成功!"), FColor::Green);
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetTexture: 无效的图片加载路径 [%s]"), *TexturePath));
        }
    }
    else {
        FSlateBrush OldBrush = _Image->GetBrush();
        if (bIsZoom) {
            if (_Goods->GetPutState() == PutState::Vertical) {
                OldBrush.ImageSize = VerticalPutState;
            }
            else {
                OldBrush.ImageSize = HorizontalPutState;
            }
            _Image->SetBrush(OldBrush); // 将 brush 应用到 UImage
        }
        else {
            if (_Goods->GetPutState() == PutState::Vertical) {
                OldBrush.ImageSize = VerticalPutState;
            }
            else {
                OldBrush.ImageSize = HorizontalPutState;
            }
           _Image->SetBrush(OldBrush); // 将 brush 应用到 UImage
        }
    }
}

void UGoodsUserWidget::SetMaterial(UTGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName, float _Opacity)
{
    if (!_TexttureForm.Equals(TEXT(""))) {
        FString MaterialPath = FString::Format(*_TexttureForm, { *_ImageName, *_ImageName, *_ImageName });
        UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);
        if (Material) {
            UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
            if (DynMaterial) {
                FSlateBrush NewBrush;
                NewBrush.SetResourceObject(DynMaterial); // 绑定纹理资源
                if (_Goods->GetPutState() == PutState::Vertical) {
                    NewBrush.ImageSize = VerticalPutState;
                }
                else {
                    NewBrush.ImageSize = HorizontalPutState;
                }
                _Image->SetBrush(NewBrush); // 将 brush 应用到 UImage
                _Image->SetRenderOpacity(_Opacity);
                //UEtytl::DebugLog(TEXT("UGoodsUserWidget::SetMaterial: 加载物品图像成功!"), FColor::Green);
            }
            else {
                UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMaterial: 动态材质实例创建失败! 路径 [%s]"), *MaterialPath));
            }
        }
        else {
            UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMaterial: 无效的材质实例加载路径 [%s]"), *MaterialPath));
        }
    }
    else {
        FSlateBrush OldBrush = _Image->GetBrush();
        if (_Goods->GetPutState() == PutState::Vertical) {
            OldBrush.ImageSize = VerticalPutState;
        }
        else {
            OldBrush.ImageSize = HorizontalPutState;
            _Image->SetBrush(OldBrush); // 将 brush 应用到 UImage
        }
    }
}

void UGoodsUserWidget::SetMediaTexture(UTGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName, float _Opacity)
{
    //if (MediaSourceIndex <= 0) {
    //    _Image->SetRenderOpacity(0.f);
    //    return;
    //}

    //auto* tempGameInstance = GetWorld()->GetGameInstance();
    //if (!tempGameInstance) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMediaTexture: 获取游戏实例失败!")));
    //    _Image->SetRenderOpacity(0.f);
    //    return;
    //}
    //ULootGrabbingGameInstance* LootGrabbingGameInstance = Cast<ULootGrabbingGameInstance>(tempGameInstance);
    //if (!IsValid(LootGrabbingGameInstance)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMediaTexture: 转换<LootGrabbingGameInstance>游戏实例失败!")));
    //    _Image->SetRenderOpacity(0.f);
    //    return;
    //}

    //// 1) 创建 MediaPlayer
    //MediaPlayerQualityLoadAnimation = LootGrabbingGameInstance->MediaPlayers[MediaSourceIndex];
    //if (MediaPlayerQualityLoadAnimation) {
    //    MediaPlayerQualityLoadAnimation->OnMediaOpened.AddDynamic(this, &UGoodsUserWidget::HandleMediaOpened);
    //    MediaPlayerQualityLoadAnimation->OnMediaOpenFailed.AddDynamic(this, &UGoodsUserWidget::HandleMediaOpenFailed);
    //}

    //// 2) 创建 MediaTexture 并绑定到 Player
    //MediaTextureQualityLoadAnimation = LootGrabbingGameInstance->MediaTextures[MediaSourceIndex];
    //MediaTextureQualityLoadAnimation->SetMediaPlayer(MediaPlayerQualityLoadAnimation);
    //MediaTextureQualityLoadAnimation->UpdateResource();

    //// 3) 创建 FileMediaSource 并设置路径
    //FileSourceQualityLoadAnimation = LootGrabbingGameInstance->FileSources[MediaSourceIndex];
    //if (!IsValid(FileSourceQualityLoadAnimation)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMediaTexture: FileSources[%d]为空指针!"), MediaSourceIndex));
    //    _Image->SetRenderOpacity(0.f);
    //    return;
    //}

    //if (!MediaPlayerQualityLoadAnimation->OpenSource(FileSourceQualityLoadAnimation)) {
    //    UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMediaTexture: 打开媒体文件[%d]失败!"), MediaSourceIndex));
    //    _Image->SetRenderOpacity(0.f);
    //    return;
    //}

    //FSlateBrush NewBrush;
    //NewBrush.SetResourceObject(MediaTextureQualityLoadAnimation); // 绑定纹理资源
    //if (_Goods->GetPutState() == PutState::Vertical) {
    //    NewBrush.ImageSize = VerticalPutState;
    //}
    //else {
    //    NewBrush.ImageSize = HorizontalPutState;
    //}
    //_Image->SetBrush(NewBrush); // 将 brush 应用到 UImage
    //_Image->SetRenderOpacity(_Opacity);
}