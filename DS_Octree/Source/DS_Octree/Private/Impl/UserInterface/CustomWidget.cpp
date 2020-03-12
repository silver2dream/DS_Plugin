// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CustomWidget.h"



void UCustomWidget::BindTask(UWidgetCtrl* widgetCtrler)
{
	ProcessComponents(widgetCtrler);
	UE_LOG(LogTemp, Warning, TEXT("CustomWidget BindTask"));
}
