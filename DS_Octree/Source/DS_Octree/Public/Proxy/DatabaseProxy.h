// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "DatabaseProxy.generated.h"

struct FStatement;
struct FSQLQueryResult;
struct FSQLKeyValuePair;

UENUM()
namespace EDatabase
{
	enum Type
	{
		None,
		SQLite,
		MySQL,
		MSSQL,
	};
}

UENUM()
namespace EGameWidgetField
{
	enum Field
	{
		Id,
		ModulePath,
		TableName,
		State,
		AnimationStart,
		AnimationEnd,
	};
}

UENUM()
namespace ETable
{
	enum Name
	{
		Animation,
		Bidder,
		Configuration,
		Event,
		GameWidget,
		JinContent,
		JinMain,
		Login,
		MetaData,
		PlayerWidget,
		Production,
		Purchase,
		Resource,
		Screensaver,
		StreetMap,
		Max
	};
}

/**
 * 
 */
UCLASS()
class DS_OCTREE_API UDatabaseProxy : public UObject
{
	GENERATED_BODY()
	
public:
	UDatabaseProxy();

	static void SetSQLType(int32 Type);

	static bool IsConnectDatabase(TMap<int32, FString> ConnectProperties);

	static bool InitDatabase();

	static bool CloneSQL(TMap<int32, FString> ConnectProperties);

	static bool IsTableExist(FString Name);

	static bool CreateTable(FString Name);

	static FSQLQueryResult GetCustomTable();

	static void ParseEnumToTableName(TArray<FString>& Output, FString EnumName);

	static FString GetEnumShortName(FString EnumName, int32 Value);

	static bool IsUserInfoExist(FString TableName, FString Act, FString Pwd);

	static int32 GetFinalId(FString TableName);

	static FSQLQueryResult GetResource();

	static FSQLQueryResult GetEvent();

	static FSQLQueryResult GetMeta();

	static FSQLQueryResult GetAnimation();

	static FSQLQueryResult GetListenerComponent(TArray<FString>& TableNameGroup);

	static FSQLQueryResult GetTableContextByTableName(FString TableName);

	static FSQLQueryResult GetTableContextByTableNameAndId(FString TableName, int32 Id);

	static FSQLQueryResult GetActualWidgetData(FName CppName);

	static FSQLQueryResult GetAnimateDataByIdGroup(FString AnimateIdList);

	static FSQLQueryResult GetPurchaseDataById(int32 BidderId);

	static bool IsPurchaseDataExist(int32 BidderId, FString ItemName);

	static bool GetDataIntoObject(FStatement Statement, UObject* ObjectToPopulate);

	static FSQLQueryResult Select(FStatement Statement);

	static bool Update(FString TableName, FString Id, TMap<FString, FString> ValuesMap);

	static bool Insert(FString TableName, TArray<FString>& Values);

	static bool Delete(FStatement Statement);

	static void Disconnect();

private:

	static void AssignResultsToObjectProperties(const FSQLQueryResultRow& ResultValue, UObject* ObjectToPopulate);

	static TMap<FString, UProperty*> CollectProperties(UObject* SourceObject);

	static TScriptInterface<class ISQL> Database;

	static int32 DatabaseType;
};
