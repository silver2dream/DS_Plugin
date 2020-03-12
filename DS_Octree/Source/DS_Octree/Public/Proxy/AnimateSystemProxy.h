// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "AnimateParam.h"
#include "AnimateSystemProxy.generated.h"

class UTrackChain;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UAnimateSystemProxy : public UObject
{
	GENERATED_BODY()
	
public:
	UAnimateSystemProxy();

	void GenerateAnimate(class UMovieScene* MovieScene, FGuid Guid, FAnimateParams AnimateParams);
	
private:
	TArray<class UTrackChain*> TrackChainGroup;

	UTrackChain* DefalutTrackChain;
};
