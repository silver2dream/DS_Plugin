// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "SlotProxy.h"

#include "CanvasPanelSlot.h"
#include "WidgetSwitcherSlot.h"
#include "VerticalBoxSlot.h"
#include "HorizontalBoxSlot.h"
#include "Widget.h"

#include "Utility.h"

using namespace Utility;

TMap<TSubclassOf<class UPanelSlot>, TFunction<void(FTransformInfo)>> USlotProxy::ProxyMap;

USlotProxy::USlotProxy()
{
	ProxyMap.Add(UVerticalBoxSlot::StaticClass(), [=](FTransformInfo Info) {
		auto VerticalBoxSlot = Cast<UVerticalBoxSlot>(Info.WidgetRef->Slot);
		VerticalBoxSlot->SetHorizontalAlignment(Info.HorizontalAlignment);
		VerticalBoxSlot->SetVerticalAlignment(Info.VerticalAlignment);

		FSlateChildSize Size;
		Size.SizeRule = Info.SlateSizeRule;
		VerticalBoxSlot->SetSize(Size);
	});

	ProxyMap.Add(UHorizontalBoxSlot::StaticClass(), [=](FTransformInfo Info) {
		auto HorizationBoxSlot = Cast<UHorizontalBoxSlot>(Info.WidgetRef->Slot);
		HorizationBoxSlot->SetHorizontalAlignment(Info.HorizontalAlignment);
		HorizationBoxSlot->SetVerticalAlignment(Info.VerticalAlignment);

		FSlateChildSize Size;
		Size.SizeRule = Info.SlateSizeRule;
		HorizationBoxSlot->SetSize(Size);
	});

	ProxyMap.Add(UWidgetSwitcherSlot::StaticClass(), [=](FTransformInfo Info) {
		auto WidgetSwitcherSlot = Cast<UWidgetSwitcherSlot>(Info.WidgetRef->Slot);
		WidgetSwitcherSlot->SetHorizontalAlignment(Info.HorizontalAlignment);
		WidgetSwitcherSlot->SetVerticalAlignment(Info.VerticalAlignment);

	});

	ProxyMap.Add(UCanvasPanelSlot::StaticClass(), [=](FTransformInfo Info) {
		auto CanvasPanelSlot = Cast<UCanvasPanelSlot>(Info.WidgetRef->Slot);
		CanvasPanelSlot->SetPosition(Info.Position);
		CanvasPanelSlot->SetSize(Info.Size);
	});
}

FTransformInfo USlotProxy::GetTransformInfo(UWidget* Widget, FSQLKeyValuePair Position, FSQLKeyValuePair Size)
{
	if (IsCustomStyle(Widget->Slot))
	{
		return FTransformInfo(Widget, GetVector2D(Position).GetValue(), GetVector2D(Size).GetValue());
	}

	TArray<FString> PosStlye;
	Position.Value.ParseIntoArray(PosStlye, TEXT(","), true);
	
	return FTransformInfo(Widget, static_cast<EHorizontalAlignment>(FCString::Atoi(*PosStlye[0]))
								, static_cast<EVerticalAlignment>(FCString::Atoi(*PosStlye[1]))
								, static_cast<ESlateSizeRule::Type>(FCString::Atoi(*Size.Value)));
}

bool USlotProxy::IsCustomStyle(class UPanelSlot* Slot)
{
	if (Slot->GetClass() == UCanvasPanelSlot::StaticClass()) return true;

	return false;
}

TFunction<void(FTransformInfo)> USlotProxy::GetProxy(class UPanelSlot* Slot)
{
	auto Proxy = ProxyMap.Find(Slot->GetClass());
	return *Proxy;
}
