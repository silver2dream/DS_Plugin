// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "AnimateSystemProxy.h"

#include "MovieScene2DTransformTrackChain.h"
#include "MovieSceneMarginTrackChain.h"
#include "MovieSceneColorTrackChain.h"

namespace EChain
{
	enum Value
	{
		First,
		Two,
		Three,
	};
}

UAnimateSystemProxy::UAnimateSystemProxy()
{
	TrackChainGroup.Add(NewObject<UMovieScene2DTransformTrackChain>(UMovieScene2DTransformTrackChain::StaticClass()));
	TrackChainGroup.Add(NewObject<UMovieSceneMarginTrackChain>(UMovieSceneMarginTrackChain::StaticClass()));
	TrackChainGroup.Add(NewObject<UMovieSceneColorTrackChain>(UMovieSceneColorTrackChain::StaticClass()));

	TrackChainGroup[EChain::First]->NextHop = TrackChainGroup[EChain::Two];
	TrackChainGroup[EChain::Two]->NextHop = TrackChainGroup[EChain::Three];

	DefalutTrackChain = TrackChainGroup[EChain::First];


}

void UAnimateSystemProxy::GenerateAnimate(class UMovieScene* MovieScene, FGuid Guid, FAnimateParams AnimateParams)
{
	DefalutTrackChain->Dispatch(MovieScene, Guid, AnimateParams);
}

