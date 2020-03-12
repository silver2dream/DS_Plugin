// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abstract/WidgetBase.h"
#include "Screensaver.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UScreensaver : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UCImage* Screensaver;

};
