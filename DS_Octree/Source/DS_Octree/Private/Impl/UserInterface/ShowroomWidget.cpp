// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Studio.h"
#include "CButton.h"
#include "StateController.h"
#include "ShowroomWidget.h"

#include "DSGameInstance.h"
#include "WidgetSwitcher.h"
#include "Image.h"
#include "Scrollbox.h"
#include "TextBlock.h"
#include "ScrollBoxSlot.h"
#include "ButtonSlot.h"

#include "DatabaseProxy.h"
#include "SQL.h"
#include "Prodution.h"

constexpr char* MasterMaterialPath = u8"/DS_Octree/Cap_Mat.Cap_Mat";
constexpr char* PurchaseTable = u8"Purchase";
constexpr char* ProductionTable = u8"Production";

void UShowroomWidget::BindTask(UWidgetCtrl* widgetCtrl)
{
	StudioIns = GetWorld()->SpawnActor<AStudio>(FVector(1000.f, 2000.f, -9000.f), FRotator::ZeroRotator);
	
	FStringAssetReference MasterRef(MasterMaterialPath);
	UMaterialInstanceDynamic* MaterialIns = UMaterialInstanceDynamic::Create(Cast<UMaterial>(MasterRef.TryLoad()), GetWorld());
	auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MaterialIns->SetTextureParameterValue(TEXT("Diffuse"), GI->renderTarget2D);

	FSlateBrush brush;
	brush.SetResourceObject(MaterialIns);
	AllView->SetBrush(brush);
	OneView->SetBrush(brush);

	SwitchStyleButton->OnLamdaEvent.AddLambda([=] {
		FeatureSwitcher->SetActiveWidgetIndex(0);
		StudioIns->TurnCamera(0);
	});
	SwitchStyleButton->OnClicked.AddDynamic(SwitchStyleButton, &UCButton::CallLamdaEvent);

	SetupBtn->OnClicked.AddDynamic(this, &UShowroomWidget::Setup);

	SwitchFurniture->OnLamdaEvent.AddLambda([=] {
		FeatureSwitcher->SetActiveWidgetIndex(1);
		StudioIns->TurnCamera(1);
	});
	SwitchFurniture->OnClicked.AddDynamic(SwitchFurniture, &UCButton::CallLamdaEvent);
	SwitchFurniture->OnClicked.AddDynamic(this, &UShowroomWidget::LoadAllItem);

	LeftArrow->OnLamdaEvent.AddLambda([=] {
		StudioIns->Switch(0);
	});
	LeftArrow->OnClicked.AddDynamic(LeftArrow, &UCButton::CallLamdaEvent);

	RightArrow->OnLamdaEvent.AddLambda([=] {
		StudioIns->Switch(1);
	});
	RightArrow->OnClicked.AddDynamic(RightArrow, &UCButton::CallLamdaEvent);

	RecoverBtn->OnClicked.AddDynamic(this, &UShowroomWidget::RecoverItem);

	CloseBtn->OnClicked.AddDynamic(widgetCtrl, &UWidgetCtrl::PopWidget);
		
}

void UShowroomWidget::LoadAllItem()
{
	if (ItemBox->GetChildrenCount() > 0) return;

	UStaticMeshComponent* meshComponent = NewObject<UStaticMeshComponent>(StudioIns, UStaticMeshComponent::StaticClass(), TEXT("ItemAlone"));
	StudioIns->Item.Add(meshComponent);
	meshComponent->AttachToComponent(StudioIns->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	StudioIns->AddInstanceComponent(meshComponent);
	meshComponent->RegisterComponent();

	FButtonStyle btnStyle;
	FLinearColor Color = FLinearColor(0.f, 0.f, 0.f, 0.5f);
	btnStyle.Normal.TintColor = Color;
	btnStyle.Hovered.TintColor = FLinearColor(0.f, 0.f, 1.f, 0.5f);
	btnStyle.Pressed.TintColor = Color;

	FStatement Statement;
	Statement.Select.Add("*");
	Statement.TableName.Append(ProductionTable);
	auto Result = UDatabaseProxy::Select(Statement);

	for (int32 itemIndex = 0; itemIndex < Result.ResultRows.Num(); ++itemIndex)
	{
		auto ProductionInfo = Result.ResultRows[itemIndex].Fields;
		UCButton* Item = NewObject<UCButton>(UCButton::StaticClass());
		Item->SetStyle(btnStyle);
		Item->OnLamdaEvent.AddLambda([=] {
			StudioIns->SetItemStyle(meshComponent, ProductionInfo, EItem::ITEM_ALONE);
			CacheItemName = ProductionInfo[EProduction::ItemName].Value;
		});
		Item->OnHovered.AddDynamic(Item, &UCButton::CallLamdaEvent);
		Item->OnClicked.AddDynamic(this, &UShowroomWidget::Setup);
		StudioIns->AddProductionId(ProductionInfo[EProduction::ItemName].Value, FCString::Atoi(*ProductionInfo[EProduction::Id].Value));
		ItemBox->AddChild(Item);
		UScrollBoxSlot* sbSlot = Cast<UScrollBoxSlot>(Item->Slot);
		sbSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);

		UTextBlock* text = NewObject<UTextBlock>(UTextBlock::StaticClass(), *FString::Printf(TEXT("Item_%d"), itemIndex));
		text->SetText(FText::FromString(ProductionInfo[EProduction::MeshName].Value));
		Item->AddChild(text);
		UButtonSlot* textSlot = Cast<UButtonSlot>(text->Slot);
		textSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
		textSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}
}

void UShowroomWidget::Setup()
{
	if (StoreItemActor == nullptr)
	{
		StoreItemActor = NewObject<AStaticMeshActor>(AStaticMeshActor::StaticClass(), TEXT("StoreItem"));
		SwapItemDetail(StoreItemActor, focusActor);
	}

	auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	const auto BidderId = GI->GetBidderId();
	
	auto item = StudioIns->GetCurrentItem();
	const auto FocusActorName = focusActor->GetName();
	auto FocusStaticMeshComponent = focusActor->GetStaticMeshComponent();
	FocusStaticMeshComponent->SetMobility(EComponentMobility::Movable);
	FocusStaticMeshComponent->SetStaticMesh(item->GetStaticMesh());
	FocusStaticMeshComponent->SetMobility(EComponentMobility::Static);

	auto FocusMaterialGroup = FocusStaticMeshComponent->GetMaterials();
	FString MaterialList;
	for (int32 SlotIndex = 0; SlotIndex < FocusMaterialGroup.Num(); ++SlotIndex)
	{
		auto Material = item->GetMaterial(SlotIndex);
		FocusStaticMeshComponent->SetMaterial(SlotIndex, Material);
		MaterialList.Append(Material->GetName()).Append(",");
	}
	MaterialList.RemoveFromEnd(",");

	const auto Nextid = UDatabaseProxy::GetFinalId(PurchaseTable) + 1;
	const auto ProductionId = (item->GetName().Equals("ItemAlone") ? StudioIns->GetProductId(CacheItemName) : StudioIns->GetProductId(item->GetName()));
	TArray<FString> InsertParam;
	InsertParam.Add(FString::FromInt(Nextid));
	InsertParam.Add(FString::FromInt(ProductionId));
	InsertParam.Add(FString::FromInt(BidderId));
	InsertParam.Add(FocusActorName);
	InsertParam.Add(item->GetStaticMesh()->GetName());
	InsertParam.Add(MaterialList);

	if (UDatabaseProxy::IsPurchaseDataExist(BidderId, FocusActorName))
	{
		TMap<FString, FString> UpdateParam;
		UpdateParam.Add("ObjectName", FocusActorName);
		UpdateParam.Add("MeshName",item->GetStaticMesh()->GetName());
		UpdateParam.Add("MaterialName", MaterialList);

		UDatabaseProxy::Update(PurchaseTable, FString::FromInt(BidderId), UpdateParam);
	}
	else
	{
		UDatabaseProxy::Insert(PurchaseTable, InsertParam);
	}
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, "Set up");
}

void UShowroomWidget::RecoverItem()
{
	if (StoreItemActor)
	{
		auto GI = Cast<UDSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		SwapItemDetail(focusActor, StoreItemActor);
		FStatement Statement;
		Statement.TableName.Append(PurchaseTable);
		Statement.Condition.Append("BidderId = '").Append(FString::FromInt(GI->GetBidderId())).Append("'");
		Statement.Condition.Append(" And ObjectName = '").Append(focusActor->GetName()).Append("'");
		UDatabaseProxy::Delete(Statement);
		StoreItemActor = nullptr;
	}
}

void UShowroomWidget::SwapItemDetail(AStaticMeshActor* focus, AStaticMeshActor* store)
{
	if (!focus || !store) return;

	auto StoreStaticMeshComponent = store->GetStaticMeshComponent();
	auto FocusStaticMeshComponent = focus->GetStaticMeshComponent();
	FocusStaticMeshComponent->SetMobility(EComponentMobility::Movable);
	FocusStaticMeshComponent->SetStaticMesh(StoreStaticMeshComponent->GetStaticMesh());
	FocusStaticMeshComponent->SetMobility(EComponentMobility::Static);

	for (int32 SlotIndex = 0; SlotIndex < FocusStaticMeshComponent->GetMaterials().Num(); ++SlotIndex)
	{
		FocusStaticMeshComponent->SetMaterial(SlotIndex, StoreStaticMeshComponent->GetMaterial(SlotIndex));
	}
}
