// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CWidgetSwitcher.h"
#include "CanvasPanelSlot.h"

#include "SlotProxy.h"

UCWidgetSwitcher::UCWidgetSwitcher()
{}

void UCWidgetSwitcher::BindWith(TArray<FSQLKeyValuePair> Fields)
{
	auto TransInfo = USlotProxy::GetTransformInfo(this, Fields[EField::Position], Fields[EField::Size]);
	USlotProxy::GetProxy(this->Slot)(TransInfo);

	SetListener(Fields[EField::Listener]);
}
