// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "MediaTexture.h"
#include "MediaPlayer.h"
#include "MediaPlaylist.h"
#include "VideoPlayer.h"
#include "MediaSoundComponent.h"
#include "Components/SynthComponent.h"
#include "Components/AudioComponent.h"

void UVideoPlayer::Init(FString playerUri, FString sourceUri)
{
	Player = GetAssetByPath<UMediaPlayer>(playerUri);
	Player->SetLooping(true);

	MediaList = GetAssetByPath<UMediaPlaylist>(sourceUri);

	FActorSpawnParameters SoundPlayerParams;
	SoundPlayerParams.Name = FName("VideoSoundPlayer");
	SoundPlayer = GetOuter()->GetWorld()->SpawnActor<AActor>(SoundPlayerParams);

	SoundComponent = NewObject<UMediaSoundComponent>(SoundPlayer, UMediaSoundComponent::StaticClass(), TEXT("MediaSound"));
	SoundComponent->MediaPlayer = Player;
	SoundComponent->AttachToComponent(SoundPlayer->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SoundComponent->RegisterComponent();

	CacheVolume = 0.f;
	CachePlayIndex = 1;
}

void UVideoPlayer::Play()
{
	if (!Player->IsPaused() && !Player->IsPlaying())
	{
		Player->OpenPlaylistIndex(MediaList, CachePlayIndex);
	}
	else
	{
		Player->Play();
	}
}

void UVideoPlayer::Pause()
{
	if (IsPlaying())
	{
		Player->Pause();
	}
}

void UVideoPlayer::Stop()
{
	CachePlayIndex = Player->GetPlaylistIndex();
	Player->Close();
}

void UVideoPlayer::SeekTime(FTimespan time)
{
	Player->Seek(time);
}

void UVideoPlayer::SetVolume(float VolumeValue)
{
	auto clamp = FMath::Clamp(VolumeValue, 0.001f, 1.f);
	SoundComponent->GetAudioComponent()->SetVolumeMultiplier(clamp);
	CacheVolume = clamp;
}

void UVideoPlayer::SetMute()
{
	SoundComponent->GetAudioComponent()->SetVolumeMultiplier(0.001f);
}

float UVideoPlayer::GetVolume()
{
	return CacheVolume;
}

FTimespan UVideoPlayer::GetTime()
{
	auto Time = Player->GetTime();
	return Time;
}

FTimespan UVideoPlayer::GetDuration()
{
	auto Time = Player->GetDuration();
	return Time;
}

bool UVideoPlayer::IsVaild()
{
	return Player && MediaList;
}

bool UVideoPlayer::IsPlaying()
{
	bool IsPlaying = Player->IsPlaying();
	return IsPlaying;
}

void UVideoPlayer::PlayByIndex(int32 MediaIndex)
{
	Player->OpenPlaylistIndex(MediaList, MediaIndex);
}
