// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetBase.h"
#include "RigestWidget.generated.h"

UENUM()
namespace ERegistParam
{
	enum Fields
	{
		Account,
		Password,
		Confirm,
		Name,
		Phone,
		Address,
		FinalField,
	};
}

class UEditableTextBox;
class UCButton;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API URigestWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	
	virtual void BindTask(UWidgetCtrl* widgetCtrl) override;

	UPROPERTY( meta = (BindWidget) )
	UEditableTextBox* registerBox_0;

	UPROPERTY( meta = (BindWidget))
	UEditableTextBox* registerBox_1;

	UPROPERTY( meta = (BindWidget))
	UEditableTextBox* registerBox_2;

	UPROPERTY( meta = (BindWidget))
	UEditableTextBox* registerBox_3;

	UPROPERTY( meta = (BindWidget))
	UEditableTextBox* registerBox_4;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* registerBox_5;

	UPROPERTY( meta = (BindWidget))
	UCButton* ConfirmBtn;

	UPROPERTY( meta = (BindWidget))
	UCButton* CancelBtn;

	UPROPERTY( meta = (BindWidget))
	UCButton* ResetBtn;

private:
	void RegistCommit(UWidgetCtrl* widgetCtrl);

	UFUNCTION()
	void ResetFields();

};
