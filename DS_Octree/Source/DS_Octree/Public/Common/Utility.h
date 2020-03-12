// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SQLiteDatabase.h"

namespace Utility
{
	static TOptional<FVector2D> GetVector2D(FSQLKeyValuePair data)
	{
		TArray<FString> vector;
		data.Value.ParseIntoArray(vector, TEXT(","), true);
		if (vector.Num() < 2) return FVector2D::ZeroVector;
		TOptional<FVector2D> V2D = FVector2D(FCString::Atof(*vector[0]), FCString::Atof(*vector[1]));
		return V2D;
	}

	template<typename EType = ESlateVisibility>
	static EType GetVisible(FSQLKeyValuePair data)
	{
		uint8 Value = FCString::Atoi(*data.Value);
		auto Visible = (Value ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		return static_cast<EType>(Visible);
	}

	template<typename AssetType = UObject>
	static AssetType* GetAsset(FSQLKeyValuePair path, FSQLKeyValuePair name)
	{
		FStringAssetReference AssetRef = path.Value.Append(name.Value);
		auto Asset = AssetRef.TryLoad();
		if (!Asset) return nullptr;
		return Cast<AssetType>(Asset);
	}

	template<typename AssetType = UObject>
	static AssetType* GetAssetByPath(FString path)
	{
		FStringAssetReference AssetRef = path;
		auto Asset = AssetRef.TryLoad();
		if (!Asset) return nullptr;
		return Cast<AssetType>(Asset);
	}

	static TMap<FString,FString> GetComposite(FSQLKeyValuePair Composites, FSQLKeyValuePair CompositesEvent)
	{
		TArray<FString> CompositesGroup;
		Composites.Value.ParseIntoArray(CompositesGroup, TEXT(","), true);

		TArray<FString> EventGroup;
		CompositesEvent.Value.ParseIntoArray(EventGroup, TEXT(","), true);

		TMap<FString, FString> CompositeMap;
		if (CompositesGroup.Num() != EventGroup.Num() ) return CompositeMap;

		for (auto index = 0; index < CompositesGroup.Num(); ++index)
		{
			CompositeMap.Add(CompositesGroup[index], EventGroup[index]);
		}
		
		return CompositeMap;
	}

	template<typename RetVal>
	struct FValueGenerater
	{
		static RetVal ParseString(FString Value)
		{
			return RetVal();
		};
	};

	template<>
	struct FValueGenerater<float>
	{
		static float ParseString(FString Value)
		{
			auto fValue = FCString::Atof(*Value);
			return fValue;
		};
	};

	template<>
	struct FValueGenerater<FVector2D>
	{
		static FVector2D ParseString(FString Value)
		{
			TArray<FString> vector;
			Value.ParseIntoArray(vector, TEXT(","), true);
			if (vector.Num() < 2) return FVector2D::ZeroVector;
			FVector2D V2D = FVector2D(FCString::Atof(*vector[0]), FCString::Atof(*vector[1]));
			return V2D;
		};
	};

	template<>
	struct FValueGenerater<FVector>
	{
		static FVector ParseString(FString Value)
		{
			TArray<FString> vector;
			Value.ParseIntoArray(vector, TEXT(","), true);
			if (vector.Num() < 3) return FVector::ZeroVector;
			FVector V3D = FVector(FCString::Atof(*vector[0]), FCString::Atof(*vector[1]), FCString::Atof(*vector[2]));
			return V3D;
		};
	};

	template<>
	struct FValueGenerater<FPlane>
	{
		static FPlane ParseString(FString Value)
		{
			TArray<FString> vector;
			Value.ParseIntoArray(vector, TEXT(","), true);
			if (vector.Num() < 4) return FPlane::ZeroVector;
			FPlane plane = FPlane(FCString::Atof(*vector[0]), FCString::Atof(*vector[1]), FCString::Atof(*vector[2]), FCString::Atof(*vector[3]));
			return plane;
		};
	};

	template<typename T>
	static void ParseAnimateValue(TArray<T>& VectorGroup, FString* Value)
	{
		if (!Value) return;

		TArray<FString> Result;
		(*Value).ParseIntoArray(Result, TEXT(";"), true);
		for (auto ValueString : Result)
		{
			VectorGroup.Add(FValueGenerater<T>::ParseString(ValueString));
		}
	}
}

