// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Lambda.generated.h"

DECLARE_EVENT(ULambda, FLamdaEvent);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULambda : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API ILambda
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FLamdaEvent OnLamdaEvent;

	UFUNCTION()
	virtual void CallLamdaEvent();
};
