// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "Config.h"

#include "DatabaseProxy.h"

template<typename T>
void GetConfigMeta(FString ParaName, T& Output) {}

template<>
void GetConfigMeta<int32>(FString ParaName, int32& Output)
{
	GConfig->GetInt(
		TEXT("/Script/Zpak.SQLInfo"),
		*ParaName,
		Output,
		GGameIni
	);
}

template<>
void GetConfigMeta<FString>(FString ParaName, FString& Output)
{
	GConfig->GetString(
		TEXT("/Script/Zpak.SQLInfo"),
		*ParaName,
		Output,
		GGameIni
	);
}

// This function does not need to be modified.
UConfig::UConfig(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any IConfig functions that are not pure virtual.
IConfig::IConfig()
{
}

TMap<int32, FString> IConfig::GetConnectProperties()
{
	int32 SQLType;
	GetConfigMeta(TEXT("SQLType"), SQLType);
	UDatabaseProxy::SetSQLType(SQLType);

	if (SQLType == EDatabase::SQLite)
	{
		FString DatabasePath;
		GetConfigMeta(TEXT("SQLitePath"), DatabasePath);
		Config.ConnectProperties.Add(EConnectProperty::DatabasePath, DatabasePath);
	}
	else
	{
		for (int32 Name = EConfigMeta::HostName; Name <= EConfigMeta::ConnectPort; ++Name)
		{
			FString MetaName = UDatabaseProxy::GetEnumShortName("EConfigMeta", Name);
			FString OutValue;
			GetConfigMeta(MetaName, OutValue);
			Config.ConnectProperties.Add(Name, OutValue);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *OutValue);
		}
	}

	return Config.ConnectProperties;
}

void IConfig::LoadSetting()
{
	FStatement Statement;
	/*Statement.Select.Add("EnumKey");
	Statement.Select.Add("Content");*/
	Statement.Select.Add("*");
	Statement.TableName = "Configuration";
	auto Result = UDatabaseProxy::Select(Statement);
	for (uint8 index = 0; index < Result.ResultRows.Num(); ++index)
	{
		auto Key = static_cast<ESetup>(FCString::Atoi(*Result.ResultRows[index].Fields[1].Value));
		Config.Setting.Add(Key) = Result.ResultRows[index].Fields[2].Value;
	}
}

void IConfig::SetMeshHoveredOutline(UWorld* World)
{
	TArray<AActor*> PostProcess;
	UGameplayStatics::GetAllActorsOfClass(World, APostProcessVolume::StaticClass(), PostProcess);
	FStringAssetReference Ref = Config.Setting[ESetup::Outline];
	Cast<APostProcessVolume>(PostProcess[0])->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, Ref.TryLoad()));
}
