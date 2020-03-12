// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "StateController.h"
#include "TestState.h"


UTestState::UTestState()
{}

UWorld* UTestState::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void UTestState::Begin_Implementation()
{
	Controller->LoadGameWidget(EState::TEST);
	Controller->BindWidgetFeature();

	const auto FirstWidget = Controller->GetWidgetFromHead();
	if (!FirstWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("FirstWidget Is Not Exist"));
		return;
	}

	Controller->PushWidget(FirstWidget);

	SetRunBegin(true);
}

void UTestState::Update_Implementation()
{}