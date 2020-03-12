// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetBase.h"
#include "QueryPwdWidget.generated.h"

class UCButton;
class UCImage;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UQueryPwdWidget : public UWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWiget))
	UEditableTextBox* PhoneNum;

	UPROPERTY(meta = (BindWiget))
	UCButton* Commit;

	UPROPERTY(meta = (BindWiget))
	UCImage* QueryBG;
	
	virtual void BindTask(UWidgetCtrl* widgetCtrler) override;
};
