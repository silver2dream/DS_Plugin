// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CSlider.h"
#include "CWidgetSwitcher.h"
#include "DSGameInstance.h"
#include "VideoPlayer.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "PlayerWidget.h"

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	auto SliderValue = Player->GetTime().GetTotalSeconds() / Player->GetDuration().GetTotalSeconds();
	Time->SetValue(SliderValue);
	TimeValue->SetText(FText::AsTimespan(Player->GetTime()));
	Duration->SetText(FText::AsTimespan(Player->GetDuration()));
}

void UPlayerWidget::BindTask(UWidgetCtrl* widgetCtrler)
{
	ProcessComponents(widgetCtrler);

	auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(widgetCtrler->GetWorld()));
	Player = GI->VideoPlayer;
	Player->SetVolume(DefalutVolume);

	Time->OnMouseCaptureBegin.AddDynamic(this, &UPlayerWidget::OnTimeCapturBegin);
	Time->OnMouseCaptureEnd.AddDynamic(this, &UPlayerWidget::OnTimeCapturEnd);
	Time->OnValueChanged.AddDynamic(this, &UPlayerWidget::OnTimeValueChanged);

	Volume->SetValue(DefalutVolume);
	Volume->OnMouseCaptureBegin.AddDynamic(this, &UPlayerWidget::OnVolumeCapturBegin);
	Volume->OnMouseCaptureEnd.AddDynamic(this, &UPlayerWidget::OnVolumeCapturEnd);
	Volume->OnValueChanged.AddDynamic(this, &UPlayerWidget::OnVolumeValueChanged);
	
	VolumeValue->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerWidget::OnTimeCapturBegin()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "** Capture Begin **");
	Player->Pause();
}

void UPlayerWidget::OnTimeCapturEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, "** Capture End **");
	Player->Play();
}

void UPlayerWidget::OnTimeValueChanged(float Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple, "** TimeValueChanged **" + FString::SanitizeFloat(Value));
	auto SeekTime = Value * Player->GetDuration().GetTotalSeconds();
	Player->SeekTime(FTimespan::FromSeconds(SeekTime));
}

void UPlayerWidget::OnVolumeCapturBegin()
{
	VolumeValue->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerWidget::OnVolumeCapturEnd()
{
	VolumeValue->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerWidget::OnVolumeValueChanged(float Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Magenta, "** VolumeValueChanged **" + FString::SanitizeFloat(Value));
	VolumeValue->SetText(FText::AsPercent(Value));
	Player->SetVolume(Value);
	MuteCtrl->SetActiveWidget(Voice);
}
