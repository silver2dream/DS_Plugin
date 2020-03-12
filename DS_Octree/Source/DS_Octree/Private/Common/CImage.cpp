// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Materials/MaterialInterface.h"
#include "CImage.h"

#include "SlotProxy.h"

UCImage::UCImage()
{}

void UCImage::BindWith(TArray<FSQLKeyValuePair> Fields)
{
	FMeta EventMeta(Fields);
	
	OnMouseButtonDownEvent.BindUFunction(this, FName("BroadcastEvents"));
	
	OnLamdaEvent.AddLambda([=] { UEventContainer::Get(Fields[EField::Event_Name].Value)(EventMeta); });
	AddToOnListenerEvent(Fields);

	OnClick.AddDynamic(this, &UCImage::CallLamdaEvent);
	OnClick.AddDynamic(this, &UCImage::CallListenerEvent);

	SetBrushFromMaterial(GetAsset<UMaterialInterface>(Fields[EField::Normal_Path], Fields[EField::Normal_Name]));
	
	auto TransInfo = USlotProxy::GetTransformInfo(this, Fields[EField::Position], Fields[EField::Size]);
	USlotProxy::GetProxy(this->Slot)(TransInfo);

	SetVisibility(GetVisible(Fields[EField::Visible]));

	SetListener(Fields[EField::Listener]);
}

void UCImage::BroadcastEvents()
{
	OnClick.Broadcast();
}
