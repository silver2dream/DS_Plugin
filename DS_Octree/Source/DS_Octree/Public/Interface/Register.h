// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Register.generated.h"

class UWidgetCtrl;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URegister : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API IRegister
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	virtual void RegisterUniversalLambda() PURE_VIRTUAL(IRegister::RegisterUniversalLambda, void;);
	
	static TWeakObjectPtr<UStateController> CtrlRef;
};
