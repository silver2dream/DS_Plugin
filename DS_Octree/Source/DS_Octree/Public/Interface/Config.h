// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Config.generated.h"

UENUM()
namespace EConfigMeta
{
	enum Name
	{
		HostName,
		UserName ,
		Password,
		Schema,
		ConnectPort,
	};
}

UENUM()
enum class ESetup :uint8
{
	Outline,
	MediaPlayer,
	MediaList,
	BGM,
	AudioPlayer,
};


USTRUCT()
struct FDS_Config
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	TMap<int32, FString> ConnectProperties;

	UPROPERTY()
	TMap<ESetup, FString> Setting;

	FDS_Config() {}
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConfig : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API IConfig
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IConfig();

	TMap<int32, FString> GetConnectProperties();

	void LoadSetting();

	void SetMeshHoveredOutline(UWorld* World);

	FDS_Config Config;

};
