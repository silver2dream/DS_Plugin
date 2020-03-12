// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AVPlayer.generated.h"

struct FDS_Config;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAVPlayer : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API IAVPlayer
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AudioAndVideoPlayerInit(UWorld* World, FDS_Config& Config);

	virtual void Play();

	virtual void Pause();

	virtual void Stop();

	virtual void PlaySoundPure();

	virtual void PauseSoundPure();

	virtual void StopSoundPure();

	virtual void Mute();

	virtual void Voice();

	virtual void SwitchMusic(FName musicName);

	virtual void Switch(int32 MediaIndex);

protected:
	class UVideoPlayer* VideoPlayer;

	class UAudioPlayer* AudioPlayer;
};
