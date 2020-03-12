// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "Lambda.h"
#include "Listener.h"
#include "Binder.h"
#include "CImage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMouseClickDelegate);

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCImage : public UImage, public ILambda, public IBinder, public IListener
{
	GENERATED_BODY()
	
public:

	UCImage();
	
	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) override;

private:

	FMouseClickDelegate OnClick;

	UFUNCTION()
	void BroadcastEvents();
};
