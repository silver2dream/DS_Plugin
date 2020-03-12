// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Listener.generated.h"

DECLARE_EVENT(UListener, FListenerEvent);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UListener : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API IListener
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FListenerEvent OnListenerEvent;
	
	UFUNCTION()
	virtual void CallListenerEvent();

	virtual void AddToOnListenerEvent(TArray<FSQLKeyValuePair> Fields);

	virtual void SetListener(FSQLKeyValuePair data);

	bool bListener = false;
};
