// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Listener.h"
#include "SQLiteDatabase.h"

// This function does not need to be modified.
UListener::UListener(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IListener functions that are not pure virtual.

void IListener::CallListenerEvent()
{
	OnListenerEvent.Broadcast();
}

void IListener::AddToOnListenerEvent(TArray<FSQLKeyValuePair> Fields)
{
	auto CompositeMap = GetComposite(Fields[EField::Composite], Fields[EField::CompositeEvent]);
	for (auto Pair : CompositeMap)
	{
		auto Widget = Pair.Key;
		FMeta ChildMeta;
		ChildMeta.RefComponent = Widget;
		ChildMeta.SwitcherWidetName = Fields[EField::SwitcherWidgetName].Value;
		ChildMeta.MediaIndex = FCString::Atoi(*Fields[EField::MediaIndex].Value);
		OnListenerEvent.AddLambda([=] {UEventContainer::Get(Pair.Value)(ChildMeta); });
	}
}

void IListener::SetListener(FSQLKeyValuePair data)
{
	bListener = FCString::ToBool(*data.Value);
}
