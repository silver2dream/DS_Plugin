// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "State.h"


// This function does not need to be modified.
UState::UState(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IState functions that are not pure virtual.
IState::IState()
{
	bRunBegin = false;
}

void IState::SetStateName(FString name)
{
	Statename = name;
}

FString IState::GetStateName()
{
	return Statename;
}

void IState::SetRunBegin(bool flag)
{
	bRunBegin = flag;
}

bool IState::IsRunBegin()
{
	return bRunBegin;
}

void IState::SetController(UStateController * ctrler)
{
	Controller = ctrler;
}
