// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetSwitcher.h"
#include "Listener.h"
#include "Binder.h"
#include "CWidgetSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCWidgetSwitcher : public UWidgetSwitcher,public IListener, public IBinder
{
	GENERATED_BODY()
	
public:
	UCWidgetSwitcher();

	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) override;
};
