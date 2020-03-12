// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "WidgetFactory.h"

#include "CButton.h"
#include "CImage.h"
#include "CSlider.h"
#include "CVerticalBox.h"
#include "CWidgetSwitcher.h"
#include "UMG.h"
#include "WidgetField.h"

TMap<TEnumAsByte<EWidget::Type>, TSubclassOf<UWidget>> UWidgetFactory::WidgetMap;

UWidgetFactory::UWidgetFactory()
{
	WidgetMap.Add(EWidget::CButton, UCButton::StaticClass());
	WidgetMap.Add(EWidget::CImage, UCImage::StaticClass());
	WidgetMap.Add(EWidget::CSlider, UCSlider::StaticClass());
	WidgetMap.Add(EWidget::CUVerticalBox, UCVerticalBox::StaticClass());
	WidgetMap.Add(EWidget::CWidgetSwitcher, UCWidgetSwitcher::StaticClass());
}

UWidget* UWidgetFactory::GetNew(UWidgetBase* UserWidget, FSQLQueryResultRow Meta)
{
	auto Type = FCString::Atoi( *Meta.Fields[EField::Type].Value);
	auto Name = Meta.Fields[EField::ObjectName].Value;

	auto Widget = UserWidget->WidgetTree->ConstructWidget<UWidget>(WidgetMap[static_cast<EWidget::Type>(Type)], FName(*Name));

	return Widget;
}