// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "State.h"
#include "DemoState.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UDemoState : public UObject, public IState
{
	GENERATED_BODY()

public:
	UDemoState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DemoState")
		void Begin();
	virtual void Begin_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DemoState")
		void Update();
	virtual void Update_Implementation() override;
	
	UFUNCTION()
	void SetCustomDepthEnable(AActor* actor);
	
	UFUNCTION()
	void SetCustomDepthUnable(AActor* actor);

	UFUNCTION()
	void ShowItemMenu(AActor* actor, FKey key);

	virtual UWorld* GetWorld() const override;

};
