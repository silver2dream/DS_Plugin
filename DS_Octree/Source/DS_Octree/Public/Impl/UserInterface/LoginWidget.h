// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetBase.h"
#include "LoginWidget.generated.h"

class UCButton;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API ULoginWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:

	virtual void BindTask(UWidgetCtrl* widgetController) override;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* UsernameBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordBox;

	UPROPERTY(meta = (BindWidget))
	UCButton* Login;

	UPROPERTY(meta = (BindWidget))
	UCButton* Register;

	UPROPERTY(meta = (BindWidget))
	UCButton* ForgetPwd;

private:
	void SelectUserinfo(UWidgetCtrl* widgetCtrl);
};
