// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "WidgetCtrl.generated.h"

class UWidgetBase;


UENUM()
namespace EState
{
	enum Status
	{
		INIT,
		LOGIN,
		DEMO,
		TEST
	};
}


/**
 * 
 */
UCLASS(abstract)
class DS_OCTREE_API UWidgetCtrl : public UObject
{
	GENERATED_BODY()

public:
	UWidgetCtrl();

	virtual UWorld* GetWorld() const override;

	void LoadGameWidget(int32 state);

	void BindWidgetFeature();

	UFUNCTION()
	void PushWidget(UWidgetBase* widget);

	UFUNCTION()
	void PopWidget();

	UFUNCTION()
	void PopAllWidget();

	void RemoveAllWidget();

	void SwapWidget();

	void ListenerAttach(class UWidget* Widget);

	bool IsPreviousAnimatePlaying();
	
	UWidgetBase* GetWidgetFromStore(FName WidgetName);

	UWidgetBase* GetWidgetFromHead();

	UPROPERTY()
	TMap<FString, class UWidget*> Listener;

protected:
	UPROPERTY()
	TMap<FName, UWidgetBase*> Store;

	UPROPERTY()
	TArray<UWidgetBase*> WidgetStack;

	UPROPERTY()
	UWidgetBase* PreviousWidget;
	
};
