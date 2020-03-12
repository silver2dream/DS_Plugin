// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetBase.h"
#include "DialogWidget.generated.h"

UENUM()
namespace EMessage
{
	enum Type
	{
		None,
		Exception,
		ErrorPassword,
		FieldsNotFully,
		SecondPasswordDifferent,
		RigestSuccessful,
		RigestFail,
		AccountOrPasswordError,
		UserinfoNotExist,
		UserinfoExistAlready,
	};
}

class UCButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UDialogWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;

	virtual void SetMessage(int32 Id);

	virtual void SetCustomMessage(FString CMessage);

	virtual void CleanMessage();

	bool IsMessageEmpty();

protected:
	virtual void AnimationEndCallback() override;

private:

	void LoadMessageContentIntoMessageMap();

	UPROPERTY(meta = (BindWidget))
	UCButton* Confirm;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;

	UPROPERTY(meta = (BindWidget))
	FString MessageContent;

	UPROPERTY()
	TMap<int32, FString> MessageMap;
};
