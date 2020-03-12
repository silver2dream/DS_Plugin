// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CButton.h"
#include "DialogWidget.h"

#include "TextBlock.h"

constexpr char* MESSAGE_FILE_PATH = u8"DS_Octree/Resources/Message.txt";

bool UDialogWidget::Initialize()
{
	const auto Successful = Super::Initialize();
	if (!Successful) return Successful;

	LoadMessageContentIntoMessageMap();

	return Successful;
}

void UDialogWidget::BindTask(UWidgetCtrl* widgetCtrler)
{
	Confirm->OnLamdaEvent.AddLambda([=] {
		CleanMessage();
		widgetCtrler->PopWidget();
	});
	Confirm->OnClicked.AddDynamic(Confirm, &UCButton::CallLamdaEvent);
}

void UDialogWidget::SetMessage(int32 Id)
{
	const auto Message = MessageMap.Find(Id);
	if (Message)
	{
		MessageContent.Append(*Message);
		MessageText->SetText(FText::FromString(MessageContent));
	}
}

void UDialogWidget::SetCustomMessage(FString CMessage)
{
	MessageText->SetText(FText::FromString(CMessage));
}

void UDialogWidget::CleanMessage()
{
	MessageContent.Empty(0);
	MessageText->SetText(FText());
}

bool UDialogWidget::IsMessageEmpty()
{
	if (MessageContent.IsEmpty())
	{
		return true;
	}
	return false;
}

void UDialogWidget::AnimationEndCallback()
{
	this->RemoveFromViewport();
}

void UDialogWidget::LoadMessageContentIntoMessageMap()
{
	FString FullPath = FPaths::ProjectPluginsDir().Append(MESSAGE_FILE_PATH);
	TArray<FString> Content;
	FFileHelper::LoadFileToStringArray(Content, *FullPath);
	for (int32 Id = EMessage::None; Id < Content.Num(); ++Id)
	{
		MessageMap.Add(Id, Content[Id]);
	}
}
