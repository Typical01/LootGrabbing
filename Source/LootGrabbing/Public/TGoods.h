// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Goods.h"

#include "TGoods.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LOOTGRABBING_API UTGoods : public UGoods
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	class UContainerQueueUserWidget* ContainerQueueUserWidget;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlay = true;

public:
	UTGoods() {}
	UTGoods(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	void Copy(const UTGoods* Data);
};
