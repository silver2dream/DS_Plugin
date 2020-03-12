// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Prodution.generated.h"

UENUM()
namespace EProduction
{
	enum Field
	{
		Id,
		Type,
		ItemName,
		MeshName,
		MaterialName,
		Path,
		Price,
	};
}


/**
 * 
 */
UCLASS()
class DS_OCTREE_API UProdution : public UObject
{
	GENERATED_BODY()
public:
	UProdution();
	
	UPROPERTY()
	int32 Id;

	UPROPERTY()
	int32 Type;

	UPROPERTY()
	FString ItemName;
	
	UPROPERTY()
	FString MeshName;
	
	UPROPERTY()
	FString MaterialName;
	
	UPROPERTY()
	FString Path;
	
	UPROPERTY()
	int32 Price;

	FString PrintInfom();
};
