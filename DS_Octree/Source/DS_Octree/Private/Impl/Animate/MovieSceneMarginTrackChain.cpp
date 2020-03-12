// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "MovieSceneMarginTrackChain.h"

#include "MovieSceneMarginTrack.h"
#include "MovieSceneMarginSection.h"

void UMovieSceneMarginTrackChain::Dispatch(UMovieScene* MovieScene, FGuid Guid, FAnimateParams Animateparams)
{
	if (!IsDuty(Animateparams.Track)) return NextHop->Dispatch(MovieScene, Guid, Animateparams); 

	auto Track = MovieScene->AddTrack<UMovieSceneMarginTrack>(Guid);
	FMargiProperty Property(Animateparams.PropertyPackge);
	Track->SetPropertyNameAndPath(Property.Name, Property.Path);

	auto Section = Cast<UMovieSceneMarginSection>(Track->CreateNewSection());
	Section->SetEndTime(Animateparams.Duration);
	Track->AddSection(*Section);

	auto EvalOp = FCurveProxy::DispatchCurve<float>(Animateparams.CurveType);
	for (auto CurrentTime = 0.f; CurrentTime <= Animateparams.Duration ; CurrentTime += 0.1f)
	{
		Section->GetLeftCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].X, Property.Plane[EAnimateValue::End].X, CurrentTime / Animateparams.Duration));
		Section->GetTopCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].Y, Property.Plane[EAnimateValue::End].Y, CurrentTime / Animateparams.Duration));
		Section->GetRightCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].Z, Property.Plane[EAnimateValue::End].Z, CurrentTime / Animateparams.Duration));
		Section->GetBottomCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].W, Property.Plane[EAnimateValue::End].W, CurrentTime / Animateparams.Duration));
	}
	
	if (Animateparams.IsDurationOverride())
	{
		MovieScene->SetPlaybackRange(0.f, Animateparams.Duration);
	}
}

bool UMovieSceneMarginTrackChain::IsDuty(FString type)
{
	return type.Equals("MovieSceneMarginTrack");
}