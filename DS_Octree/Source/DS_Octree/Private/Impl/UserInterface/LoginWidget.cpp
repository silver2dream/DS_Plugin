// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "DSGameInstance.h"
#include "StateController.h"

#include "CButton.h"
#include "EventContainer.h"
#include "EditableTextBox.h"

#include "LoginWidget.h"
#include "DialogWidget.h"

#include "Bidder.h"

#include "DatabaseProxy.h"
#include "SQL.h"

void ULoginWidget::BindTask(UWidgetCtrl* widgetController)
{
	Login->OnLamdaEvent.AddLambda([=] {
		SelectUserinfo(widgetController);
	});
	Login->OnClicked.AddDynamic(Login, &UCButton::CallLamdaEvent);

	Register->OnLamdaEvent.AddLambda([=] {
		const auto Widget = widgetController->GetWidgetFromStore(FName("Register"));
		if (Widget)
		{
			widgetController->PushWidget(Widget);
		}
	});
	Register->OnClicked.AddDynamic(Register, &UCButton::CallLamdaEvent);

	ForgetPwd->OnLamdaEvent.AddLambda([=] {
		const auto Widget = widgetController->GetWidgetFromStore(FName("Forget"));
		if (Widget)
		{
			widgetController->PushWidget(Widget);
		}
	});
	ForgetPwd->OnClicked.AddDynamic(ForgetPwd, &UCButton::CallLamdaEvent);

	//widgetController->PushWidget(this);
}

void ULoginWidget::SelectUserinfo(UWidgetCtrl* widgetCtrl)
{
	auto DialogWidget = Cast<UDialogWidget>(widgetCtrl->GetWidgetFromStore(FName("Message")));
	if (!DialogWidget) return;
	
	auto Account = UsernameBox->GetText().ToString();
	auto Password = PasswordBox->GetText().ToString();
	auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetOuter()->GetWorld()));
	GI->Bidder = NewObject<UBidder>(GI, UBidder::StaticClass());

	FStatement Statement;
	Statement.QueryString.Append("SELECT Id,Name,Total FROM Bidder WHERE ");
	Statement.QueryString.Append("Account = '").Append(Account).Append("'");
	Statement.QueryString.Append(" And Password = ").Append(Password);

	if (!UDatabaseProxy::GetDataIntoObject(Statement, GI->Bidder) || GI->GetBidderId() == NULL)
	{
		DialogWidget->SetMessage(EMessage::AccountOrPasswordError);
		widgetCtrl->PushWidget(DialogWidget);
	}
}
