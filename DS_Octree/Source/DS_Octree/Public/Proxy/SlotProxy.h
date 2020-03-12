// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "SlotProxy.generated.h"

class UWidget;

USTRUCT()
struct FTransformInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FTransformInfo() {}

	FTransformInfo(UWidget* Widget, FVector2D position, FVector2D size)
		: WidgetRef(Widget)
		, Position(position)
		, Size(size)
	{}

	FTransformInfo(UWidget* Widget, EHorizontalAlignment Hstyle, EVerticalAlignment Vstyle, ESlateSizeRule::Type SizeRule)
		: WidgetRef(Widget)
		, HorizontalAlignment(Hstyle)
		, VerticalAlignment(Vstyle)
		, SlateSizeRule(SizeRule)
	{}

	UPROPERTY()
	UWidget* WidgetRef;

	UPROPERTY()
	FVector2D Position;

	UPROPERTY()
	FVector2D Size;

	UPROPERTY()
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UPROPERTY()
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	UPROPERTY()
	TEnumAsByte<ESlateSizeRule::Type> SlateSizeRule;
};


/**
 * 
 */
UCLASS()
class DS_OCTREE_API USlotProxy : public UObject
{
	GENERATED_BODY()
	
public:
	USlotProxy();

	static FTransformInfo GetTransformInfo(UWidget* Widget, FSQLKeyValuePair Position, FSQLKeyValuePair Size);
	
	static bool IsCustomStyle(class UPanelSlot* Slot);

	static TFunction<void(FTransformInfo)> GetProxy(class UPanelSlot* Slot);

private:
	static TMap<TSubclassOf<UPanelSlot>, TFunction<void(FTransformInfo)>> ProxyMap;
};
