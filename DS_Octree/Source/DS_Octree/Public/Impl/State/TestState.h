// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "State.h"
#include "TestState.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UTestState : public UObject, public IState
{
	GENERATED_BODY()
	
public:
	UTestState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TestState")
	void Begin();
	virtual void Begin_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TestState")
	void Update();
	virtual void Update_Implementation() override;

	virtual UWorld* GetWorld() const override;

};
