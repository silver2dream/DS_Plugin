// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CButton.h"
#include "DialogWidget.h"
#include "RigestWidget.h"
#include "StateController.h"

#include "EditableTextBox.h"
#include "DialogWidget.h"

#include "DatabaseProxy.h"
#include "SQL.h"

constexpr char* USER_TABLE = u8"Bidder";

void URigestWidget::BindTask(UWidgetCtrl* widgetCtrl)
{
	ConfirmBtn->OnLamdaEvent.AddLambda([=] {
		RegistCommit(widgetCtrl);
	});
	ConfirmBtn->OnClicked.AddDynamic(ConfirmBtn, &UCButton::CallLamdaEvent);

	CancelBtn->OnLamdaEvent.AddLambda([=] {
		widgetCtrl->PopWidget();
	});
	CancelBtn->OnClicked.AddDynamic(CancelBtn, &UCButton::CallLamdaEvent);
		
	ResetBtn->OnClicked.AddDynamic(this, &URigestWidget::ResetFields);
}

void URigestWidget::RegistCommit(UWidgetCtrl* widgetCtrl)
{
	auto DialogWidget = Cast<UDialogWidget>(widgetCtrl->GetWidgetFromStore(FName("Message")));
	if (!DialogWidget) return;

	TMap<uint8, FText> RegistParamMap;
	TArray<FString> InsertValue{};

	for (int32 registIndex = ERegistParam::Account; registIndex < ERegistParam::FinalField; ++registIndex)
	{
		auto param = Cast<UEditableTextBox>(GetWidgetFromName(*FString::Printf(TEXT("registerBox_%d"), registIndex)));
		RegistParamMap.Add(registIndex, param->GetText());
		if (param->GetText().IsEmpty())
		{
			DialogWidget->SetMessage(EMessage::FieldsNotFully);
			break;
		}
		if (registIndex != ERegistParam::Confirm)
		{
			InsertValue.Add(param->GetText().ToString());
		}
	}

	if (DialogWidget->IsMessageEmpty())
	{
		if (!RegistParamMap[ERegistParam::Confirm].ToString().Equals(RegistParamMap[ERegistParam::Password].ToString()))
		{
			DialogWidget->SetMessage(EMessage::SecondPasswordDifferent);
		}
		else
		{
			if (UDatabaseProxy::IsUserInfoExist(USER_TABLE, RegistParamMap[ERegistParam::Account].ToString(), RegistParamMap[ERegistParam::Password].ToString()))
			{
				DialogWidget->SetMessage(EMessage::UserinfoExistAlready);
			}
			else
			{
				const auto FinalId = UDatabaseProxy::GetFinalId(USER_TABLE);
				const auto NextId = FinalId + 1;
				InsertValue.Insert(FString::FromInt(NextId), 0);
				InsertValue.Insert(FString::FromInt(0), InsertValue.Num());
				if (UDatabaseProxy::Insert(USER_TABLE, InsertValue))
				{
					DialogWidget->SetMessage(EMessage::RigestSuccessful);
					widgetCtrl->PopWidget();
				}
				else
				{
					DialogWidget->SetMessage(EMessage::RigestFail);
				}
			}
		}
	}

	widgetCtrl->PushWidget(DialogWidget);
}

void URigestWidget::ResetFields()
{
	for (int32 registIndex = ERegistParam::Account; registIndex < ERegistParam::FinalField; ++registIndex)
	{
		auto param = Cast<UEditableTextBox>(GetWidgetFromName(*FString::Printf(TEXT("registerBox_%d"), registIndex)));
		param->SetText(FText::GetEmpty());
	}
}
