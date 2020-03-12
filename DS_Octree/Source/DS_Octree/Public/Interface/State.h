// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "State.generated.h"

class UStateController;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UState : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API IState
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IState();

	void SetStateName(FString name);

	FString GetStateName();

	void SetRunBegin(bool flag);

	bool IsRunBegin();

	void SetController(UStateController* ctrler);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void Begin();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void Update();

protected:
	UStateController* Controller;

private:
	FString Statename;

	bool bRunBegin;
};
