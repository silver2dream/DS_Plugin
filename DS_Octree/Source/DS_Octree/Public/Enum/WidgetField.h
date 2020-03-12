// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
namespace EWidget
{
	enum Type
	{
		None,
		CButton,
		CImage,
		CSlider,
		CWidgetSwitcher,
		CUVerticalBox,
	};
}

namespace EField
{
	enum Field : uint8
	{
		Id,
		Type,
		ObjectName,
		Parent,
		Position,
		Size,
		Normal_Name,
		Normal_Path,
		Hovered_Name,
		Hovered_Path,
		Pressed_Name,
		Pressed_Path,
		Event_Name,
		Widget_Name,
		SwitcherWidgetName,
		MediaIndex,
		Visible,
		Listener,
		Composite,
		CompositeEvent,
		StartAnimate,
		EndAnimate,
	};
}