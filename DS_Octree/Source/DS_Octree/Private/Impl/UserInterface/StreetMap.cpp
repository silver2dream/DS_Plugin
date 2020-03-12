// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "StreetMap.h"




void UStreetMap::BindTask(UWidgetCtrl* widgetCtrler)
{
	ProcessComponents(widgetCtrler);
}

bool UStreetMap::Initialize()
{
	auto Successful = Super::Initialize();
	if (!Successful) return false;

	return Successful;
}
