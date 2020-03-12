// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "SQL.generated.h"

namespace EConnectProperty
{
	enum Name
	{
		HostName,
		UserName,
		Password,
		Schema,
		Port,
		DatabasePath,
	};
}

USTRUCT(BlueprintType)
struct FSQLKeyValuePair
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Key;

	UPROPERTY()
	FString Value;
};

USTRUCT(BlueprintType)
struct FSQLQueryResultRow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FSQLKeyValuePair> Fields;
};

USTRUCT(BlueprintType)
struct FSQLQueryResult
{
	GENERATED_USTRUCT_BODY()

	FSQLQueryResult()
	: Success(false)
	{}

	UPROPERTY()
	TArray<FSQLQueryResultRow> ResultRows;

	UPROPERTY()
	bool Success;

	UPROPERTY()
	FString ErrorMessage;
};

USTRUCT(BlueprintType)
struct FSQLImmediateMessage
{
	GENERATED_USTRUCT_BODY()

	FSQLImmediateMessage()
		:Success(false)
	{}

	UPROPERTY()
	bool Success;

	UPROPERTY()
	FString ErrorMessage;
};


USTRUCT()
struct FStatement
{
	GENERATED_USTRUCT_BODY()

public:
	FStatement()
	{}

	FStatement(TArray<FString> select, FString tableName, FString condition = FString())
		: Select(select)
		, TableName(tableName)
		, Condition(condition)
	{}

	FStatement(FString Query)
		: QueryString(Query)
	{}

	UPROPERTY()
	TArray<FString> Select;

	UPROPERTY()
	FString TableName;

	UPROPERTY()
	FString Condition;

	UPROPERTY()
	FString QueryString;

	FString CreateTable()
	{
		FString Create("Create Table ");
	}

	FString GetQuery()
	{
		if (!QueryString.IsEmpty())
		{
			return QueryString;
		}

		FString Query("Select ");
		for (int32 index = 0; index < Select.Num(); ++index)
		{
			Query.Append(Select[index]);
			if (index + 1 != Select.Num())
			{
				Query.Append(",");
			}
		}
		Query.Append(" From ").Append(TableName);
		if (!Condition.IsEmpty())
		{
			Query.Append(" WHERE ").Append(Condition);
		}
		return Query;
	}

	FString GetDelete()
	{
		if (!Condition.IsEmpty()) return FString();

		FString Delete("Delete");
		Delete.Append(" From ").Append(TableName);
		Delete.Append(" WHERE ").Append(Condition);
		return Delete;
	}
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USQL : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS_SQL_API ISQL
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsConnect(TMap<int32, FString> ConnectProperties) PURE_VIRTUAL(ISQL::IsConnect, return false;);

	virtual bool TestingConnect(TMap<int32, FString> ConnectProperties) PURE_VIRTUAL(ISQL::TestngConnect, return false;);

	virtual bool CloneDatabase(FString SchemaName) PURE_VIRTUAL(ISQL::CloneDatabase, return false;);

	virtual bool IsTableExist(FString Name) PURE_VIRTUAL(ISQL::IsTableExist, return false;);

	virtual bool CreateTable(FString Name) PURE_VIRTUAL(ISQL::CreateTable, return false;);

	virtual FSQLQueryResult Select(FStatement Statement) PURE_VIRTUAL(ISQL::Select, return FSQLQueryResult(););

	virtual FSQLImmediateMessage Insert(FString TableName, TArray<FString>& Values) PURE_VIRTUAL(ISQL::Insert, return FSQLImmediateMessage(););

	virtual FSQLImmediateMessage Update(FString TableName, FString Id, TMap<FString, FString> ValuesMap) PURE_VIRTUAL(ISQL::Update, return FSQLImmediateMessage(););

	virtual FSQLImmediateMessage Delete(FStatement Statement) PURE_VIRTUAL(ISQL::Delete, return FSQLImmediateMessage(););

	virtual void Shutdown() PURE_VIRTUAL(ISQL::Update, void;);
};
