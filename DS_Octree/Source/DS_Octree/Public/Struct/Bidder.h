// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Bidder.generated.h"

UENUM()
namespace EPurchase
{
	enum Field
	{
		Id,
		Path,
		MaterialName,
		ObjectName,
	};
}

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UBidder : public UObject
{
	GENERATED_BODY()
public:
	UBidder();
	
	UPROPERTY()
	int32 Id;
	
	UPROPERTY()
	FString Account;
	
	UPROPERTY()
	FString Password;
	
	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	FString Phone;
	
	UPROPERTY()
	FString Address;
	
	UPROPERTY()
	int32 Total;
};
