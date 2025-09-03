// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LootGrabbingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LOOTGRABBING_API ALootGrabbingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UWorld* World;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
