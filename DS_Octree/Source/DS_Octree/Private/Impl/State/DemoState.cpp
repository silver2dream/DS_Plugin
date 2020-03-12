// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "DSGameInstance.h"
#include "DatabaseProxy.h"

#include "Studio.h"
#include "ShowroomWidget.h"

#include "StateController.h"
#include "DemoState.h"


UDemoState::UDemoState()
{
}

void UDemoState::Begin_Implementation()
{
	/*read prodution from PurchaseList into */
	TMap<FString, UStaticMesh*> PrePutItemMap;
	TMap<FString, TArray<UMaterialInterface*>> MaterialMap;
	auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto BidderPurchaseItem = UDatabaseProxy::GetPurchaseDataById(GI->GetBidderId());
	if (BidderPurchaseItem.ResultRows.Num() > 0)
	{
		for (auto Item : BidderPurchaseItem.ResultRows)
		{
			auto ItemName = Item.Fields[EPurchase::ObjectName].Value;
			FStringAssetReference MeshRef = Item.Fields[EPurchase::Path].Value;
			PrePutItemMap.Add(ItemName, Cast<UStaticMesh>(MeshRef.TryLoad()));
			MaterialMap.Add(ItemName);

			auto MaterialList = Item.Fields[EPurchase::MaterialName].Value;
			TArray<FString> MaterialGroup;
			MaterialList.ParseIntoArrayWS(MaterialGroup, TEXT(","), true);
			for (auto AssetName : MaterialGroup)
			{
				FStringAssetReference MaterialRef = "/Game/Art/Material/" + AssetName;
				MaterialMap[ItemName].Add(Cast<UMaterialInterface>(MaterialRef.TryLoad()));
			}
		}
	}

	/*Bind Function to All Mesh */
	TArray<AActor*> MeshActorGroup;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), MeshActorGroup);
	for (auto MeshActor : MeshActorGroup)
	{
		MeshActor->OnBeginCursorOver.AddDynamic(this, &UDemoState::SetCustomDepthEnable);
		MeshActor->OnEndCursorOver.AddDynamic(this, &UDemoState::SetCustomDepthUnable);
		MeshActor->OnClicked.AddDynamic(this, &UDemoState::ShowItemMenu);

		auto ItemName = MeshActor->GetName();
		if (PrePutItemMap.Contains(ItemName))
		{
			auto staticMeshComponent = Cast<AStaticMeshActor>(MeshActor)->GetStaticMeshComponent();
			staticMeshComponent->SetMobility(EComponentMobility::Movable);
			staticMeshComponent->SetStaticMesh(PrePutItemMap[ItemName]);
			staticMeshComponent->SetMobility(EComponentMobility::Static);

			for (int32 slotIndex = 0; slotIndex < MaterialMap[ItemName].Num(); ++slotIndex)
			{
				staticMeshComponent->SetMaterial(slotIndex, MaterialMap[ItemName][slotIndex]);
			}
		}
	}
	
	Controller->LoadGameWidget(EState::DEMO);
	Controller->BindWidgetFeature();

	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, "Demo State");
	SetRunBegin(true);
}

void UDemoState::Update_Implementation()
{
	//auto Save
}

void UDemoState::SetCustomDepthEnable(AActor* actor)
{
	Cast<AStaticMeshActor>(actor)->GetStaticMeshComponent()->SetRenderCustomDepth(true);
}

void UDemoState::SetCustomDepthUnable(AActor* actor)
{
	Cast<AStaticMeshActor>(actor)->GetStaticMeshComponent()->SetRenderCustomDepth(false);
}

void UDemoState::ShowItemMenu(AActor* actor, FKey key)
{
	auto WidgetBase = Controller->GetWidgetFromStore(FName("Showroom"));
	auto widget = Cast<UShowroomWidget>(WidgetBase);
	if (widget)
	{
		auto widgetActiveIndex = widget->FeatureSwitcher->GetActiveWidgetIndex();

		widget->focusActor = Cast<AStaticMeshActor>(actor);
		widget->StudioIns->UpdateStudioItem(widget->focusActor->GetStaticMeshComponent()->GetStaticMesh()->GetName());
		widget->StudioIns->TurnCamera(widgetActiveIndex);

		Controller->PushWidget(widget);
	}
}

UWorld* UDemoState::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}
