// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"

#include "LoginWidget.h"
#include "QueryPwdWidget.h"
#include "DialogWidget.h"

#include "DSGameInstance.h"
#include "StateController.h"
#include "DemoState.h"
#include "LoginState.h"
#include "TestState.h"

ULoginState::ULoginState()
{
}

void ULoginState::Begin_Implementation()
{
	Controller->LoadGameWidget(EState::LOGIN);
	Controller->BindWidgetFeature();

	GameIns = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SetRunBegin(true);
}

void ULoginState::Update_Implementation()
{
	// if user data is not empty, change state to init
	if (GameIns->IsLogined())
	{
		Controller->RemoveAllWidget();
		//Controller->SetState(NewObject<UDemoState>(GetWorld(), UDemoState::StaticClass()), Controller);
		Controller->SetState(NewObject<UTestState>(GetWorld(), UTestState::StaticClass()), Controller);
	}
	//Controller->SetState(NewObject<UTestState>(GetWorld(), UTestState::StaticClass()), Controller);
	// else show msg widget
}

class UWorld* ULoginState::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}
