// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abstract/WidgetBase.h"
#include "JinContent.generated.h"

struct FMeta;
class UWidgetSwitcher;
class UCButton;
class UCVerticalBox;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UJinContent : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

	virtual bool Initialize() override;

private:
	void CreateMaskAndAnimation();

	void AddExtraEvent();

	void AddExtraAnimation(FName AnimateName, UWidget* Widget, FVector4 AnimateParam, float delayTime);

	UFUNCTION()
	void InitAllWidgetVisiable();

	void SetWidgetVisiable(ESlateVisibility Type);

	void SetMaskColor(class UImage* Mask, int32 Index);

	void DispatchImmediateEvent(FName AnimationName, FName FuncName, float Duration, float KeyTime);

	UFUNCTION()
	void ImmediateEvent();

	UFUNCTION()
	void SubImmediateEvent();

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ContentSwitcher;

	UPROPERTY(meta = (BindWidget))
	UCButton* FeelJinRay;

	UPROPERTY(meta = (BindWidget))
	UCButton* UpperSeven;

	UPROPERTY(meta = (BindWidget))
	UCButton* OnlyOne;

	UPROPERTY(meta = (BindWidget))
	UCButton* TheArch;

	UPROPERTY(meta = (BindWidget))
	UCButton* Construction;

	UPROPERTY(meta = (BindWidget))
	UCButton* TopTeam;

	UPROPERTY(meta = (BindWidget))
	int32 SelectedIndex;

	UPROPERTY()
	FName AnimateSub;

	UPROPERTY()
	TArray<class UCButton*> ButtonContainer;

	UPROPERTY()
	TArray<class UImage*> ButtonMaskContainer;

	UPROPERTY()
	TArray<class UImage*> ContentMaskContainer;

	FMeta ImmediateMeta;
	
};
