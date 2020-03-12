// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "DSGameInstance.h"
#include "CustomActor.h"


// Sets default values
ACustomActor::ACustomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomActor::BeginPlay()
{
	Super::BeginPlay();
	FTransform transform = FTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, 240.f), FVector(1.f, 1.f, 1.f));

	BoxComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass());
	BoxComponent->ShapeColor = FColor::Blue;
	BoxComponent->SetBoxExtent(FVector(100.f, 100.f, 100.f), false);
	BoxComponent->SetWorldTransform(transform);
	RootComponent = BoxComponent;
	BoxComponent->RegisterComponent();

	DrawDebugBox(GetWorld(), BoxComponent->GetCenterOfMass(), BoxComponent->GetUnscaledBoxExtent(), FColor::Blue, true, 0.f, 0, 1.f);

	AddToOctree();
}

void ACustomActor::BeginDestroy()
{
	RemoveFormOctree();
	Super::BeginDestroy();
}

// Called every frame
void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomActor::AddToOctree()
{
	/*auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (OctreeId.IsValidId())
	{
		GI->Octree->RemoveElement(OctreeId);
		OctreeId = FOctreeElementId();
	}

	GI->Octree->AddElement(this);*/
}

void ACustomActor::RemoveFormOctree()
{
	/*if (GetWorld())
	{
		auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (OctreeId.IsValidId())
		{
			GI->Octree->RemoveElement(OctreeId);
		}

		OctreeId = FOctreeElementId();
	}*/
}

