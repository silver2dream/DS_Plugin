// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "WidgetFactory.generated.h"

enum EWidget::Type;
struct FSQLQueryResultRow;
class UWidgetBase;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UWidgetFactory : public UObject
{
	GENERATED_BODY()
	
public:
	UWidgetFactory();

	static UWidget* GetNew(UWidgetBase* UserWidget, FSQLQueryResultRow Meta);

private:
	static TMap<TEnumAsByte<EWidget::Type>, TSubclassOf<UWidget>> WidgetMap;
};
