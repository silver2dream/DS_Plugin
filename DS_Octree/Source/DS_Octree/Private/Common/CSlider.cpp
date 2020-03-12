// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CSlider.h"

UCSlider::UCSlider()
{}

void UCSlider::BindWith(TArray<FSQLKeyValuePair> Fields)
{
	/*OnLamdaEvent.AddLambda([=] {});
	OnMouseCaptureEnd.AddDynamic(this, &UCSlider::CallLamdaEvent);
	OnValueChanged.AddDynamic(this, &UCSlider::OnValueChange);*/

	SetVisibility(GetVisible(Fields[EField::Visible]));
}

void UCSlider::OnValueChange(float time)
{
	 // CurTime /AllTime
}
