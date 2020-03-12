// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Slider.h"
#include "Lambda.h"
#include "Binder.h"
#include "CSlider.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCSlider : public USlider, public ILambda, public IBinder
{
	GENERATED_BODY()
	
public:
	UCSlider();
	
	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) override;

	virtual void OnValueChange(float time);
};
