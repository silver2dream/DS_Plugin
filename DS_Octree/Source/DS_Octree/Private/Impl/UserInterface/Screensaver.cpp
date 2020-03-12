// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Screensaver.h"

#include "CImage.h"
#include "WidgetTree.h"
#include "PanelWidget.h"
#include "CanvasPanelSlot.h"

#include "EventContainer.h"

void UScreensaver::BindTask(UWidgetCtrl* widgetCtrler)
{
	ProcessComponents(widgetCtrler);
	
	//widgetCtrler->PushWidget(this);
}

bool UScreensaver::Initialize()
{
	auto Successful = Super::Initialize();
	if (!Successful) return false;

	return true;
}
