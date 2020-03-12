// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomActor.generated.h"


UCLASS()
class DS_OCTREE_API ACustomActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACustomActor();

	FOctreeElementId OctreeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomActor")
		UBoxComponent* BoxComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(BlueprintCallable, Category = "CustomActor")
		void AddToOctree();

	UFUNCTION(BlueprintCallable, Category = "CustomActor")
		void RemoveFormOctree();
};



