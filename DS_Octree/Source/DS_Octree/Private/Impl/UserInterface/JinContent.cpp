// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "JinContent.h"

#include "CButton.h"
#include "CImage.h"
#include "WidgetTree.h"
#include "PanelWidget.h"
#include "CanvasPanelSlot.h"

#include "MovieSceneMarginTrack.h"
#include "MovieSceneMarginSection.h"
#include "MovieSceneEventTrack.h"
#include "MovieSceneEventSection.h"

#include "EventContainer.h"

void UJinContent::BindTask(UWidgetCtrl* widgetCtrler)
{
	ProcessComponents(widgetCtrler);

	CreateMaskAndAnimation();
	AddExtraEvent();
}

bool UJinContent::Initialize()
{
	auto Successful = Super::Initialize();
	if (!Successful) return false;

	AnimateSub = FName("Sub");
	SelectedIndex = 0;

	auto AnimationStart = GetAnimationByName(AnimateStart);
	AnimationStart->OnAnimationStarted.AddDynamic(this, &UJinContent::InitAllWidgetVisiable);

	DispatchImmediateEvent(AnimateStart, FName("ImmediateEvent"), 1.5f, 1.f);
	DispatchImmediateEvent(AnimateSub, FName("SubImmediateEvent"), 1.5f, 1.f);
	DispatchImmediateEvent(AnimateEnd, FName("InitAllWidgetVisiable"), 1.5f, 0.5f);

	return true;
}

void UJinContent::CreateMaskAndAnimation()
{
	UPanelWidget* Root = Cast<UPanelWidget>(this->GetRootWidget());

	auto EndAnimation = GetAnimationByName(AnimateEnd);
	EndAnimation->GetMovieScene()->SetPlaybackRange(0.f, 2.f);

	auto ContentSize = Cast<UCanvasPanelSlot>(ContentSwitcher->Slot)->GetSize();

	for (auto index = 0; index < Root->GetSlots().Num(); ++index)
	{
		auto DelayTime = index * 0.1f;

		auto WidgetSlot = Root->GetSlots()[index];
		if (!WidgetSlot->Content->IsA(UCButton::StaticClass())) continue;
		ButtonContainer.Add(Cast<UCButton>(WidgetSlot->Content));

		auto ButtonSlot = Cast<UCanvasPanelSlot>(WidgetSlot);
		auto Size = ButtonSlot->GetSize();
		auto Position = ButtonSlot->GetPosition();

		auto ButtonMask = WidgetTree->ConstructWidget<UCImage>(UCImage::StaticClass(), FName(*FString::Printf(TEXT("ButtonMask%d"), index)));
		Root->AddChild(ButtonMask);
		auto MaskSlot = Cast<UCanvasPanelSlot>(ButtonMask->Slot);
		MaskSlot->SetPosition(Position);
		MaskSlot->SetSize(Size);
		FVector4 AnimateParam(1920.f, Position.X, Size.X, 0.f);
		AddExtraAnimation(AnimateStart, ButtonMask, AnimateParam, DelayTime + 1.f);
		AddExtraAnimation(AnimateSub, ButtonMask, AnimateParam, DelayTime + 1.f);

		auto ContentMask = WidgetTree->ConstructWidget<UCImage>(UCImage::StaticClass(), FName(*FString::Printf(TEXT("ContentMask%d"), index)));
		Root->AddChild(ContentMask);
		auto ContentMaskSlot = Cast<UCanvasPanelSlot>(ContentMask->Slot);
		ContentMaskSlot->SetSize(FVector2D(ContentSize.X + Size.X, Size.Y));
		ContentMaskSlot->SetPosition(FVector2D(Position.X + Size.X, Position.Y));
		AnimateParam.Z = ContentSize.X + Size.X;
		AddExtraAnimation(AnimateStart, ContentMask, AnimateParam, DelayTime);

		AnimateParam.Y = Position.X + Size.X;
		AnimateParam.Z = ContentSize.X;
		AddExtraAnimation(AnimateSub, ContentMask, AnimateParam, DelayTime);

		AnimateParam.X = Position.X;
		AnimateParam.Y = 1920.f;
		AnimateParam.Z = ContentSize.X + Size.X;
		AnimateParam.W = 0.f;
		AddExtraAnimation(AnimateEnd, ContentMask, AnimateParam, DelayTime);

		SetMaskColor(ButtonMask, index);
		ButtonMaskContainer.Add(ButtonMask);

		SetMaskColor(ContentMask, index);
		ContentMaskContainer.Add(ContentMask);
	}
}

void UJinContent::AddExtraAnimation(FName AnimateName, UWidget* Widget, FVector4 AnimateParam, float delayTime)
{
	auto MajorAnimation = GetAnimationByName(AnimateName);
	auto MovieScene = MajorAnimation->GetMovieScene();

	auto Guid = MovieScene->AddPossessable(Widget->GetName(), Widget->GetClass());
	MajorAnimation->BindPossessableObject(Guid, *Widget, this);

	Guid = MovieScene->AddPossessable(Widget->Slot->GetName(), Widget->Slot->GetClass());
	MajorAnimation->BindPossessableObject(Guid, *Widget->Slot, this);

	auto Track = MovieScene->AddTrack<UMovieSceneMarginTrack>(Guid);
	Track->SetPropertyNameAndPath(FName("Offsets"), FString("LayoutData.Offsets"));

	auto Section = Cast<UMovieSceneMarginSection>(Track->CreateNewSection());
	Section->SetEndTime(1.5f + delayTime);
	Track->AddSection(*Section);

	auto EvalOp = [=](float A, float B, float Alpha) {
		return (A + Alpha * (B - A));
	};

	FRichCurve& PositionCurve = Section->GetLeftCurve();
	FRichCurve& SizeCurve = Section->GetRightCurve();
	PositionCurve.AddKey(delayTime, AnimateParam.X);
	PositionCurve.AddKey(0.5f + delayTime, EvalOp(AnimateParam.X, AnimateParam.Y, 1.f / 1.f));
	SizeCurve.AddKey(1.f + delayTime, EvalOp(AnimateParam.Z, AnimateParam.W, 0.f / 0.5f));
	SizeCurve.AddKey(1.5f + delayTime, EvalOp(AnimateParam.Z, AnimateParam.W, 0.5f / 0.5f));
}

void UJinContent::AddExtraEvent()
{
	auto PlaySub = [=]() { PlayAnimationByName(AnimateSub, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f); };

	for (auto index = 0; index < ButtonContainer.Num(); ++index)
	{
		auto SetIndex = [=]() { 
			SelectedIndex = index; 
			for (auto maskIndex = 0; maskIndex < ButtonContainer.Num(); ++maskIndex)
			{
				SetMaskColor(ButtonMaskContainer[maskIndex], maskIndex);
				SetMaskColor(ContentMaskContainer[maskIndex], maskIndex);
			}
		};

		ButtonContainer[index]->OnLamdaEvent.AddLambda(SetIndex);
		ButtonContainer[index]->OnLamdaEvent.AddLambda(PlaySub);
	}
}

void UJinContent::InitAllWidgetVisiable()
{
	SetWidgetVisiable(ESlateVisibility::Collapsed);
}

void UJinContent::SetWidgetVisiable(ESlateVisibility Type)
{
	ContentSwitcher->SetVisibility(Type);
	for (auto widget : ButtonContainer)
	{
		widget->SetVisibility(Type);
	}
}

void UJinContent::SetMaskColor(UImage* Mask, int32 Index)
{
	if (SelectedIndex == Index)
	{
		Mask->SetColorAndOpacity(FLinearColor::Blue);
		return;
	}
	Mask->SetColorAndOpacity(FLinearColor::White);
}

void UJinContent::DispatchImmediateEvent(FName AnimationName, FName FuncName, float Duration, float KeyTime)
{
	auto Animation = GetAnimationByName(AnimationName);
	auto MovieScene = Animation->GetMovieScene();
	auto Track = MovieScene->AddMasterTrack(UMovieSceneEventTrack::StaticClass());
	auto Section = Cast<UMovieSceneEventSection>(Track->CreateNewSection());
	Section->SetEndTime(Duration);
	Track->AddSection(*Section);
	
	FEventPayload Event(FuncName);
	auto Curve = Section->GetCurveInterface();
	Curve.AddKeyValue(KeyTime, Event);
}

void UJinContent::ImmediateEvent()
{
	SetWidgetVisiable(ESlateVisibility::Visible);
	UEventContainer::Get("Stop")(ImmediateMeta);
	UEventContainer::Get("SwapWidget")(ImmediateMeta);
	UEventContainer::Get("PopWidget")(ImmediateMeta);
}

void UJinContent::SubImmediateEvent()
{
	ContentSwitcher->SetActiveWidgetIndex(SelectedIndex);
}
