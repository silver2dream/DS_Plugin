// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "Lambda.h"
#include "Binder.h"
#include "CTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UCTextBlock : public UTextBlock, public ILambda, public IBinder
{
	GENERATED_BODY()
	
public:
	UCTextBlock();

	virtual void BindWith(TArray<FSQLKeyValuePair> Fields) override;

};
