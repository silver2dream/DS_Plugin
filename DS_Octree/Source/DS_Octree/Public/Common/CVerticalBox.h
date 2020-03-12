// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/VerticalBox.h"
#include "Listener.h"
#include "Binder.h"
#include "CVerticalBox.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCVerticalBox : public UVerticalBox, public IBinder, public IListener
{
	GENERATED_BODY()
	
public:
	UCVerticalBox();

	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) override;
	
};
