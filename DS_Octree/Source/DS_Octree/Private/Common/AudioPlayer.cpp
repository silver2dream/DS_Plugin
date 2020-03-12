// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Sound/SoundWave.h"
#include "AudioPlayer.h"

void UAudioPlayer::Init(FString playerUri, FString sourceUri)
{
	FActorSpawnParameters AudioPlayerParams;
	AudioPlayerParams.Name = FName("AudioPlayer");
	AudioPlayer = GetOuter()->GetWorld()->SpawnActor<AAmbientSound>(AudioPlayerParams);
	Music = GetAssetByPath<USoundWave>(sourceUri);
	AudioPlayer->GetAudioComponent()->SetSound(Music);
}

void UAudioPlayer::Play()
{
	if (IsVaild() && !IsPlaying())
	{
		AudioPlayer->Play();
	}
	else if (IsPause())
	{
		AudioPlayer->GetAudioComponent()->SetPaused(false);
	}
}

void UAudioPlayer::Pause()
{
	if (IsPlaying())
	{
		AudioPlayer->GetAudioComponent()->SetPaused(true);
	}
}

void UAudioPlayer::Stop()
{
	if (IsPlaying())
	{
		AudioPlayer->Stop();
	}
}

bool UAudioPlayer::IsVaild()
{
	return AudioPlayer && Music;
}

void UAudioPlayer::SwitchMusic(FName musicName)
{
	//Music = MusicList[musicName];
	//AudioPlayer->GetAudioComponent()->SetSound(Music);
}

bool UAudioPlayer::IsPlaying()
{
	bool IsPlaying = AudioPlayer->GetAudioComponent()->IsPlaying();
	return IsPlaying;
}

bool UAudioPlayer::IsPause()
{
	bool IsPause = AudioPlayer->GetAudioComponent()->bIsPaused;
	return IsPause;
}

