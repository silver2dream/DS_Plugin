// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
namespace EAnimateValue
{
	enum Type
	{
		Start,
		End,
	};
}

UENUM()
namespace EAnimateMeta
{
	enum Value
	{
		ID,
		Name,
		TrackType,
		DurationOverride,
		Property,
		Duratiom,
		AxisType,
		CurveType,
		Begin,
		End,
	};
}

UENUM()
namespace ETrackType
{
	enum Type
	{
		None,
		MovieScene2DTransformTrack,
		MovieSceneMarginTrack,
		MovieSceneColorTrack,
	};
}

UENUM()
namespace ESectionType
{
	enum Type
	{
		MovieScene2DTransformSection,
		MovieScene3DTransformSection,
	};
}

UENUM()
namespace EAxisMode
{
	enum Type
	{
		XOnly,
		YOnly,
		Both,
	};
}

UENUM()
namespace ECurve
{
	enum Type
	{
		Lerp,
		InterpSinIn,
		InterpSinOut,
		InterpSinInOut,
		InterpExpoIn,
		InterpExpoOut,
		InterpExpoInOut,
		InterpCircularIn,
		InterpCircularOut,
		InterpCircularInOut,
	};
}