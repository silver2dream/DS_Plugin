// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Lambda.h"


// This function does not need to be modified.
ULambda::ULambda(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any ILambda functions that are not pure virtual.

void ILambda::CallLamdaEvent()
{
	OnLamdaEvent.Broadcast();
}
