// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Listener.h"
#include "WidgetBase.h"
#include "WidgetCtrl.h"

#include "DatabaseProxy.h"

namespace EGameWidget
{
	enum Field
	{
		ModulePath,
		TableName,
	};
}

UWidgetCtrl::UWidgetCtrl()
{}

UWorld* UWidgetCtrl::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void UWidgetCtrl::LoadGameWidget(int32 state)
{
	FStatement Statement;
	Statement.Select.Add("ModulePath");
	Statement.Select.Add("TableName");
	Statement.TableName = "GameWidget";
	Statement.Condition.Append("State = ");
	Statement.Condition.Append(FString::FromInt(state));

	auto Result = UDatabaseProxy::Select(Statement);
	FStringClassReference classRef;
	FName Name;

	for (auto ResultRow : Result.ResultRows)
	{
		classRef = ResultRow.Fields[EGameWidget::ModulePath].Value;
		Name = FName(*ResultRow.Fields[EGameWidget::TableName].Value);
		auto widget = CreateWidget<UWidgetBase>(GetWorld(), classRef.TryLoadClass<UWidgetBase>());
		//Is widget exist? yes:do nothing : no: create widget from c++ class
		if (widget)
		{
			widget->SetCppName(Name);
		}
		else
		{
			widget = CreateWidget<UWidgetBase>(GetWorld(), UWidgetBase::StaticClass());
			widget->SetCppName(Name);
			UE_LOG(LogTemp, Warning, TEXT("Widget Is Not Use BP Module"));
		}
		
		Store.Add(Name, widget);
	}
}

void UWidgetCtrl::BindWidgetFeature()
{
	for (auto widget : Store)
	{
		widget.Value->BindTask(this);
	}
}

void UWidgetCtrl::PushWidget(UWidgetBase* widget)
{
	if (IsPreviousAnimatePlaying())
	{
		PreviousWidget->SetNotify(FNotify(this, &UWidgetCtrl::PushWidget, widget));
		return;
	}

	if (widget && widget->IsInViewport())
	{
		widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		widget->AddToViewport();
	}

	if (!WidgetStack.Contains(widget))
	{
		WidgetStack.Add(widget);
		widget->PlayAnimationByName(widget->AnimateStart, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
}

void UWidgetCtrl::PopWidget()
{
	if (WidgetStack.Num() > 0)
	{
		PreviousWidget = WidgetStack.Pop(true);
		PreviousWidget->PauseAnimationByName(PreviousWidget->AnimateStart);
		auto HasPlayed = PreviousWidget->PlayAnimationByName(PreviousWidget->AnimateEnd, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, true);
		if (!HasPlayed)
		{
			PreviousWidget->RemoveFromViewport();
		}
	}
}

void UWidgetCtrl::PopAllWidget()
{
	PopWidget();
	for (auto Times = WidgetStack.Num(); Times > 1; --Times)
	{
		WidgetStack.Pop(true)->RemoveFromViewport();
	}
}

void UWidgetCtrl::RemoveAllWidget()
{
	for (auto widget :Store)
	{
		if (widget.Value->IsInViewport())
		{
			widget.Value->RemoveFromViewport();
		}
		widget.Value = nullptr;
	}
	Store.Empty();
	WidgetStack.Empty();
}

void UWidgetCtrl::SwapWidget()
{
	auto Length = WidgetStack.Num();
	if (Length >= 2)
	{
		int32 last = Length - 1;
		int32 second = last - 1;
		WidgetStack.Swap(last, second);
	}
}

void UWidgetCtrl::ListenerAttach(class UWidget* Widget)
{
	if (!Cast<IListener>(Widget)) return;

	auto Name = Widget->GetName();
	Listener.Add(Name, Widget);

}

bool UWidgetCtrl::IsPreviousAnimatePlaying()
{
	bool isPlaying = false;

	if (!PreviousWidget)  return isPlaying;

	auto PlayerGroup = PreviousWidget->ActiveSequencePlayers;
	for (auto Player : PlayerGroup)
	{
		isPlaying = (EMovieScenePlayerStatus::Playing == Player->GetPlaybackStatus());
	}

	return isPlaying;
}

UWidgetBase* UWidgetCtrl::GetWidgetFromStore(FName WidgetName)
{
	auto Widget = Store.Find(WidgetName);
	return *Widget;
}

UWidgetBase* UWidgetCtrl::GetWidgetFromHead()
{
	UWidgetBase* Widget = nullptr;
	for (auto Head : Store)
	{
		Widget = Head.Value;
		if (Widget != nullptr)
		{
			break;
		}
	}
	return Widget;
}
