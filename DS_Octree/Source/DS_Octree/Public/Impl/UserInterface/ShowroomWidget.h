// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetBase.h"
#include "ShowroomWidget.generated.h"

class UScrollBox;
class UWidgetSwitcher;
class AStudio;
class UImage;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UShowroomWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	
	virtual void BindTask(UWidgetCtrl* widgetCtrl) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* FeatureSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCButton* SwitchStyleButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCButton* SetupBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCButton* SwitchFurniture;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* CloseBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ItemBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCButton* RightArrow;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCButton* LeftArrow;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCButton* RecoverBtn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* AllView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* OneView;

	UPROPERTY()
	AStudio* StudioIns;

	UPROPERTY()
	AStaticMeshActor* focusActor;

private:
	UFUNCTION()
	void LoadAllItem();

	UFUNCTION()
	void Setup();

	UFUNCTION()
	void RecoverItem();
	
	UPROPERTY()
	AStaticMeshActor* StoreItemActor;

	void SwapItemDetail(AStaticMeshActor* focus, AStaticMeshActor* store);

	UPROPERTY()
	FString CacheItemName;
};
