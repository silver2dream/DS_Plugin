// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "TestMain.h"
#include "CButton.h"
#include "DSGameInstance.h"
#include "WidgetCtrl.h"
#include "VideoPlayer.h"
#include "AudioPlayer.h"

#include "WidgetAnimation.h"
#include "MovieScene.h"
#include "MovieSceneSection.h"
#include "MovieScene2DTransformTrack.h"
#include "MovieScene2DTransformSection.h"
#include "UnrealMathUtility.h"

void UTestMain::BindTask(UWidgetCtrl* widgetCtrler)
{
	ProcessComponents(widgetCtrler);

	UE_LOG(LogTemp, Warning, TEXT("TestMain BindTask"));

	//widgetCtrler->PushWidget(this);
}

bool UTestMain::Initialize()
{
	auto Successful = Super::Initialize();
	if (!Successful) return false;

	auto GI = Cast<UDSGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return false;
	
	GI->VideoPlayer->PlayByIndex(0);
	GI->AudioPlayer->Play();

	UE_LOG(LogTemp, Warning, TEXT("TestMain Init"));

	return true;
}
