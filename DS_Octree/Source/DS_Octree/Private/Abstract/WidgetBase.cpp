// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "WidgetBase.h"

#include "UMG.h"

#include "MovieScene.h"
#include "WidgetAnimation.h"
#include "AnimateSystemProxy.h"
#include "DatabaseProxy.h"

#include "Binder.h"
#include "Listener.h"
#include "WidgetCtrl.h"

#include "WidgetFactory.h"

bool UWidgetBase::Initialize()
{
	auto Successful = Super::Initialize();
	if (!Successful) return false;

	AnimateStart = "Start";
	AnimateEnd = "End";
	StartDefalutTime = 0.f;
	EndDefalutTime = 1.f;

	FillAnimationsMap();

	return true;
}

UWorld* UWidgetBase::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void UWidgetBase::ProcessComponents(UWidgetCtrl* widgetCtrler)
{
	auto PropertyMap = GetWidgetProperty(Cast<UPanelWidget>(GetRootWidget()));

	auto AllComponentMeta = UDatabaseProxy::GetActualWidgetData(GetCppName());
	for (auto Meta : AllComponentMeta.ResultRows)
	{
		auto WidgetPtr = PropertyMap.Find(GetWidgetName(EField::ObjectName, Meta));
		auto Parent = PropertyMap.Find(GetWidgetName(EField::Parent, Meta));
		UWidget* Widget = nullptr;
		if (WidgetPtr == nullptr)
		{
			Widget = UWidgetFactory::GetNew(this, Meta);
			PropertyMap.Add(Widget->GetName(), Widget);
			if (Parent)
			{
				Cast<UPanelWidget>(*Parent)->AddChild(Widget);
			}
			else
			{
				AddChild(Widget);
			}
		}
		else
		{
			Widget = *WidgetPtr;
		}
		
		BindMeta(Widget, Meta);

		widgetCtrler->ListenerAttach(Widget);
	}
}

UWidgetAnimation* UWidgetBase::GetAnimationByName(FName AnimationName) const
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
	if (WidgetAnim)
	{
		return *WidgetAnim;
	}
	return nullptr;

}

bool UWidgetBase::PlayAnimationByName(FName AnimationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed, bool bPop = false)
{
	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);

	if (WidgetAnim && bPop)
	{
		if (!WidgetAnim->OnAnimationFinished.IsBound())
		{
			WidgetAnim->OnAnimationFinished.AddDynamic(this, &UWidgetBase::AnimationEndCallback);
		}
	}

	if (WidgetAnim && AnimationName == AnimateStart)
	{
		if (!WidgetAnim->OnAnimationFinished.IsBound())
		{
			WidgetAnim->OnAnimationFinished.AddDynamic(this, &UWidgetBase::AnimationStartCallback);
		}
	}

	if (WidgetAnim)
	{
		PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
		return true;
	}
	return false;

}

bool UWidgetBase::PauseAnimationByName(FName AnimationName)
{
	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);
	if (WidgetAnim)
	{	
		PauseAnimation(WidgetAnim);
		return true;
	}
	return false;
}

void UWidgetBase::SetNotify(FNotify NotifyRef)
{
	Notify = NotifyRef;
}

void UWidgetBase::FillAnimationsMap()
{
	AnimationsMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// Only want the properties that are widget animations
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					WidgetAnim->GetMovieScene()->SetPlaybackRange(StartDefalutTime, EndDefalutTime);
					FName AnimName = WidgetAnim->GetMovieScene()->GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}

}

void UWidgetBase::AnimationStartCallback()
{
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, this->GetName());
}

void UWidgetBase::AnimationEndCallback()
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Purple, this->GetName());
	this->RemoveFromViewport();
	if (Notify.IsSet())
	{
		auto CtrlClass = Notify.GetValue().CtrlClass;
		auto Method = Notify.GetValue().NotifyRef.GetValue();
		auto WidgetRef = Notify.GetValue().NotifyWidget;
		(*CtrlClass.*Method)(WidgetRef);
		Notify.Reset();
	}
}

FName UWidgetBase::GetCppName()
{
	return TableName;
}

void UWidgetBase::SetCppName(FName Name)
{
	TableName = Name;
}

TMap<FString, UWidget*> UWidgetBase::GetWidgetProperty(UPanelWidget* Root, TMap<FString, UWidget*>&& propertyMap)
{
	for (auto propertySlot: Root->GetSlots())
	{
		auto widget = propertySlot->Content;
		propertyMap.Add(widget->GetName(), widget);
		if (IsPanel(widget))
		{
			GetWidgetProperty(Cast<UPanelWidget>(widget), std::move(propertyMap));
		}
	}

	return propertyMap;
}

void UWidgetBase::AddChild(UWidget* widget)
{
	auto Root = Cast<UPanelWidget>(this->GetRootWidget());
	Root->AddChild(widget);
}

void UWidgetBase::BindMeta(UWidget* widget, FSQLQueryResultRow meta)
{
	if (widget != nullptr)
	{
		auto UniversalWidget = Cast<IBinder>(widget);
		UniversalWidget->BindWith(meta.Fields);
		
		AttachExtraAnimation(widget, meta);
	}
}

bool UWidgetBase::IsPanel(UWidget* widget)
{
	if (widget->IsA(UPanelWidget::StaticClass()) && !widget->IsA(UContentWidget::StaticClass()))
	{
		return true;
	}
	return false;
}

FString UWidgetBase::GetWidgetName(TEnumAsByte<EField::Field> FieldName, FSQLQueryResultRow meta)
{
	return meta.Fields[FieldName].Value;
}

void UWidgetBase::AttachExtraAnimation(UWidget* BindingObject, FSQLQueryResultRow meta)
{
	if (!BindingObject) return;

	auto StartAnimate = UDatabaseProxy::GetAnimateDataByIdGroup(meta.Fields[EField::StartAnimate].Value);
	auto EndAnimate = UDatabaseProxy::GetAnimateDataByIdGroup(meta.Fields[EField::EndAnimate].Value);

	TMap<FName, TArray<FAnimateParams> > AnimateDispatchMap;
	FillAnimateDispatchMap(AnimateDispatchMap, AnimateStart, StartAnimate);
	FillAnimateDispatchMap(AnimateDispatchMap, AnimateEnd, EndAnimate);
	
	for (auto Animate :AnimateDispatchMap)
	{
		for (auto ExtraAnimate : Animate.Value)
		{
			auto AnimateName = Animate.Key;
			auto AnimateParam = ExtraAnimate;
			if (AnimateParam.IsTrackNone()) continue;

			auto MajorAnimation = GetAnimationByName(AnimateName);
			if (!MajorAnimation) continue;

			auto MovieScene = MajorAnimation->GetMovieScene();

			FGuid Guid;
			Guid = MovieScene->AddPossessable(BindingObject->GetName(), BindingObject->GetClass());
			MajorAnimation->BindPossessableObject(Guid, *BindingObject, this);

			if (AnimateParam.PropertyPackge.Find("Offsets"))
			{
				auto objectSlot = BindingObject->Slot;
				Guid = MovieScene->AddPossessable(objectSlot->GetName(), objectSlot->GetClass());
				MajorAnimation->BindPossessableObject(Guid, *objectSlot, this);
			}

			auto Proxy = NewObject<UAnimateSystemProxy>(UAnimateSystemProxy::StaticClass());
			Proxy->GenerateAnimate(MovieScene, Guid, AnimateParam);

			UE_LOG(LogTemp, Warning, TEXT("Start: %.3f EndTime: %.3f"), MajorAnimation->GetStartTime(), MajorAnimation->GetEndTime());
		}
	}

}

void UWidgetBase::FillAnimateDispatchMap(TMap<FName, TArray<FAnimateParams> >& DispatchMap, FName AnimationName, FSQLQueryResult Result)
{
	TArray<FAnimateParams> AnimateParamGroup;
	for (auto RowMeta : Result.ResultRows)
	{
		AnimateParamGroup.Add(FAnimateParams(RowMeta));
	}
	DispatchMap.Add(AnimationName, AnimateParamGroup);
}
