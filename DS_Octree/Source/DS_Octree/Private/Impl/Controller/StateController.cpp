// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "StateController.h"
#include "State.h"

UStateController::UStateController()
{}

void UStateController::SetState(TScriptInterface<IState> state, UStateController * ctrler)
{
	if (currentState != nullptr)
	{
		currentState = nullptr;
	}

	currentState = state;
	currentState->SetController(ctrler);
}

void UStateController::Update()
{
	if (currentState != nullptr && !currentState->IsRunBegin())
	{
		currentState->Begin_Implementation();
	}

	if (currentState != nullptr)
	{
		currentState->Update_Implementation();
	}
}

