// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Register.h"
#include "EventContainer.h"

// This function does not need to be modified.
URegister::URegister(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IRegister functions that are not pure virtual.
TWeakObjectPtr<UStateController> IRegister::CtrlRef = nullptr;