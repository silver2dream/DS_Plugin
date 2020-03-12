// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Lambda.h"
#include "Binder.h"
#include "Listener.h"
#include "Components/Button.h"
#include "CButton.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCButton : public UButton, public ILambda, public IBinder, public IListener
{
	GENERATED_BODY()

public:

	UCButton();
	
	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) override;
};
