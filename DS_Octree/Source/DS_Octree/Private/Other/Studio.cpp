// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "CineCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DSGameInstance.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Prodution.h"
#include "Studio.h"

#include "DatabaseProxy.h"
#include "SQL.h"

// Sets default values
AStudio::AStudio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.033f;
	curFrontIndex = 0;
	waitCalcularIndex = 0;
	ModNum = 4;

	/*Set Root*/
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	AddInstanceComponent(Root);

	/*set wall*/
	TArray<UStaticMeshComponent*> wallgroup;
	wallgroup.SetNum(6);
	for (uint8 wallindex = 0; wallindex < wallgroup.Num(); wallindex++)
	{
		FName wallname = *FString::Printf(TEXT("wall_%d"), wallindex);;
		wallgroup[wallindex] = CreateDefaultSubobject<UStaticMeshComponent>(wallname);
		FStringAssetReference clsref = "/ds_octree/templatefloor.templatefloor";
		UStaticMesh* mesh = Cast<UStaticMesh>(clsref.TryLoad());
		wallgroup[wallindex]->SetStaticMesh(mesh);
		wallgroup[wallindex]->SetRelativeLocation(WallLocation[wallindex]);
		wallgroup[wallindex]->SetRelativeRotation(WallRotation[wallindex]);

		wallgroup[wallindex]->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
		AddInstanceComponent(wallgroup[wallindex]);
	}

	/*Set PointLight*/
	UPointLightComponent* pointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	pointLight->SetRelativeLocation(FVector(0.f, 200.f, 680.f));
	pointLight->SetRelativeRotation(FRotator::ZeroRotator);
	pointLight->SetAttenuationRadius(1700.f);
	pointLight->SetIntensity(100000.f);
	pointLight->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	AddInstanceComponent(pointLight);

	/*Set Cam and Cam capture*/
	CaptureCam = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CaptureCam"));
	CaptureCam->SetRelativeLocation(FVector(0.f, 492.f, 618.f));
	CaptureCam->SetRelativeRotation(FRotator(-20.f,-90.f,0.f));
	CaptureCam->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	AddInstanceComponent(CaptureCam);
		
	CapTexure = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CapTexure"));
	CapTexure->bCaptureEveryFrame = false;
	CapTexure->bCaptureOnMovement = false;
	CapTexure->AttachToComponent(CaptureCam, FAttachmentTransformRules::KeepRelativeTransform);
	AddInstanceComponent(CapTexure);
	
}

// Called when the game starts or when spawned
void AStudio::BeginPlay()
{
	Super::BeginPlay();

	UpdateStudioItem("NONE");

	/*Set render Target*/
	auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->renderTarget2D = NewObject<UTextureRenderTarget2D>(GetWorld());
	GI->renderTarget2D->InitAutoFormat(1024, 1024);
	GI->renderTarget2D->UpdateResourceImmediate(true);
	GI->renderTarget2D->bAutoGenerateMips = true;
	CapTexure->TextureTarget = GI->renderTarget2D;

}

// Called every frame
void AStudio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CapTexure->CaptureScene();

	if (Item.Num())
	{
		FVector oriVector;
		FRotator orientVector;
		oriVector = UKismetMathLibrary::Conv_RotatorToVector(Item[curFrontIndex]->GetComponentRotation());
		orientVector = UKismetMathLibrary::Conv_VectorToRotator(UKismetMathLibrary::RotateAngleAxis(oriVector, DeltaTime * Velocity, FVector(0.f, 0.f, 1.f)));
		Item[curFrontIndex]->SetWorldRotation(orientVector);
	}

}

void AStudio::Switch(int32 Orientation)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, FString::FromInt(Orientation));
	auto tempRotation = Item[curFrontIndex]->RelativeRotation;
	waitCalcularIndex %= ModNum;
	Orientation == 0 ? (waitCalcularIndex++) : (waitCalcularIndex += (ModNum-1));
	for (uint8 itemIndex = 0; itemIndex < EItem::ITEM_ALONE; ++itemIndex)
	{
		int32 result = CalcularIndex(itemIndex, waitCalcularIndex);
		Item[itemIndex]->SetRelativeLocation(FVector(itemLoaction[result].X, itemLoaction[result].Y, Item[itemIndex]->RelativeLocation.Z));
		Item[itemIndex]->SetWorldRotation(FRotator::ZeroRotator);

		if (result == 0)
		{
			curFrontIndex = itemIndex;
			frontIndexkeeping = curFrontIndex;
			Item[itemIndex]->SetWorldRotation(tempRotation);
		}
		
	}
}

void AStudio::UpdateStudioItem(FString meshName)
{
	if (Item.Num() > 0)
	{
		if (meshName.Equals(Item[curFrontIndex]->GetStaticMesh()->GetName()))
		{
			return;
		}
	}

	FStatement Statement;
	Statement.TableName.Append("Production");
	Statement.Select.Add("*");
	Statement.Condition.Append("MeshName = '").Append(meshName).Append("'");
	auto produtionGroup = UDatabaseProxy::Select(Statement);
	
	/*Update Item*/
	curFrontIndex = 0;
	frontIndexkeeping = 0;
	waitCalcularIndex = 0;
	for (uint8 itemIndex = 0; itemIndex < produtionGroup.ResultRows.Num(); ++itemIndex)
	{
		auto Prodution = produtionGroup.ResultRows[itemIndex].Fields;
		//FName itemName = *FString::Printf(TEXT("Item_%d"), itemIndex);
		FName itemName(*Prodution[EProduction::ItemName].Value);
		if (!Item.IsValidIndex(itemIndex))
		{
			Item.Add(NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), itemName));
			AddProductionId(Prodution[EProduction::ItemName].Value, FCString::Atoi(*Prodution[EProduction::Id].Value));
		}
		SetItemStyle(Item[itemIndex], Prodution, itemIndex);

		/*AttachToComponent*/
		if (!Item[itemIndex]->IsAttachedTo(Root))
		{
			Item[itemIndex]->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			AddInstanceComponent(Item[itemIndex]);
			if (!Item[itemIndex]->IsRegistered())
			{
				Item[itemIndex]->RegisterComponent();
			}

		}
		
	}
}

UStaticMeshComponent* AStudio::GetCurrentItem()
{
	return Item[curFrontIndex];
}

void AStudio::TurnCamera(uint8 infoIndex)
{
	if (CaptureCam)
	{
		curFrontIndex = (infoIndex == 0 ? frontIndexkeeping : EItem::ITEM_ALONE);

		TArray<FVector> Location = {
			{ FVector(0.f, 492.f, 618.f) },
			{ FVector(0.f, 952.f, 335.f) }
		};

		TArray<FRotator> Rotation = {
			{ FRotator(-20.f,-90.f,0.f) },
			{ FRotator(-20.f,90.f,0.f) }
		};

		CaptureCam->SetRelativeLocation(Location[infoIndex]);
		CaptureCam->SetRelativeRotation(Rotation[infoIndex]);
	}
}

void AStudio::SetItemStyle(UStaticMeshComponent* meshComponent, TArray<FSQLKeyValuePair> fields, uint8 itemIndex)
{
	FStringAssetReference clsref = fields[EProduction::Path].Value;
	UStaticMesh* Mesh = Cast<UStaticMesh>(clsref.TryLoad());
	meshComponent->SetStaticMesh(Mesh);
	auto box = meshComponent->GetStaticMesh()->GetBounds().GetBox();
	meshComponent->SetRelativeLocation(FVector(itemLoaction[itemIndex].X, itemLoaction[itemIndex].Y, itemLoaction[itemIndex].Z + (PIVOT_Z - box.GetCenter().Z)));
	if (itemIndex != EItem::ITEM_ALONE)
	{
		meshComponent->SetWorldRotation(FRotator::ZeroRotator);
	}
	meshComponent->SetRelativeScale3D(FVector(SCALE_X / box.GetSize().X, SCALE_Y / box.GetSize().Y, SCALE_Z / box.GetSize().Z));

	TArray<FString> MatArray;
	FString matRef = fields[EProduction::MaterialName].Value;
	matRef.ParseIntoArrayWS(MatArray, TEXT(","), true);
	for (uint8 elementIndex = 0; elementIndex < MatArray.Num(); ++elementIndex)
	{
		FStringAssetReference matARF = "/Game/Art/Material/" + MatArray[elementIndex];
		UMaterialInterface* mat = Cast<UMaterialInterface>(matARF.TryLoad());
		meshComponent->SetMaterial(elementIndex, mat);
	}
}

void AStudio::AddProductionId(FString Name, int32 Id)
{
	ProductionMap.Add(Name, Id);
}

int32 AStudio::GetProductId(FString Name)
{
	if (ProductionMap.Num() > 0)
	{
		const auto Id = ProductionMap.Find(Name);
		return *Id;
	}
	return 0;
}

void AStudio::ResetProductionId()
{
	ProductionMap.Reset();
}

int32 AStudio::CalcularIndex(int32 markIndex, int32 orientIndex)
{
	return (markIndex + orientIndex) % ModNum;
}

