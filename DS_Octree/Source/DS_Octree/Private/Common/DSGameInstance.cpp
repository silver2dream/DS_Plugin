// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "DSGameInstance.h"

#include "Bidder.h"

UDSGameInstance::UDSGameInstance()
{
	/*Create Octree*/
	//Octree = MakeUnique<FDSOctree>(FVector::ZeroVector, HALF_WORLD_MAX);
	Bidder = nullptr;
}

int32 UDSGameInstance::GetBidderId()
{
	if (Bidder)
	{
		const auto Id = Bidder->Id;
		return Id;
	}
	return 0;
}

bool UDSGameInstance::IsLogined()
{
	if (GetBidderId() != NULL)
	{
		return true;
	}
	return false;
}


