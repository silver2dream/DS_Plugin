// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Prodution.h"

UProdution::UProdution()
{}

FString UProdution::PrintInfom()
{
	return FString::Printf(TEXT("Id:%d Type:%d MeshName:%s MaterialName:%s Price:%d"), Id, Type, *MeshName, *MaterialName, Price);
}
