// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "State.h"
#include "LoginState.generated.h"

class ULoginWidget;
class UQueryPwdWidget;
class UDialogWidget;
class UDSGameInstance;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API ULoginState : public UObject, public IState
{
	GENERATED_BODY()

public:
	ULoginState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LoginState")
	void Begin();
	virtual void Begin_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LoginState")
	void Update();
	virtual void Update_Implementation() override;

	virtual class UWorld* GetWorld() const override;

private:
	UDSGameInstance* GameIns;
};
