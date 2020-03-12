// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimateEnum.h"

template<typename RetVal>
struct TMemberFunc
{
	typedef TFunction<RetVal(RetVal, RetVal,float)> EvalType;
};

struct FCurveLibrary
{
	template<typename RetVal>
	static RetVal NormalLerp(const RetVal& A, const RetVal& B, const float Alpha)
	{
		return (RetVal)(A + Alpha * (B - A));
	}
};

/**
 * 
 */
struct FCurveProxy
{
	FCurveProxy() {}

	template<typename T>
	static auto DispatchCurve(ECurve::Type CurveType)
	{
		TMemberFunc<T>::EvalType EvalOperator;

		switch (CurveType)
		{
		case ECurve::Lerp:
			EvalOperator = &FCurveLibrary::NormalLerp<T>;
			break;
		case ECurve::InterpSinIn:
			EvalOperator = &FMath::InterpSinIn<T>;
			break;
		case ECurve::InterpSinOut:
			EvalOperator = &FMath::InterpSinOut<T>;
			break;
		case ECurve::InterpSinInOut:
			EvalOperator = &FMath::InterpSinInOut<T>;
			break;
		case ECurve::InterpExpoIn:
			EvalOperator = &FMath::InterpExpoIn<T>;
			break;
		case ECurve::InterpExpoOut:
			EvalOperator = &FMath::InterpExpoOut<T>;
			break;
		case ECurve::InterpExpoInOut:
			EvalOperator = &FMath::InterpExpoInOut<T>;
			break;
		case ECurve::InterpCircularIn:
			EvalOperator = &FMath::InterpCircularIn<T>;
			break;
		case ECurve::InterpCircularOut:
			EvalOperator = &FMath::InterpCircularOut<T>;
			break;
		case ECurve::InterpCircularInOut:
			EvalOperator = &FMath::InterpCircularInOut<T>;
			break;
		default:
			break;
		}

		return EvalOperator;
	}
};