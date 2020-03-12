// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CPlayer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPlayer : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class DS_OCTREE_API ICPlayer
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Init(FString playerUri,FString sourceUri) PURE_VIRTUAL(IVideoPlayer::PlayerInit, void;);

	virtual void Play() PURE_VIRTUAL(IVideoPlayer::Play, void;);

	virtual void Pause() PURE_VIRTUAL(IVideoPlayer::Pause, void;);

	virtual void Stop() PURE_VIRTUAL(IVideoPlayer::Stop, void;);

	virtual bool IsVaild() PURE_VIRTUAL(IVideoPlayer::IsVaild, return false;);
	
	virtual bool IsPlaying() PURE_VIRTUAL(IVideoPlayer::IsVaild, return false;);
};
