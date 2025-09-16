// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/IUserObjectListEntry.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "FileMediaSource.h"

#include "GoodsUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOTGRABBING_API UGoodsUserWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	/*
	* 重写
	*/
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;


	/*
	* 控件
	*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* SizeBoxGoods; //尺寸框

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ButtonGoods; //按钮: 物品

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageLoadAnimationQualityBorder; //图像: 物品加载动画品质边框

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageLoadAnimationQuality; //图像: 物品加载动画品质覆层

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageLoadAnimationIcon; //图像: 物品加载动画图标

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageLoadAnimation; //图像: 物品加载动画

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlockGoodsName; //文本框: 物品名

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageGoods; //图像: 物品

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageGoodsQualityColor; //图像: 物品品质颜色

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//UImage* ImageGoodsSlotBorder; //图像: 物品槽边框

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageSlotColor; //图像: 物品槽颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageSlotBackgroundColor; //图像: 物品槽背景颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Opacity = 0.24f; //透明度: 物品品质颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MediaSourceIndex = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UMediaPlayer* MediaPlayerQualityLoadAnimation; //媒体播放器: 品质加载动画

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UMediaTexture* MediaTextureQualityLoadAnimation; //媒体纹理: 品质加载动画

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UFileMediaSource* FileSourceQualityLoadAnimation; //媒体文件源: 品质加载动画

	int32 SlotWidth;
	int32 Slot;
	FVector2D HorizontalPutState;
	FVector2D VerticalPutState;
	FString SlotString;

public:
	void SetTexture(UImage* _Image, class UTGoods* _Goods, FString _TexttureForm, FString _ImageName = FString(), 
		float _Opacity = 1.25f, bool bIsZoom = true, FLinearColor LinearColor = FLinearColor(1.f, 1.f, 1.f, 1.f));
	void SetMaterial(class UTGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName = FString(), float _Opacity = 1.25f);
	void SetMediaTexture(class UTGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName, float _Opacity = 1.25f);
	void LoadGoodsImage(class UTGoods* _Goods);
};
