// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abstract/TrackChain.h"
#include "MovieScene2DTransformTrackChain.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UMovieScene2DTransformTrackChain : public UTrackChain
{
	GENERATED_BODY()
	
public:
	virtual void Dispatch(UMovieScene* MovieScene, FGuid Guid, FAnimateParams Animateparams) override;

	virtual bool IsDuty(FString type) override;
};
