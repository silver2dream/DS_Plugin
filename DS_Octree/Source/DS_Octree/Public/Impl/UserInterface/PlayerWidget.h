// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abstract/WidgetBase.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UPlayerWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

protected:

	UFUNCTION()
	void OnTimeCapturBegin();

	UFUNCTION()
	void OnTimeCapturEnd();

	UFUNCTION()
	void OnTimeValueChanged(float Value);

	UFUNCTION()
	void OnVolumeCapturBegin();

	UFUNCTION()
	void OnVolumeCapturEnd();

	UFUNCTION()
	void OnVolumeValueChanged(float Value);

private:
	UPROPERTY( meta = (BindWidget))
	UCImage* PlayerScreen;

	UPROPERTY( meta = (BindWidget))
	UCButton* Play;

	UPROPERTY( meta = (BindWidget))
	UCButton* Pause;							
													
	UPROPERTY( meta = (BindWidget))
	UCButton* Stop;								
													
	UPROPERTY( meta = (BindWidget))
	class UCSlider* Time;						
													
	UPROPERTY( meta = (BindWidget))
	class UCSlider* Volume;						
													
	UPROPERTY( meta = (BindWidget))
	UCButton* Voice;							
													
	UPROPERTY( meta = (BindWidget))
	UCButton* Mute;								
													
	UPROPERTY( meta = (BindWidget))
	class UCTextBlock* TimeValue;				
													
	UPROPERTY( meta = (BindWidget))
	class UCTextBlock* Duration;				
													
	UPROPERTY( meta = (BindWidget))
	class UCTextBlock* VolumeValue;			
													
	UPROPERTY( meta = (BindWidget))
	class UCWidgetSwitcher* PlayCtrl;		
													
	UPROPERTY( meta = (BindWidget))
	class UCWidgetSwitcher* MuteCtrl;			

	UPROPERTY()
	class UVideoPlayer* Player;

	const float DefalutVolume = 0.5f;
};
