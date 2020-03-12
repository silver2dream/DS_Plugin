
#pragma once
#include "SQLiteBlueprintNodes.generated.h"


USTRUCT(BlueprintType)
struct DS_SQL_API FSQLiteQueryLogicExpectedNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryLogicExpectedNode(){}
	FSQLiteQueryLogicExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery) 
	{
		Query += Append;
	}
};

USTRUCT(BlueprintType)
struct DS_SQL_API FSQLiteQueryTermExpectedNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryTermExpectedNode(){}
	FSQLiteQueryTermExpectedNode(FString LHSQuery, FString Append) : Query(LHSQuery) 
	{
		Query += Append;
	}
};

USTRUCT(BlueprintType)
struct DS_SQL_API FSQLiteQueryFinalizedQuery
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SQLite Database Query")
	FString Query;

	FSQLiteQueryFinalizedQuery(){}
	FSQLiteQueryFinalizedQuery(FString Query) : Query(Query){}
};
