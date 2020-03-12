// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "MovieSceneColorTrackChain.h"

#include "MovieSceneColorTrack.h"
#include "MovieSceneColorSection.h"


void UMovieSceneColorTrackChain::Dispatch(UMovieScene* MovieScene, FGuid Guid, FAnimateParams Animateparams)
{
	if (!IsDuty(Animateparams.Track)) return NextHop->Dispatch(MovieScene, Guid, Animateparams);

	auto Track = MovieScene->AddTrack<UMovieSceneColorTrack>(Guid);
	FColorProperty Property(Animateparams.PropertyPackge);
	Track->SetPropertyNameAndPath(Property.Name, Property.Path);

	auto Section = Cast<UMovieSceneColorSection>(Track->CreateNewSection());
	Section->SetEndTime(Animateparams.Duration);
	Track->AddSection(*Section);

	auto EvalOp = FCurveProxy::DispatchCurve<float>(Animateparams.CurveType);
	for (auto CurrentTime = 0.f; CurrentTime <= Animateparams.Duration; CurrentTime += 0.1f)
	{
		Section->GetRedCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].X, Property.Plane[EAnimateValue::End].X, CurrentTime / Animateparams.Duration));
		Section->GetGreenCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].Y, Property.Plane[EAnimateValue::End].Y, CurrentTime / Animateparams.Duration));
		Section->GetBlueCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].Z, Property.Plane[EAnimateValue::End].Z, CurrentTime / Animateparams.Duration));
		Section->GetAlphaCurve().AddKey(CurrentTime, EvalOp(Property.Plane[EAnimateValue::Start].W, Property.Plane[EAnimateValue::End].W, CurrentTime / Animateparams.Duration));
	}
	
	if (Animateparams.IsDurationOverride())
	{
		MovieScene->SetPlaybackRange(0.f, Animateparams.Duration);
	}
}

bool UMovieSceneColorTrackChain::IsDuty(FString type)
{
	return type.Equals("MovieSceneColorTrack");
}
