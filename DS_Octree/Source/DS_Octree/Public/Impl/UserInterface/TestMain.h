// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abstract/WidgetBase.h"
#include "TestMain.generated.h"

class UCButton;
class UCImage;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UTestMain : public UWidgetBase
{
	GENERATED_BODY()
public:

	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCButton* Home;

	UPROPERTY(meta = (BindWidget))
	UCButton* VideoOne;							
	
	UPROPERTY(meta = (BindWidget))
	UCButton* VideoTwo;							
	
	UPROPERTY(meta = (BindWidget))
	UCButton* Sound;

	UPROPERTY(meta = (BindWidget))
	UCButton* MuteSound;
	
	UPROPERTY(meta = (BindWidget))
	UCButton* Exit;								
	
	UPROPERTY(meta = (BindWidget))
	UCButton* Menu;								

	UPROPERTY(meta = (BindWidget))
	class UCWidgetSwitcher* PureSoundCtrl;
};
