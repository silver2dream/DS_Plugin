// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Studio.generated.h"

class USceneCaptureComponent2D;
class UCineCameraComponent;

UENUM()
namespace EItem
{
	enum Type
	{
		ITEM_ALONE = 4,
	};
}

UCLASS()
class DS_OCTREE_API AStudio : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStudio();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Studio")
	TArray<UStaticMeshComponent*> Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Studio")
	USceneComponent* Root;

	float Velocity = 30.f;

	UFUNCTION()
	void Switch(int32 Orientation);

	UFUNCTION()
	void UpdateStudioItem(FString meshName);

	UFUNCTION()
	UStaticMeshComponent* GetCurrentItem();

	UFUNCTION()
	void TurnCamera(uint8 infoIndex);

	UFUNCTION()
	void SetItemStyle(UStaticMeshComponent* meshComponent, TArray<FSQLKeyValuePair> fields, uint8 itemIndex);

	void AddProductionId(FString Name,int32 Id);

	int32 GetProductId(FString Name);

	void ResetProductionId();

private:
	UCineCameraComponent* CaptureCam;

	USceneCaptureComponent2D* CapTexure;

	TMap<FString, int32> ProductionMap;

	int32 curFrontIndex;

	int32 frontIndexkeeping;

	int32 waitCalcularIndex;

	int32 ModNum;

	TArray<FVector> WallLocation = { 
									{ 0.f, 0.f, 0.f          },
									{ 1990.f, 0.f, 1810.f    },
									{ 180.f, 0.f, 3800.f     },
									{ -1820.f, 0.f, 1800.f   },
									{ 0.f, 2000.f, 1810.f    },
									{ 10.f, -1960.f, 1810.f  }
								   };

	TArray<FRotator> WallRotation = {//y,z,x
									{0.f,0.f,0.f},
									{90.f,0.f,0.f},
									{0.f,0.f,0.f},
									{-90.f,0.f,0.f},
									{0.f,0.f,-90.f},
									{0.f,0.f,90.f}
								   };

	TArray<FVector> itemLoaction = {
									{0.f,0.f,0.f},
									{-490.f,-250.f,0.f},
									{0.f,-510.f,0.f},
									{490.f,-250.f,0.f},
									{ 0.f, 1330.f, 0.f }
								   };

	int32 CalcularIndex(int32 markIndex, int32 orientIndex);

	const float SCALE_X = 245.f;
	const float SCALE_Y = 290.f;
	const float SCALE_Z = 367.f;
	const float PIVOT_Z = 182.f;

};
