// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Config.h"
#include "AVPlayer.h"
#include "WidgetCtrl.h"
#include "StateController.generated.h"

class IState;
class UUserWidget;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UStateController : public UWidgetCtrl, public IConfig, public IAVPlayer
{
	GENERATED_BODY()
	
public:
	UStateController();

	void SetState(TScriptInterface<IState> state, UStateController* ctrler);
	
	void Update();

private:
	UPROPERTY()
	TScriptInterface<IState> currentState;

};
