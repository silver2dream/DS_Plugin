// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "CPlayer.h"
#include "VideoPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UVideoPlayer : public UObject, public ICPlayer
{
	GENERATED_BODY()
public:
	virtual void Init(FString playerUri, FString sourceUri) override;

	virtual void Play() override;
	
	virtual void Pause() override;

	virtual void Stop() override;

	virtual void SeekTime(FTimespan time);

	virtual void SetVolume(float VolumeValue);

	virtual void SetMute();

	virtual float GetVolume();

	virtual FTimespan GetTime();

	virtual FTimespan GetDuration();

	virtual bool IsVaild() override;
	
	virtual bool IsPlaying() override;

	virtual void PlayByIndex(int32 MediaIndex);

protected:
	UPROPERTY()
	class UMediaPlayer* Player;

	UPROPERTY()
	class UMediaPlaylist* MediaList;

	UPROPERTY()
	class AActor* SoundPlayer;

	UPROPERTY()
	class UMediaSoundComponent* SoundComponent;

	UPROPERTY()
	float CacheVolume;

	UPROPERTY()
	float CachePlayIndex;
};
