// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Bidder.h"
#include "CustomActor.h"
#include "DSGameInstance.generated.h"

USTRUCT()
struct FOctreeSemantics
{
	GENERATED_USTRUCT_BODY()
public:
	enum { MaxElementsPerLeaf = 16 };
	enum { MinInclusiveElementsPerNode = 7 };
	enum { MaxNodeDepth = 10 };

	typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

	FORCEINLINE static FBoxCenterAndExtent GetBoundingBox(ACustomActor* Actor)
	{
		return FBoxCenterAndExtent(Actor->GetActorLocation(), Actor->BoxComponent->GetUnscaledBoxExtent());
	}

	FORCEINLINE static void SetElementId(ACustomActor* Actor, FOctreeElementId Id)
	{
		Actor->OctreeId = Id;
	}
};

typedef TOctree<ACustomActor*, FOctreeSemantics> FDSOctree;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UDSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UDSGameInstance();
	
	int32 GetBidderId();

	//TUniquePtr<FDSOctree> Octree;

	UPROPERTY()
	UBidder* Bidder;

	UPROPERTY()
	UTextureRenderTarget2D* renderTarget2D;

	bool IsLogined();

	UPROPERTY()
	class UVideoPlayer* VideoPlayer;

	UPROPERTY()
	class UAudioPlayer* AudioPlayer;
};
