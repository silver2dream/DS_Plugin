// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WidgetField.h"
#include "SQL.h"
#include "Binder.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBinder : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API IBinder
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) PURE_VIRTUAL(IBinder::BindTogether, void;);

};
