// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CVerticalBox.h"

#include "SlotProxy.h"

UCVerticalBox::UCVerticalBox()
{}

void UCVerticalBox::BindWith(TArray<FSQLKeyValuePair> Fields)
{
	auto TransInfo = USlotProxy::GetTransformInfo(this, Fields[EField::Position], Fields[EField::Size]);
	USlotProxy::GetProxy(this->Slot)(TransInfo);
}