// Fill out your copyright notice in the Description page of Project Settings.

#include "DS_Octree.h"
#include "DatabaseProxy.h"

#include "SQLiteDatabase.h"
#include "MySqlAPI.h"




TScriptInterface<ISQL> UDatabaseProxy::Database;
int32 UDatabaseProxy::DatabaseType;


UDatabaseProxy::UDatabaseProxy()
{}

void UDatabaseProxy::SetSQLType(int32 Type)
{
	DatabaseType = Type;
}

bool UDatabaseProxy::IsConnectDatabase(TMap<int32, FString> ConnectProperties)
{
	if (InitDatabase())
	{
		if (Database->IsConnect(ConnectProperties))
		{
			UE_LOG(LogTemp, Warning, TEXT("Database Loaded."));
			return true;
		}
	}
	return false;
}

bool UDatabaseProxy::InitDatabase()
{
	if (!Database)
	{
		auto SQLiteLambda = []() { return (UObject*) NewObject<USQLiteDatabase>(USQLiteDatabase::StaticClass()); };
		auto MySQLLambda = []() { return (UObject*)NewObject<UMySqlAPI>(UMySqlAPI::StaticClass()); };

		auto TempDb = (DatabaseType == EDatabase::SQLite ? SQLiteLambda() : MySQLLambda());
		TempDb->AddToRoot();
		Database = TempDb;
		if (!Database) return false;
	}
	return true;
}

bool UDatabaseProxy::CloneSQL(TMap<int32, FString> ConnectProperties)
{
	auto IsSucceed = false;
	if (InitDatabase())
	{
		auto ConnProperty = (DatabaseType==EDatabase::SQLite ? ConnectProperties[EConnectProperty::DatabasePath] : ConnectProperties[EConnectProperty::Schema]);

		if (Database->TestingConnect(ConnectProperties))
		{
			IsSucceed = Database->CloneDatabase(ConnProperty);
		}
	}
	return IsSucceed;
}

bool UDatabaseProxy::IsTableExist(FString Name)
{
	const auto IsExist = Database->IsTableExist(Name);
	return IsExist;
}

bool UDatabaseProxy::CreateTable(FString Name)
{
	const auto IsSucceed = Database->CreateTable(Name);
	return IsSucceed;
}

FSQLQueryResult UDatabaseProxy::GetCustomTable()
{
	FSQLQueryResult CustomTables;
	FStatement Statement;
	Statement.Select.Add("TableName");
	Statement.TableName = "Registered";
	Statement.Condition.Append("IsDelete = 0");
	CustomTables = Database->Select(Statement);
	return CustomTables;
}

FSQLQueryResult UDatabaseProxy::GetResource()
{
	FSQLQueryResult Resource;
	FStatement Statement;
	Statement.Select.Add("Name");
	Statement.TableName = "Resource";
	Resource = Database->Select(Statement);
	return Resource;
}

FSQLQueryResult UDatabaseProxy::GetEvent()
{
	FSQLQueryResult Event;
	FStatement Statement;
	Statement.Select.Add("EventName");
	Statement.TableName = "Event";
	Event = Database->Select(Statement);
	return Event;
}

FSQLQueryResult UDatabaseProxy::GetMeta()
{
	FSQLQueryResult Meta;
	FStatement Statement;
	Statement.Select.Add("Name");
	Statement.TableName = "MetaData";
	Meta = Database->Select(Statement);
	return Meta;
}

FSQLQueryResult UDatabaseProxy::GetAnimation()
{
	FSQLQueryResult Animation;
	FStatement Statement;
	Statement.Select.Add("Name");
	Statement.TableName = "Animation";
	Animation = Database->Select(Statement);
	return Animation;
}

FSQLQueryResult UDatabaseProxy::GetListenerComponent(TArray<FString>& TableNameGroup)
{
	FSQLQueryResult Context;
	FString QueryString;
	for (auto TableIndex = 0; TableIndex < TableNameGroup.Num(); ++TableIndex)
	{
		QueryString.Append("SELECT ObjectName FROM ").Append(TableNameGroup[TableIndex]).Append(" WHERE Listener = 1");
		if ( (TableIndex + 1) != TableNameGroup.Num())
			QueryString.Append(" UNION ");
	}
	FStatement Statement;
	Statement.QueryString = QueryString;
	Context = Database->Select(Statement);
	return Context;
}

FSQLQueryResult UDatabaseProxy::GetTableContextByTableName(FString TableName)
{
	FSQLQueryResult Context;
	FStatement Statement;
	Statement.Select.Add("*");
	Statement.TableName = TableName;
	Context = Database->Select(Statement);
	return Context;
}

FSQLQueryResult UDatabaseProxy::GetTableContextByTableNameAndId(FString TableName, int32 Id)
{
	FSQLQueryResult Context;
	FStatement Statement;
	auto RowId = FString::FromInt(Id);
	FString Condition("Id = ");
	Condition.Append(RowId);

	Statement.Select.Add("*");
	Statement.TableName = TableName;
	Statement.Condition = Condition;
	
	Context = Database->Select(Statement);
	return Context;
}

FSQLQueryResult UDatabaseProxy::GetActualWidgetData(FName CppName)
{
	FSQLQueryResult Context;
	FString Query = "SELECT A.Id,A.Type,A.Objectname,A.Parent,A.Position,A.Size,C.Name,C.Path,D.Name,D.Path,E.Name,E.Path,B.EventName,F.WidgetName,F.SwitcherWidgetName,F.MediaIndex,A.Visible,A.Listener,A.Composite,A.CompositeEvent";
	Query.Append(", A.StartAnimate, A.EndAnimate ");
	Query.Append("FROM " + CppName.ToString() + " A ");
	Query.Append("LEFT OUTER JOIN Event B ON A.Event = B.Id ");
	Query.Append("LEFT OUTER JOIN Resource C ON A.Normal = C.Id ");
	Query.Append("LEFT OUTER JOIN Resource D ON A.Hovered = D.Id ");
	Query.Append("LEFT OUTER JOIN Resource E ON A.Pressed = E.Id ");
	Query.Append("LEFT OUTER JOIN MetaData F ON A.Meta = F.Id ");

	FStatement Statement(Query);
	Context = UDatabaseProxy::Select(Statement);
	return Context;
}

FSQLQueryResult UDatabaseProxy::GetAnimateDataByIdGroup(FString AnimateIdList)
{
	FSQLQueryResult AllExtraAnimte;

	if (AnimateIdList.IsEmpty()) return AllExtraAnimte;

	TArray<FString> AnimateIdGroup;
	AnimateIdList.ParseIntoArray(AnimateIdGroup, TEXT(","), true);

	FStatement Statement;
	Statement.Select.Add("*");
	Statement.TableName = "Animation";
	for (auto i = 0; i < AnimateIdGroup.Num(); ++i)
	{
		Statement.Condition.Append("Id = ").Append(AnimateIdGroup[i]);

		if (i == AnimateIdGroup.Num() - 1) continue;

		Statement.Condition.Append(" Or ");
	}

	AllExtraAnimte = UDatabaseProxy::Select(Statement);
	return AllExtraAnimte;
}

FSQLQueryResult UDatabaseProxy::GetPurchaseDataById(int32 BidderId)
{
	FSQLQueryResult preItemGroup;
	FStatement Statement;
	Statement.QueryString.Append("SELECT A.Id,A.Path,B.MaterialName,B.ObjectName FROM Production as A Join Purchase as B On A.Material = B.MaterialName where B.BidderId = ");
	Statement.QueryString.Append(FString::FromInt(BidderId));
	preItemGroup = Database->Select(Statement);
	return preItemGroup;
}

bool UDatabaseProxy::IsPurchaseDataExist(int32 BidderId, FString ItemName)
{
	FSQLQueryResult Result;
	FStatement Statement;
	Statement.Select.Add("*");
	Statement.TableName.Append("Purchase");
	Statement.Condition.Append("BidderId = ").Append(FString::FromInt(BidderId));
	Statement.Condition.Append(" And ObjectName = '").Append(ItemName).Append("'");
	Result = Database->Select(Statement);
	if (Result.ResultRows.Num() > 0) return true;

	return false;
}

FSQLQueryResult UDatabaseProxy::Select(FStatement Statement)
{
	FSQLQueryResult Result;
	Result = Database->Select(Statement);
	return Result;
}

bool UDatabaseProxy::Update(FString TableName, FString Id, TMap<FString, FString> ValuesMap)
{
	const auto ImmediateMessage = Database->Update(TableName, Id, ValuesMap);
	return ImmediateMessage.Success;
}

bool UDatabaseProxy::Insert(FString TableName, TArray<FString>& Values)
{
	const auto ImmediateMessage = Database->Insert(TableName, Values);
	return ImmediateMessage.Success;
}

bool UDatabaseProxy::Delete(FStatement Statement)
{
	const auto ImmediateMessage = Database->Delete(Statement);
	return ImmediateMessage.Success;
}

void UDatabaseProxy::Disconnect()
{
	if (DatabaseType == EDatabase::MySQL)
	{
		Database->Shutdown();
	}
}

void UDatabaseProxy::ParseEnumToTableName(TArray<FString>& Output, FString EnumName)
{
	for (int32 Table = ETable::Animation; Table < ETable::Max; ++Table)
	{
		Output.Add(GetEnumShortName(EnumName, Table));
	}
}

FString UDatabaseProxy::GetEnumShortName(FString EnumName, int32 Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameStringByValue(Value);
}

bool UDatabaseProxy::IsUserInfoExist(FString TableName, FString Act, FString Pwd)
{
	FStatement Statement;
	Statement.Select.Add("*");
	Statement.TableName.Append(TableName);
	Statement.Condition.Append("Account = '");
	Statement.Condition.Append(Act).Append("'");
	Statement.Condition.Append(" And Password = '");
	Statement.Condition.Append(Pwd).Append("'");
	auto Result = UDatabaseProxy::Select(Statement);
	if (Result.ResultRows.Num() > 0) return true;

	return false;
}

int32 UDatabaseProxy::GetFinalId(FString TableName)
{
	FStatement Statement;
	Statement.Select.Add("COUNT(Id)");
	Statement.TableName.Append(TableName);
	auto Result = UDatabaseProxy::Select(Statement);
	if (Result.ResultRows.Num() > 0)
	{
		const auto FinalId = FCString::Atoi(*Result.ResultRows[0].Fields[0].Value);
		return FinalId;
	}
	return 0;
}

bool UDatabaseProxy::GetDataIntoObject(FStatement Statement, UObject* ObjectToPopulate)
{
	if (ObjectToPopulate == nullptr) return false;
	if (Database == nullptr) return false;

	auto Result = Database->Select(Statement);
	if (Result.ResultRows.Num() > 0 && Result.Success)
	{
		AssignResultsToObjectProperties(Result.ResultRows[0], ObjectToPopulate);
		return true;
	}

	return false;
}

void UDatabaseProxy::AssignResultsToObjectProperties(const FSQLQueryResultRow& ResultValue, UObject* ObjectToPopulate)
{
	auto propertyMap = CollectProperties(ObjectToPopulate);
	for (auto field : ResultValue.Fields)
	{
		if (propertyMap.Contains(field.Key))
		{
			UProperty* targetProperty = propertyMap[field.Key];
			if (UInt64Property* int64prop = Cast<UInt64Property>(targetProperty))
			{
				int64prop->SetPropertyValue_InContainer(ObjectToPopulate, FCString::Atoi64(*field.Value));
			}
			else if (UIntProperty* int32prop = Cast<UIntProperty>(targetProperty))
			{
				int32prop->SetPropertyValue_InContainer(ObjectToPopulate, FCString::Atoi(*field.Value));
			}
			else if (UBoolProperty* boolProp = Cast<UBoolProperty>(targetProperty))
			{
				boolProp->SetPropertyValue_InContainer(ObjectToPopulate, FCString::ToBool(*field.Value));
			}
			else if (UDoubleProperty* doubleProp = Cast<UDoubleProperty>(targetProperty))
			{
				doubleProp->SetPropertyValue_InContainer(ObjectToPopulate, FCString::Atod(*field.Value));
			}
			else if (UFloatProperty* floatProp = Cast<UFloatProperty>(targetProperty))
			{
				floatProp->SetPropertyValue_InContainer(ObjectToPopulate, FCString::Atof(*field.Value));
			}
			else if (UStrProperty* strProp = Cast<UStrProperty>(targetProperty))
			{
				strProp->SetPropertyValue_InContainer(ObjectToPopulate, field.Value);
			}
		}
	}
}

TMap<FString, UProperty*> UDatabaseProxy::CollectProperties(UObject* SourceObject)
{
	UClass* SourceObjectClass = SourceObject->GetClass();
	TMap<FString, UProperty*> Props;
	for (TFieldIterator<UProperty> PropIt(SourceObjectClass, EFieldIteratorFlags::SuperClassFlags::IncludeSuper);
		PropIt; ++PropIt)
	{
		Props.Add(*PropIt->GetNameCPP(), *PropIt);
	}

	return Props;
}
