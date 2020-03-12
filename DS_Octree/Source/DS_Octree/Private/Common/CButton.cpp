// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CButton.h"
#include "TextBlock.h"
#include "EventContainer.h"

#include "SlotProxy.h"

UCButton::UCButton()
{
}

void UCButton::BindWith(TArray<FSQLKeyValuePair> Fields)
{
	FMeta EventMeta(Fields);
	OnLamdaEvent.AddLambda([=] { UEventContainer::Get(Fields[EField::Event_Name].Value)(EventMeta); });
	OnClicked.AddDynamic(this, &UCButton::CallLamdaEvent);
		
	AddToOnListenerEvent(Fields);
	OnClicked.AddDynamic(this, &UCButton::CallListenerEvent);

	auto TransInfo = USlotProxy::GetTransformInfo(this, Fields[EField::Position], Fields[EField::Size]);
	USlotProxy::GetProxy(this->Slot)(TransInfo);

	FButtonStyle Style;
	Style.Normal.SetResourceObject(GetAsset(Fields[EField::Normal_Path], Fields[EField::Normal_Name]));
	Style.Hovered.SetResourceObject(GetAsset(Fields[EField::Hovered_Path], Fields[EField::Hovered_Name]));
	Style.Pressed.SetResourceObject(GetAsset(Fields[EField::Pressed_Path], Fields[EField::Pressed_Name]));
	SetStyle(Style);
	
	if (!Fields[EField::ObjectName].Value.Contains(TEXT(".NA")))
	{
		UTextBlock* Text = NewObject<UTextBlock>(UTextBlock::StaticClass());
		Text->SetText(FText::FromString(Fields[EField::ObjectName].Value));
		Text->Font.Size = 16;
		AddChild(Text);
	}
	
	SetVisibility(GetVisible(Fields[EField::Visible]));

	SetListener(Fields[EField::Listener]);
}