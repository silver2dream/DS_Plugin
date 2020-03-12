// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetField.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

struct FSQLQueryResultRow;
struct FSQLQueryResult;
struct FAnimateParams;
class UWidgetCtrl;
class UWidgetBase;
class UWidget;


USTRUCT()
struct FNotify
{
	GENERATED_USTRUCT_BODY()

private:
	typedef void(UWidgetCtrl::*NotifyFunc)(UWidgetBase*);

public:
	FNotify() {}

	FNotify(UWidgetCtrl* Ctrl, NotifyFunc&& NotifyMethod, UWidgetBase* Widget)
		: CtrlClass(Ctrl)
		, NotifyRef(NotifyMethod)
		, NotifyWidget(Widget)
	{}

	UPROPERTY()
	UWidgetCtrl* CtrlClass;
	
	TOptional<NotifyFunc> NotifyRef;

	UPROPERTY()
	UWidgetBase* NotifyWidget;
};

/**
 * 
 */
UCLASS(abstract)
class DS_OCTREE_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	virtual UWorld* GetWorld() const override;

	virtual void SetCppName(FName Name);

	virtual void BindTask(UWidgetCtrl* widgetCtrler) PURE_VIRTUAL(UWidgetBase::BindTask, void;);

	bool PlayAnimationByName(
							FName AnimationName,
							float StartAtTime,
							int32 NumLoopsToPlay,
							EUMGSequencePlayMode::Type PlayMode,
							float PlaybackSpeed,
							bool bPop
	);

	bool PauseAnimationByName(FName AnimationName);

	void SetNotify(FNotify NotifyRef);

	UPROPERTY()
	FName AnimateStart;

	UPROPERTY()
	FName AnimateEnd;

protected:
	virtual void ProcessComponents(UWidgetCtrl* widgetCtrler);

	void FillAnimationsMap();

	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

	UFUNCTION()
	virtual void AnimationStartCallback();

	UFUNCTION()
	virtual void AnimationEndCallback();

	virtual FName GetCppName();

	TMap<FName, UWidgetAnimation*> AnimationsMap;

private:
	TMap<FString, UWidget*> GetWidgetProperty(class UPanelWidget* Root, TMap<FString, UWidget*>&& propertyMap = TMap<FString, UWidget*>());

	void AddChild(UWidget* widget);

	void BindMeta(UWidget* widget, FSQLQueryResultRow meta);

	bool IsPanel(UWidget* widget);

	FString GetWidgetName(TEnumAsByte<EField::Field> FieldName, FSQLQueryResultRow meta);

	void AttachExtraAnimation(UWidget* BindingObject, FSQLQueryResultRow meta);

	void FillAnimateDispatchMap(TMap<FName, TArray<FAnimateParams> >& DispatchMap, FName AnimationName, FSQLQueryResult Result);
	
	UPROPERTY()
	float StartDefalutTime;

	UPROPERTY()
	float EndDefalutTime;

	UPROPERTY()
	FName TableName;

	TOptional<FNotify> Notify;
};
