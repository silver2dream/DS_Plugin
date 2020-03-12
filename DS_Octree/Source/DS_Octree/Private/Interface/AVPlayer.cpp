// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Config.h"
#include "VideoPlayer.h"
#include "AudioPlayer.h"
#include "DSGameInstance.h"
#include "AVPlayer.h"


// This function does not need to be modified.
UAVPlayer::UAVPlayer(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IAVPlayer functions that are not pure virtual.

void IAVPlayer::AudioAndVideoPlayerInit(UWorld* World,FDS_Config& Config)
{
	auto GI = Cast<UDSGameInstance>( UGameplayStatics::GetGameInstance(World));

	GI->VideoPlayer = NewObject<UVideoPlayer>(World, UVideoPlayer::StaticClass());
	GI->VideoPlayer->Init(Config.Setting[ESetup::MediaPlayer], Config.Setting[ESetup::MediaList]);
	this->VideoPlayer = GI->VideoPlayer;
	
	GI->AudioPlayer = NewObject<UAudioPlayer>(World, UAudioPlayer::StaticClass());
	GI->AudioPlayer->Init(Config.Setting[ESetup::AudioPlayer], Config.Setting[ESetup::BGM]);
	this->AudioPlayer = GI->AudioPlayer;
}

void IAVPlayer::Play()
{
	VideoPlayer->Play();
}

void IAVPlayer::Pause()
{
	VideoPlayer->Pause();
}

void IAVPlayer::Stop()
{
	VideoPlayer->Stop();
}

void IAVPlayer::PlaySoundPure()
{
	AudioPlayer->Play();
}

void IAVPlayer::PauseSoundPure()
{
	AudioPlayer->Pause();
}

void IAVPlayer::StopSoundPure()
{
	AudioPlayer->Stop();
}

void IAVPlayer::Mute()
{
	VideoPlayer->SetMute();
}

void IAVPlayer::Voice()
{
	VideoPlayer->SetVolume(VideoPlayer->GetVolume());
}

void IAVPlayer::SwitchMusic(FName musicName)
{
	AudioPlayer->SwitchMusic(musicName);
}

void IAVPlayer::Switch(int32 MediaIndex)
{
	VideoPlayer->PlayByIndex(MediaIndex);
}
