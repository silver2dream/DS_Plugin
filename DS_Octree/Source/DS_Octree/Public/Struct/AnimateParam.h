// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimateEnum.h"
#include "Utility.h"
using namespace Utility;


struct FAnimateProperty
{

public:
	FAnimateProperty()
	{}

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FString Path;

};

struct FTransformProperty : public FAnimateProperty
{
	FTransformProperty() {}

	FTransformProperty(TMap<FString, FString> ValueMap)
	{
		Name = FName(**ValueMap.Find("Name"));
		Path = Name.ToString();

		ParseAnimateValue(RenderTransform, ValueMap.Find("RenderTransform"));
		ParseAnimateValue(Scale, ValueMap.Find("Scale"));
		ParseAnimateValue(Shear, ValueMap.Find("Shear"));
		ParseAnimateValue(Rotation, ValueMap.Find("Rotation"));
	}

	UPROPERTY()
	TArray<FVector2D> RenderTransform;

	UPROPERTY()
	TArray<float> Rotation;

	UPROPERTY()
	TArray<FVector2D> Scale;

	UPROPERTY()
	TArray<FVector2D> Shear;

	FString ToString(EAnimateValue::Type Type)
	{
		FString TransformString;
		TransformString.Append(FString::Printf(TEXT("%.2f,%.2f;"), RenderTransform[Type].X, RenderTransform[Type].Y));
		TransformString.Append(FString::Printf(TEXT("%.2f,%.2f;"), Scale[Type].X, Scale[Type].Y));
		TransformString.Append(FString::Printf(TEXT("%.2f;"), Rotation[Type]));
		TransformString.Append(FString::Printf(TEXT("%.2f,%.2f;"), Shear[Type].X, Shear[Type].Y));

		return TransformString;
	}
};

struct FMargiProperty : public FAnimateProperty
{
	FMargiProperty() {}

	FMargiProperty(TMap<FString, FString> ValueMap)
	{
		Name = FName(**ValueMap.Find("Name"));
		Path = FString("LayoutData.Offsets");
		
		ParseAnimateValue(Plane, ValueMap.Find("Offsets"));
	}

	UPROPERTY()
	TArray<FPlane> Plane;

	FString ToString(EAnimateValue::Type Type)
	{
		FString MargiString;
		MargiString.Append(FString::SanitizeFloat(Plane[Type].X).Append(","));
		MargiString.Append(FString::SanitizeFloat(Plane[Type].Y).Append(","));
		MargiString.Append(FString::SanitizeFloat(Plane[Type].Z).Append(","));
		MargiString.Append(FString::SanitizeFloat(Plane[Type].W).Append(";"));
		return MargiString;
	}
};

struct FColorProperty : public FAnimateProperty
{
	FColorProperty() {}

	FColorProperty(TMap<FString, FString> ValueMap)
	{
		Name = FName(**ValueMap.Find("Name"));
		Path = Name.ToString();

		ParseAnimateValue(Plane, ValueMap.Find(Name.ToString()));
	}

	UPROPERTY()
	TArray<FPlane> Plane;

	FString ToString(EAnimateValue::Type Type)
	{
		FString ColorString;
		ColorString.Append(FString::SanitizeFloat(Plane[Type].X).Append(","));
		ColorString.Append(FString::SanitizeFloat(Plane[Type].Y).Append(","));
		ColorString.Append(FString::SanitizeFloat(Plane[Type].Z).Append(","));
		ColorString.Append(FString::SanitizeFloat(Plane[Type].W).Append(";"));
		return ColorString;
	}
};

struct FAnimateParams
{
public:
	FAnimateParams() {}

	FAnimateParams(FSQLQueryResultRow meta)
	{
		ID = meta.Fields[EAnimateMeta::ID].Value;
		AnimateName = meta.Fields[EAnimateMeta::Name].Value;
		Track = meta.Fields[EAnimateMeta::TrackType].Value;
		DurationOverride = FCString::Atoi(*meta.Fields[EAnimateMeta::DurationOverride].Value);
		AxisType = static_cast<EAxisMode::Type>(FCString::Atoi(*meta.Fields[EAnimateMeta::AxisType].Value));
		CurveType = static_cast<ECurve::Type> (FCString::Atoi(*meta.Fields[EAnimateMeta::CurveType].Value));
		Duration = FCString::Atof(*meta.Fields[EAnimateMeta::Duratiom].Value);
		Begin = meta.Fields[EAnimateMeta::Begin].Value;
		End = meta.Fields[EAnimateMeta::End].Value;
		PropertyPackge = ParseProperty(meta.Fields[EAnimateMeta::Property].Value);
	}

	UPROPERTY()
	FString ID;

	UPROPERTY()
	FString AnimateName;

	UPROPERTY()
	FString Track;

	UPROPERTY()
	int32 DurationOverride;

	UPROPERTY()
	TMap<FString, FString> PropertyPackge;

	UPROPERTY()
	TEnumAsByte<EAxisMode::Type>  AxisType;

	UPROPERTY()
	TEnumAsByte<ECurve::Type> CurveType;

	UPROPERTY()
	float Duration;

	UPROPERTY()
	FString Begin;

	UPROPERTY()
	FString	End;

	bool IsTrackNone()
	{
		return Track.Equals("None");
	}

	bool IsDurationOverride()
	{
		return DurationOverride && 1;
	}

	FString GetPropertyName()
	{
		return PropertyPackge["Name"];
	}

protected:
	TMap<FString, FString> ParseProperty(FString PropertyString)
	{
		TMap<FString, FString> ValueyMap;
		//if (PropertyString.Equals("None")) return ValueyMap;

		TArray<FString> PropertyGroup;
		PropertyString.ParseIntoArray(PropertyGroup, TEXT(";"), true);

		ValueyMap.Add("Name", PropertyGroup[0]); // todo need more check

		TArray<FString> BeginGroup;
		Begin.ParseIntoArray(BeginGroup, TEXT(";"), true);

		TArray<FString> EndGroup;
		End.ParseIntoArray(EndGroup, TEXT(";"), true);

		if (EndGroup.Num() != BeginGroup.Num()) return ValueyMap;

		for (auto index = 0; index < PropertyGroup.Num(); ++index)
		{
			ValueyMap.Add(PropertyGroup[index], FString::Printf(TEXT("%s;%s"), *BeginGroup[index], *EndGroup[index]));
		}

		return ValueyMap;
	}
};