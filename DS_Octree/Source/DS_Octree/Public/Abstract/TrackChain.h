// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimateParam.h"
#include "UObject/NoExportTypes.h"
#include "TrackChain.generated.h"

class UMovieScene;

/**
 * 
 */
UCLASS(abstract)
class DS_OCTREE_API UTrackChain : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool IsDuty(FString type) PURE_VIRTUAL(IChain::IsDuty, return false;);

	virtual void Dispatch(UMovieScene* MovieScene, FGuid Guid, FAnimateParams Animateparams) PURE_VIRTUAL(UTrackChain::Dispatch, void;);
	
	UPROPERTY()
	UTrackChain* NextHop;

};
