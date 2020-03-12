// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQL.h"
#include "UObject/NoExportTypes.h"

#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>

#include "MySqlAPI.generated.h"

/**
 * 
 */
UCLASS()
class DS_SQL_API UMySqlAPI :public UObject, public ISQL
{
	GENERATED_BODY()
	
public:
	virtual bool IsConnect(TMap<int32, FString> ConnectProperties) override;

	virtual bool TestingConnect(TMap<int32, FString> ConnectProperties) override;

	virtual FSQLQueryResult Select(FStatement Statement) override;

	virtual bool CloneDatabase(FString SchemaName) override;

	virtual bool IsTableExist(FString Name) override;

	virtual bool CreateTable(FString Name) override;
	
	virtual FSQLImmediateMessage Insert(FString TableName, TArray<FString>& Values) override;

	virtual FSQLImmediateMessage Update(FString TableName, FString Id, TMap<FString, FString> ValuesMap) override;

	virtual FSQLImmediateMessage Delete(FStatement Statement) override;

	sql::ConnectOptionsMap Connect_Properties{};

	virtual void Shutdown();

private:
	sql::Driver* MySqlDriver {};
	sql::Connection* Conn{};
	sql::Statement* stmt{};

	bool IsDataExist(sql::ResultSet* Data, FSQLQueryResult& Result);

	bool IsDatabaseExist();

	UPROPERTY()
	FString Schema;
};
