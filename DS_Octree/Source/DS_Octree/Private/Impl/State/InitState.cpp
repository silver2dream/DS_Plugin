// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Studio.h"
#include "StateController.h"
#include "LoginState.h"
#include "InitState.h"
#include "Listener.h"
#include "CImage.h"
#include "CButton.h"
#include "CWidgetSwitcher.h"
#include "TestState.h"

#include "DatabaseProxy.h"
#include "EventContainer.h"

UInitState::UInitState()
{}

UWorld* UInitState::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void UInitState::Begin_Implementation()
{
	auto ConnectProperties = Controller->GetConnectProperties();
	if (UDatabaseProxy::IsConnectDatabase(ConnectProperties))
	{
		Controller->LoadSetting();

		const auto WorldContent = GetWorld();
		if (WorldContent)
		{
			Controller->AudioAndVideoPlayerInit(WorldContent, Controller->Config);
			Controller->SetMeshHoveredOutline(WorldContent);
		}
		
		RegisterUniversalLambda();

		SetRunBegin(true);
	}
}

void UInitState::Update_Implementation()
{
	Controller->SetState(NewObject<UTestState>(GetWorld(), UTestState::StaticClass()), Controller);
}

void UInitState::RegisterUniversalLambda()
{
	CtrlRef = Controller;

	UEventContainer::RegisterAs("None", FLambda([=](FMeta meta) {}));

	UEventContainer::RegisterAs("PushWidget", FLambda([=](FMeta meta) { 
		if (CtrlRef.IsValid()) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, meta.WidgetName.ToString());
			const auto Widget = CtrlRef->GetWidgetFromStore(meta.WidgetName);
			if (Widget)
			{
				CtrlRef->PushWidget(Widget);
			}
		}
	 }));

	UEventContainer::RegisterAs("PushWidgetByListener", FLambda([=](FMeta meta) {
		const auto Widget = CtrlRef->GetWidgetFromStore(*meta.RefComponent);
		if (Widget)
		{
			CtrlRef->PushWidget(Widget);
		}
	}));

	UEventContainer::RegisterAs("PopWidget", FLambda([=](FMeta meta) { 
		if (CtrlRef == nullptr) { GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "CtrlRef is Empty!"); return; }
		CtrlRef->PopWidget(); 
	}));

	UEventContainer::RegisterAs("PopAllWidget", FLambda([=](FMeta meta) {
		CtrlRef->PopAllWidget();
	}));

	UEventContainer::RegisterAs("SwapWidget", FLambda([=](FMeta meta) {
		CtrlRef->SwapWidget();
	}));

	UEventContainer::RegisterAs("Play", FLambda([=](FMeta meta) {
		CtrlRef->Play(); 
	}));

	UEventContainer::RegisterAs("Pause", FLambda([=](FMeta meta) {
		CtrlRef->Pause(); 
	}));

	UEventContainer::RegisterAs("Stop", FLambda([=](FMeta meta) {
		CtrlRef->Stop(); 
	}));

	UEventContainer::RegisterAs("PlaySoundPure", FLambda([=](FMeta meta) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "PlaySoundPure");
		CtrlRef->PlaySoundPure(); }));

	UEventContainer::RegisterAs("PauseSoundPure", FLambda([=](FMeta meta) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "PauseSoundPure");
		CtrlRef->PauseSoundPure(); }));

	UEventContainer::RegisterAs("StopSoundPure", FLambda([=](FMeta meta) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "StopSoundPure");
		CtrlRef->StopSoundPure(); }));

	UEventContainer::RegisterAs("SwitchMedia", FLambda([=](FMeta meta)
	{
		CtrlRef->Switch(meta.MediaIndex);
	}));

	UEventContainer::RegisterAs("CallPlayer", FLambda([=](FMeta meta)
	{
		UEventContainer::Get("PopAllWidget")(meta);
		UEventContainer::Get("PushWidget")(meta);
		UEventContainer::Get("SwitchMedia")(meta);
	}));

	UEventContainer::RegisterAs("Mute", FLambda([=](FMeta meta) 
	{
		CtrlRef->Mute();
	}));

	UEventContainer::RegisterAs("Voice", FLambda([=](FMeta meta)
	{
		CtrlRef->Voice();
	}));

	UEventContainer::RegisterAs("Home", FLambda([=](FMeta meta)
	{
		UEventContainer::Get("PopAllWidget")(meta);
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "Home");
	}));

	UEventContainer::RegisterAs("QuitGame", FLambda([](FMeta meta) { 
		if (ensureMsgf(CtrlRef->GetWorld() != nullptr, TEXT("GetWorld is Null")))
		{
			CtrlRef->GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
		}
		
	}));

	UEventContainer::RegisterAs("Visible", FLambda([](FMeta meta) {
		auto widget = CtrlRef->Listener.Find(meta.RefComponent);
		if (widget != nullptr)
		{
			Cast<UWidget>(*widget)->SetVisibility(ESlateVisibility::Visible);
		}
	}));

	UEventContainer::RegisterAs("Collapsed", FLambda([](FMeta meta) {
		auto widget = CtrlRef->Listener.Find(meta.RefComponent);
		if (widget != nullptr)
		{
			Cast<UWidget>(*widget)->SetVisibility(ESlateVisibility::Collapsed);
		}
	}));

	UEventContainer::RegisterAs("WidgetSwitcher", FLambda([](FMeta meta) {
		auto Switcher = CtrlRef->Listener.Find(meta.RefComponent);
		auto Widget = CtrlRef->Listener.Find(meta.SwitcherWidetName);
		if (Switcher != nullptr && Widget != nullptr)
		{
			Cast<UCWidgetSwitcher>(*Switcher)->SetActiveWidget(*Widget);
		}
	}));

	UEventContainer::RegisterAs("Enable", FLambda([](FMeta meta) {
		auto widget = CtrlRef->Listener.Find(meta.RefComponent);
		if (widget != nullptr)
		{
			Cast<UWidget>(*widget)->SetIsEnabled(true);
		}
	}));

	UEventContainer::RegisterAs("Disable", FLambda([](FMeta meta) {
		auto widget = CtrlRef->Listener.Find(meta.RefComponent);
		if (widget != nullptr)
		{
			Cast<UWidget>(*widget)->SetIsEnabled(false);
		}
	}));

	UEventContainer::RegisterAs("DoClick", FLambda([](FMeta meta) {
		auto widget = CtrlRef->Listener.Find(meta.RefComponent);
		if (widget != nullptr)
		{
			if ((*widget)->IsA(UCButton::StaticClass()))
			{
				Cast<UCButton>(*widget)->OnClicked.Broadcast();
			}
			else if ((*widget)->IsA(UCImage::StaticClass()))
			{
				Cast<UCImage>(*widget)->OnLamdaEvent.Broadcast();
				Cast<UCImage>(*widget)->OnListenerEvent.Broadcast();
			}
		}
	}));
}
