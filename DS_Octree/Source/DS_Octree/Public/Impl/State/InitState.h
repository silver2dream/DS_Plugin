// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "State.h"
#include "Register.h"
#include "InitState.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UInitState : public UObject, public IState, public IRegister
{
	GENERATED_BODY()
	
public:
	UInitState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UInitState")
			void Begin();
	virtual void Begin_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UInitState")
			void Update();
	virtual void Update_Implementation() override;

	virtual UWorld* GetWorld() const override;

protected:
	virtual void RegisterUniversalLambda() override;

};
