// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "MovieScene2DTransformTrackChain.h"

#include "CurveProxy.h"

void UMovieScene2DTransformTrackChain::Dispatch(UMovieScene* MovieScene, FGuid Guid, FAnimateParams Animateparams)
{
	if (!IsDuty(Animateparams.Track)) return NextHop->Dispatch(MovieScene, Guid, Animateparams);

	auto Track = MovieScene->AddTrack<UMovieScene2DTransformTrack>(Guid);
	FTransformProperty Property(Animateparams.PropertyPackge);
	Track->SetPropertyNameAndPath(Property.Name, Property.Path);

	auto Section = Cast<UMovieScene2DTransformSection>(Track->CreateNewSection());
	Section->SetEndTime(Animateparams.Duration);
	Track->AddSection(*Section);
	
	auto EvalOp = FCurveProxy::DispatchCurve<float>(Animateparams.CurveType);
	auto Lambda = [=](FRichCurve& RichCurve, float Start, float End) {
		for (auto CurrentTime = 0.f; CurrentTime < Animateparams.Duration; CurrentTime += 0.1f)
		{
			RichCurve.AddKey(CurrentTime, EvalOp(Start, End, CurrentTime / Animateparams.Duration));
		}
	};

	switch (Animateparams.AxisType)
	{
	case EAxisMode::XOnly:
		Lambda(Section->GetTranslationCurve(EAxis::X), Property.RenderTransform[EAnimateValue::Start].X, Property.RenderTransform[EAnimateValue::End].X);
		Lambda(Section->GetScaleCurve(EAxis::X), Property.Scale[EAnimateValue::Start].X, Property.Scale[EAnimateValue::End].X);
		Lambda(Section->GetShearCurve(EAxis::X), Property.Shear[EAnimateValue::Start].X, Property.Shear[EAnimateValue::End].X);
		break;
	case EAxisMode::YOnly:
		Lambda(Section->GetTranslationCurve(EAxis::Y), Property.RenderTransform[EAnimateValue::Start].Y, Property.RenderTransform[EAnimateValue::End].Y);
		Lambda(Section->GetScaleCurve(EAxis::Y), Property.Scale[EAnimateValue::Start].Y, Property.Scale[EAnimateValue::End].Y);
		Lambda(Section->GetShearCurve(EAxis::Y), Property.Shear[EAnimateValue::Start].Y, Property.Shear[EAnimateValue::End].Y);
		break;
	case EAxisMode::Both:
		Lambda(Section->GetTranslationCurve(EAxis::X), Property.RenderTransform[EAnimateValue::Start].X, Property.RenderTransform[EAnimateValue::End].X);
		Lambda(Section->GetScaleCurve(EAxis::X), Property.Scale[EAnimateValue::Start].X, Property.Scale[EAnimateValue::End].X);
		Lambda(Section->GetShearCurve(EAxis::X), Property.Shear[EAnimateValue::Start].X, Property.Shear[EAnimateValue::End].X);
		Lambda(Section->GetTranslationCurve(EAxis::Y), Property.RenderTransform[EAnimateValue::Start].Y, Property.RenderTransform[EAnimateValue::End].Y);
		Lambda(Section->GetScaleCurve(EAxis::Y), Property.Scale[EAnimateValue::Start].Y, Property.Scale[EAnimateValue::End].Y);
		Lambda(Section->GetShearCurve(EAxis::Y), Property.Shear[EAnimateValue::Start].Y, Property.Shear[EAnimateValue::End].Y);
		break;
	default:
		break;
	}

	Lambda(Section->GetRotationCurve(), Property.Rotation[EAnimateValue::Start], Property.Rotation[EAnimateValue::End]);

	if (Animateparams.IsDurationOverride())
	{
		MovieScene->SetPlaybackRange(0.f, Animateparams.Duration);
	}
}

bool UMovieScene2DTransformTrackChain::IsDuty(FString type)
{
	return type.Equals("MovieScene2DTransformTrack");
}

