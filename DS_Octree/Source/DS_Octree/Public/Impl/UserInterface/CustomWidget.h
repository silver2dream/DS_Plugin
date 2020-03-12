// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abstract/WidgetBase.h"
#include "CustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCustomWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

};
