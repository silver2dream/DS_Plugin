// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Binder.h"
#include "EventContainer.generated.h"

struct FMeta
{
public:
	FMeta()
	{}

	FMeta(TArray<FSQLKeyValuePair> Fields)
	{
		WidgetName = *Fields[EField::Widget_Name].Value;
		MediaIndex = FCString::Atoi(*Fields[EField::MediaIndex].Value);
	}

	FName WidgetName;
	FString RefComponent;
	FString SwitcherWidetName;
	int MediaIndex;
};


struct FLambda
{
public:
	FLambda() {}

	template<typename TFuncObj>
	FLambda(TFuncObj&& FuncObj)
	{
		Buf = Forward<TFuncObj>(FuncObj);
	}

	template<typename Meta>
	void operator()(Meta meta)
	{
		Buf(std::forward<Meta>(meta));
	}
	
private:
	TFunction<void(FMeta)> Buf;
};

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UEventContainer : public UObject
{
	GENERATED_BODY()
	
public:
	static void RegisterAs(FString typeName, FLambda&& fptr)
	{
		if (EventTypeMap.Contains(typeName))
			return;

		EventTypeMap.Add(typeName) = fptr;
	}

	static FLambda& Get(FString eventName)
	{
		return EventTypeMap[eventName];
	}

private:
	static TMap<FString, FLambda > EventTypeMap;

};
