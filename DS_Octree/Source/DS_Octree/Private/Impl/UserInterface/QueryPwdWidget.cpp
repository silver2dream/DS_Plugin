// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "MovieScene.h"
#include "EditableTextBox.h"
#include "CButton.h"
#include "CImage.h"

#include "Bidder.h"
#include "DialogWidget.h"
#include "QueryPwdWidget.h"
#include "StateController.h"

#include "DatabaseProxy.h"
#include "SQL.h"

void UQueryPwdWidget::BindTask(UWidgetCtrl* widgetCtrler)
{
	Commit->OnLamdaEvent.AddLambda([=] {
		auto DialogWidget = Cast<UDialogWidget>(widgetCtrler->GetWidgetFromStore(FName("Message")));
		if (!DialogWidget) return;

		FStatement Statement;
		Statement.TableName.Append("Bidder");
		Statement.Select.Add("Password");
		Statement.Condition.Append("Phone =").Append(PhoneNum->GetText().ToString());
		const auto Password = UDatabaseProxy::Select(Statement);

		if (Password.ResultRows.Num() < 1)
		{
			DialogWidget->SetMessage(EMessage::UserinfoNotExist);
		}
		else
		{
			DialogWidget->SetCustomMessage(Password.ResultRows[0].Fields[0].Value);
		}
		
		PhoneNum->SetText(FText());
		widgetCtrler->PopWidget();
		widgetCtrler->PushWidget(DialogWidget);
	});
	Commit->OnClicked.AddDynamic(Commit, &UCButton::CallLamdaEvent);

	QueryBG->OnLamdaEvent.AddLambda([=] {
		widgetCtrler->PopWidget();
	});
	QueryBG->OnMouseButtonDownEvent.BindUFunction(QueryBG, FName("CallLamdaEvent"));
}
