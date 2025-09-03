// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodsUserWidget.h"
#include "Engine/Texture2D.h"

#include "TypicalToolBPLib.h"
#include "Gacha.h"



void UGoodsUserWidget::NativeConstruct()
{
}

void UGoodsUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (!IsValid(ListItemObject))
    {
        UEtytl::DebugLog(TEXT("UGoodsUserWidget::NativeOnListItemObjectSet: ListItemObject 无效!"), FColor::Red);
        return;
    }

    UGoods* GoodsObject = Cast<UGoods>(ListItemObject);
    SlotWidth = GoodsObject->GetSlot() / GoodsObject->GetSlotLength(); // 格数: 高
    HorizontalPutState = FVector2D(130.8f * GoodsObject->GetSlotLength(), 130.8f * SlotWidth); // 水平显示大小
    VerticalPutState = FVector2D(130.8f * SlotWidth, 130.8f * GoodsObject->GetSlotLength()); // 垂直显示大小

    //UEtytl::DebugLog(TEXT("UGoodsUserWidget::NativeOnListItemObjectSet: 转换<UGoods>成功!"), FColor::Green);
    if (GoodsObject->GetName().Equals(TEXT("空"))) {
        TextBlockGoodsName->SetText(FText::FromString(TEXT("")));
    }
    else if (GoodsObject->GetName().Equals(TEXT("空白"))) {
        TextBlockGoodsName->SetText(FText::FromString(TEXT("")));
    }
    else {
        TextBlockGoodsName->SetText(FText::FromString(GoodsObject->GetName()));
    }

    FString GoodsQuility = GoodsObject->GetQuility();
    if (ImageLoadAnimation) ImageLoadAnimation->SetRenderOpacity(1.f);
    if (GoodsQuility.Equals(TEXT("白"))) {
        MediaSourceIndex = 1;
    }
    else if (GoodsQuility.Equals(TEXT("绿"))) {
        MediaSourceIndex = 2;
    }
    else if (GoodsQuility.Equals(TEXT("蓝"))) {
        MediaSourceIndex = 3;
    }
    else if (GoodsQuility.Equals(TEXT("紫"))) {
        MediaSourceIndex = 4;
    }
    else if (GoodsQuility.Equals(TEXT("金"))) {
        MediaSourceIndex = 5;
    }
    else if (GoodsQuility.Equals(TEXT("红"))) {
        MediaSourceIndex = 6;
    }
    else if (GoodsQuility.Equals(TEXT("橙"))) {
        MediaSourceIndex = 7;
    }
    else {
        if (ImageLoadAnimation) ImageLoadAnimation->SetRenderOpacity(0.f);
        MediaSourceIndex = 0; //默认
    }

    LoadGoodsImage(GoodsObject);

    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UGoodsUserWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
    //if (bIsSelected) {
    //    GoodsUserWidget->SetColorAndOpacity(FLinearColor::White); // 设置高亮颜色
    //}
    //else {
    //    GoodsUserWidget->SetColorAndOpacity(FLinearColor::Gray); // 设置默认颜色
    //}
}

void UGoodsUserWidget::SetTexture(UGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName, float _Opacity)
{
    if (_Image) {
        if (!_TexttureForm.Equals(TEXT(""))) {
            FString TexturePath = FString::Format(*_TexttureForm, { *_ImageName, *_ImageName });
            UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, *TexturePath);
            if (Texture) {
                FSlateBrush NewBrush;
                NewBrush.SetResourceObject(Texture); // 绑定纹理资源
                if (_Goods->GetPutState() == PutState::Vertical) {
                    NewBrush.ImageSize = VerticalPutState;
                }
                else {
                    NewBrush.ImageSize = HorizontalPutState;
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
            if (_Goods->GetPutState() == PutState::Vertical) {
                OldBrush.ImageSize = VerticalPutState;
            }
            else {
                OldBrush.ImageSize = HorizontalPutState;
                _Image->SetBrush(OldBrush); // 将 brush 应用到 UImage
            }
        }
    }
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetTexture: 无效的UImage*[%s]"), *_TexttureForm));
    }
}

void UGoodsUserWidget::SetMaterial(UGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName, float _Opacity)
{
    if (_Image) {
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
    else {
        UEtytl::DebugLog(FString::Printf(TEXT("UGoodsUserWidget::SetMaterial: 无效的UImage*[%s]"), *_TexttureForm));
    }
}

void UGoodsUserWidget::LoadGoodsImage(UGoods* _Goods)
{
    if (!_Goods->GetName().Equals(TEXT("空白"))) {
        if (ImageLoadAnimation) {
            SetMaterial(_Goods, FString(TEXT("/Game/Media/{0}/{1}.{2}")), ImageLoadAnimation, _Goods->GetQuility(), 1.f);
        }
        if (ImageGoods) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/Goods/{0}.{1}")), ImageGoods, _Goods->GetName(), 1.f);
        }
        if (ImageGoodsQualityColor) {
            SetTexture(_Goods, FString(TEXT("/Game/Image/Quility/{0}.{1}")), ImageGoodsQualityColor, _Goods->GetQuility(), Opacity);
        }
        if (!_Goods->GetName().Equals(TEXT("空"))) {
            if (ImageGoodsSlotBorder) {
                ImageGoodsSlotBorder->SetColorAndOpacity(FLinearColor(37.f / 255.f, 37.f / 255.f, 37.f / 255.f, 1.f));
                SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageGoodsSlotBorder, FString(TEXT("容器槽边框")));
            }
        }
        if (ImageSlotColor) {
            ImageSlotColor->SetColorAndOpacity(FLinearColor(255.f, 255.f, 255.f, 0.7f));
            SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageSlotColor, FString(TEXT("容器格子")));
        }
        if (ImageSlotBackgroundColor) {
            ImageSlotBackgroundColor->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.f));
            SetTexture(_Goods, FString(TEXT("/Game/Image/{0}.{1}")), ImageSlotBackgroundColor, FString(TEXT("渐变背景")));
        }
    }
    else {
        if (ImageLoadAnimation) ImageLoadAnimation->SetRenderOpacity(0.f);
        if (ImageGoods) ImageGoods->SetRenderOpacity(0.f);
        if (ImageGoodsQualityColor) ImageGoodsQualityColor->SetRenderOpacity(0.f);
        if (ImageGoodsSlotBorder) ImageGoodsSlotBorder->SetRenderOpacity(0.f);
        if (ImageSlotColor) ImageSlotColor->SetRenderOpacity(0.f);
        if (ImageSlotBackgroundColor) ImageSlotBackgroundColor->SetRenderOpacity(0.f);
    }
}
