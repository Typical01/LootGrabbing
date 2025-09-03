// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/IUserObjectListEntry.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Goods.h"

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
	UImage* ImageLoadAnimation; //图像: 物品加载动画

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlockGoodsName; //文本框: 物品名

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageGoods; //图像: 物品

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageGoodsQualityColor; //图像: 物品品质颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageGoodsSlotBorder; //图像: 物品槽边框

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageSlotColor; //图像: 物品槽颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageSlotBackgroundColor; //图像: 物品槽背景颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Opacity = 0.24f; //透明度: 物品品质颜色

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MediaSourceIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FirstIndex = 0; //列表首项

	int32 SlotWidth;
	FVector2D HorizontalPutState;
	FVector2D VerticalPutState;

public:
	void SetTexture(UGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName = FString(), float _Opacity = 1.f);
	void SetMaterial(UGoods* _Goods, FString _TexttureForm, UImage* _Image, FString _ImageName = FString(), float _Opacity = 1.f);
	void LoadGoodsImage(class UGoods* _Goods);
};
