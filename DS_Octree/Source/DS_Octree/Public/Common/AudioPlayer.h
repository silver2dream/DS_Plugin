// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "CPlayer.h"
#include "AudioPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UAudioPlayer : public UObject, public ICPlayer
{
	GENERATED_BODY()
public:
	virtual void Init(FString playerUri, FString sourceUri) override;

	virtual void Play() override;

	virtual void Pause() override;

	virtual void Stop() override;

	virtual bool IsVaild() override;
	
	virtual void SwitchMusic(FName musicName);

	virtual bool IsPlaying() override;

	virtual bool IsPause();

protected:
	UPROPERTY()
	class AAmbientSound* AudioPlayer;

	UPROPERTY()
	class USoundWave* Music;

	UPROPERTY()
	float CurrentVolume;
		
	//TMap<FName,class USoundWave*> MusicList;
};
