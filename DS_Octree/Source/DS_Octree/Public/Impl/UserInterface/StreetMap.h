// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abstract/WidgetBase.h"
#include "StreetMap.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UStreetMap : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	
	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UCImage* StreetPic;
};
