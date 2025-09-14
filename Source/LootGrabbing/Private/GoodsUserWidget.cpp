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
    Slot = GoodsObject->GetSlot(); // 格数
    SlotWidth = GoodsObject->GetSlot() / GoodsObject->GetSlotLength(); // 格数: 高
    HorizontalPutState = FVector2D(130.8f * GoodsObject->GetSlotLength(), 130.8f * SlotWidth); // 水平显示大小
    VerticalPutState = FVector2D(130.8f * SlotWidth, 130.8f * GoodsObject->GetSlotLength()); // 垂直显示大小

    //UEtytl::DebugLog(TEXT("UGoodsUserWidget::NativeOnListItemObjectSet: 转换<UTGoods>成功!"), FColor::Green);
    if (GoodsObject->GetName().Equals(TEXT("空"))) {
        bIsNone = true;
        TextBlockGoodsName->SetText(FText::FromString(TEXT("")));
    }
    else {
        bIsNone = false;
        TextBlockGoodsName->SetText(FText::FromString(GoodsObject->GetName()));

        //格式化: 搜索背景
        if (GoodsObject->GetPutState() == PutState::Horizontal || GoodsObject->GetPutState() == PutState::None) {
            if (Slot == GoodsSlot::Slot_4) {
                FString GoodsName = GoodsObject->UGoods::GetName();
                if (GoodsName.Equals(TEXT("滑膛枪展示品"))) {
                    SlotString = FString::Printf(TEXT("%dH"), Slot);
                }
                else {
                    SlotString = FString::Printf(TEXT("%d"), Slot);
                }
            }
            SlotString = FString::Printf(TEXT("%d"), Slot);
        }
        else {
            SlotString = FString::Printf(TEXT("%dV"), Slot);
        }
    }

    MediaSourceIndex = GoodsObject->QuilityTransformInt();
    LoadGoodsImage(GoodsObject);

    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGoodsUserWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{


    IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

void UGoodsUserWidget::LoadGoodsImage(UTGoods* _Goods)
{
    if (!bIsNone) {
        if (_Goods->bIsPlay) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/Search/{0}.{1}")),
                ImageLoadAnimation, SlotString, 1.25f);
            SetTexture(_Goods, FString(TEXT("/Game/Image/Search/Icon/{0}.{0}")),
                ImageLoadAnimationIcon, SlotString, 1.25f);
        }
        else {
            if (ImageLoadAnimation) ImageLoadAnimation->SetRenderOpacity(0.f);
        }

        if (ImageGoods) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/Goods/{0}.{1}")), ImageGoods, _Goods->GetName(), 1.25f);
        }
        if (ImageGoodsQualityColor) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/Quility/{0}.{1}")), ImageGoodsQualityColor, _Goods->GetQuility(), Opacity);
        }
        //if (ImageGoodsSlotBorder) {
        //    ImageGoodsSlotBorder->SetColorAndOpacity(FLinearColor(37.f / 255.f, 37.f / 255.f, 37.f / 255.f, 1.0f));
        //}
        //SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageGoodsSlotBorder, FString(TEXT("容器槽边框")));
        if (ImageSlotColor) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageSlotColor, FString(TEXT("容器格子")), 1.0f, false);
            ImageSlotColor->SetColorAndOpacity(FLinearColor(37.f, 37.f, 37.f, 0.7f));
        }
        if (ImageSlotBackgroundColor) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageSlotBackgroundColor, FString(TEXT("渐变背景")), 1.25f, false);
            ImageSlotBackgroundColor->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.0f));
        }
    }
    else {
        if (ImageLoadAnimationIcon) ImageLoadAnimationIcon->SetRenderOpacity(0.f);
        if (ImageLoadAnimation) ImageLoadAnimation->SetRenderOpacity(0.f);
        if (ImageGoods) ImageGoods->SetRenderOpacity(0.f);
        if (ImageGoodsQualityColor) ImageGoodsQualityColor->SetRenderOpacity(0.f);

        if (ImageSlotColor) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageSlotColor, FString(TEXT("容器格子")), 1.0f, false);
            ImageSlotColor->SetColorAndOpacity(FLinearColor(37.f, 37.f, 37.f, 0.7f));
        }
        if (ImageSlotBackgroundColor) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageSlotBackgroundColor, FString(TEXT("渐变背景")), 1.25f, false);
            ImageSlotBackgroundColor->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.0f));
        }

        //if (ImageGoodsSlotBorder) ImageGoodsSlotBorder->SetRenderOpacity(0.f);
        //if (ImageSlotColor) ImageSlotColor->SetRenderOpacity(0.f);
        //if (ImageSlotBackgroundColor) ImageSlotBackgroundColor->SetRenderOpacity(0.f);
    }
}

void UGoodsUserWidget::SetTexture(UTGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName, float _Opacity, bool bIsZoom)
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
                    NewBrush.ImageSize = 130.8f;
                }
                else {
                    NewBrush.ImageSize = 130.8f;
                    _Image->SetBrush(NewBrush); // 将 brush 应用到 UImage
                }
            }
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
                _Image->SetBrush(OldBrush); // 将 brush 应用到 UImage
            }
        }
        else {
            if (_Goods->GetPutState() == PutState::Vertical) {
                OldBrush.ImageSize = 130.8f;
            }
            else {
                OldBrush.ImageSize = 130.8f;
                _Image->SetBrush(OldBrush); // 将 brush 应用到 UImage
            }
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