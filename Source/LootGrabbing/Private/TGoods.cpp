// Fill out your copyright notice in the Description page of Project Settings.


#include "TGoods.h"

#include "TypicalTool/Public/TypicalToolBPLib.h"
#include "ContainerQueueUserWidget.h"
//#include "TypicalTool/Tool.h"

void UTGoods::Copy(const UTGoods* Data)
{
	Super::Copy(Data);
	ContainerQueueUserWidget = Data->ContainerQueueUserWidget;
}
