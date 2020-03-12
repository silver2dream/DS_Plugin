// Fill out your copyright notice in the Description page of Project Settings.

#include "MySqlAPI.h"

#include "Paths.h"
#include "FileHelper.h"

#include <cppconn/prepared_statement.h>

constexpr unsigned int MYSQL_FIELD_START = 1;
constexpr char* RELEVANT_SQLFILE_PATH = u8"DS_SQL/Resources/SQL/CispoDatabase.db.sql";
constexpr char* RELEVANT_CREATE_TABLE_FILE_PATH = u8"DS_SQL/Resources/SQL/CreateTable.sql";

bool UMySqlAPI::IsConnect(TMap<int32, FString> ConnectProperties)
{
	/*try
	{*/
	Schema = ConnectProperties[EConnectProperty::Schema];
		
	Connect_Properties["hostName"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::HostName]); /*"tcp://35.194.143.27:3306";*/
	Connect_Properties["userName"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::UserName]); /*"root";*/
	Connect_Properties["password"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::Password]); /*"1234";*/
	Connect_Properties["schema"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::Schema]);     /*"CispoTest";*/
	Connect_Properties["port"] = FCString::Atoi(*ConnectProperties[EConnectProperty::Port]);        /*3306;*/
	Connect_Properties["OPT_RECONNECT"] = true;
	Connect_Properties["CLIENT_MULTI_STATEMENTS"] = true;

	MySqlDriver = sql::mysql::get_mysql_driver_instance();
	if (MySqlDriver)
	{
		Conn = MySqlDriver->connect(Connect_Properties);
		if (Conn->isValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Get Connect"));
			stmt = Conn->createStatement();
			if (stmt)
			{

				return true;
			}
		}
	}
	//}
	//catch (const sql::SQLException& e)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(e.what()));
	//}

	return false;
}

FSQLQueryResult UMySqlAPI::Select(FStatement Statement)
{
	FSQLQueryResult Result;
	try
	{
		if (!stmt)
		{
			return Result;
		}

		auto Data = stmt->executeQuery(TCHAR_TO_UTF8(*Statement.GetQuery()));
		if (!IsDataExist(Data, Result))
		{
			return Result;
		}

		while (Data->next())
		{
			auto Meta = Data->getMetaData();
			const auto FieldNum = Meta->getColumnCount();
			FSQLQueryResultRow Row;
			for (unsigned int index = MYSQL_FIELD_START; index <= FieldNum; ++index)
			{
				FSQLKeyValuePair Pair;
				Pair.Key = FString(Meta->getColumnName(index).c_str());
				Pair.Value = FString(Data->getString(index).c_str());

				Row.Fields.Add(Pair);
			}
			Result.ResultRows.Add(Row);
		}
		Result.Success = true;
	}
	catch (const sql::SQLException& e)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(e.what()));
	}

	return Result;
}

FSQLImmediateMessage UMySqlAPI::Insert(FString TableName, TArray<FString>& Values)
{
	FString InsertCmd = "INSERT INTO ";
	InsertCmd.Append(TableName);
	InsertCmd.Append(" VALUES(");
	for (auto index = 0; index < Values.Num(); ++index)
	{
		InsertCmd.Append("?");
		if (index + 1 != Values.Num())
			InsertCmd.Append(",");
	}
	InsertCmd.Append(")");
	
	FSQLImmediateMessage Result;
	try
	{
		auto pstmt = Conn->prepareStatement(TCHAR_TO_UTF8(*InsertCmd));
		for (auto index = 0; index < Values.Num(); ++index)
		{
			auto FieldNum = index + 1;

			if (Values[index].Equals("NULL") || Values[index].Equals("Null"))
			{
				pstmt->setNull(FieldNum, 0);
				continue;
			}

			if (Values[index].IsNumeric())
			{
				pstmt->setInt(FieldNum, FCString::Atoi(*Values[index]));
			}
			else
			{
				pstmt->setString(FieldNum, TCHAR_TO_UTF8(*Values[index]));

			}
		}

		Result.Success = pstmt->executeUpdate();
	}
	catch (const sql::SQLException& e)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *FString(e.what()));
		Result.ErrorMessage.Append(e.what());
	}
	return Result;
}

FSQLImmediateMessage UMySqlAPI::Update(FString TableName, FString Id, TMap<FString, FString> ValuesMap)
{
	auto FieldCount = 0;
	FString Cmd = "UPDATE ";
	Cmd.Append(TableName);
	Cmd.Append(" Set ");
	for (auto Field : ValuesMap)
	{
		Cmd.Append(Field.Key).Append("=").Append("?");
		if (FieldCount + 1 != ValuesMap.Num())
		{
			Cmd.Append(", ");
			FieldCount += 1;
		}
	}
	Cmd.Append(" WHERE Id =");
	Cmd.Append(Id);

	auto pstmt = Conn->prepareStatement(TCHAR_TO_UTF8(*Cmd));
	auto FieldNum = 0;
	for (auto Field : ValuesMap )
	{
		++FieldNum;
		if (Field.Value.IsEmpty())
		{
			pstmt->setNull(FieldNum, 0);
			continue;
		}

		if (Field.Value.IsNumeric())
		{
			pstmt->setInt(FieldNum, FCString::Atoi(*Field.Value));
		}
		else
		{
			pstmt->setString(FieldNum, TCHAR_TO_UTF8(*Field.Value));
		}
	}

	FSQLImmediateMessage Result;
	/*try
	{*/
		Result.Success = pstmt->executeUpdate();
	/*}
	catch (const sql::SQLException& e)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(e.what()));
		Result.ErrorMessage.Append(e.what());
	}*/
	return Result;
}

FSQLImmediateMessage UMySqlAPI::Delete(FStatement Statement)
{
	FSQLImmediateMessage Result;
	/*try
	{*/
		Result.Success = stmt->executeUpdate(TCHAR_TO_UTF8(*Statement.GetDelete()));
	/*}
	catch (const sql::SQLException& e)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *FString(e.what()));
		Result.ErrorMessage.Append(e.what());
	}*/
	return Result;
}

void UMySqlAPI::Shutdown()
{
	if (stmt)
	{
		stmt->close();
		stmt = nullptr;
	}
	if (Conn)
	{
		Conn->close();
		Conn = nullptr;
	}
	MySqlDriver = nullptr;
	
}

bool UMySqlAPI::IsTableExist(FString Name)
{
	FStatement Statement;
	Statement.QueryString.Append("SELECT * FROM information_schema.tables ");
	Statement.QueryString.Append("WHERE table_schema = '").Append(Schema).Append("' ");
	Statement.QueryString.Append("AND table_name = '").Append(Name).Append("' ");
	Statement.QueryString.Append("LIMIT 1");
	
	const auto Result = Select(Statement);
	if (Result.ResultRows.Num() > 0) return true;

	return false;
}

bool UMySqlAPI::CreateTable(FString Name)
{
	auto IsSucceed = false;
	if (stmt)
	{
		FString FullPath = FPaths::ProjectPluginsDir().Append(RELEVANT_CREATE_TABLE_FILE_PATH);
		FString Content;
		FFileHelper::LoadFileToString(Content, *FullPath);
		Content = Content.Replace(TEXT("TableName"), *Name);
		if (!stmt->execute(TCHAR_TO_UTF8(*Content)))
		{
			IsSucceed = true;
		}
		
		if (!IsSucceed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Create Table %s Fail !"), *Name);
		}
	}
	return IsSucceed;
}

bool UMySqlAPI::TestingConnect(TMap<int32, FString> ConnectProperties)
{
	Connect_Properties["hostName"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::HostName]); /*"tcp://35.194.143.27:3306";*/
	Connect_Properties["userName"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::UserName]); /*"root";*/
	Connect_Properties["password"] = TCHAR_TO_UTF8(*ConnectProperties[EConnectProperty::Password]); /*"1234";*/
	Connect_Properties["port"] = FCString::Atoi(*ConnectProperties[EConnectProperty::Port]);        /*3306;*/
	Connect_Properties["OPT_RECONNECT"] = true;
	Connect_Properties["CLIENT_MULTI_STATEMENTS"] = true;

	/*try
	{*/
		MySqlDriver = sql::mysql::get_mysql_driver_instance();
		if (MySqlDriver)
		{
			Conn = MySqlDriver->connect(Connect_Properties);
			if (Conn->isValid())
			{
				stmt = Conn->createStatement();
				if (stmt)
				{
					return true;
				}
			}
		}
	/*}
	catch (const sql::SQLException& e)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(e.what()));
	}*/

	return false;
}

bool UMySqlAPI::UMySqlAPI::CloneDatabase(FString SchemaName)
{
	auto IsSucceed = false;
	if (stmt)
	{
		/*try
		{ */
			FString FullPath = FPaths::ProjectPluginsDir().Append(RELEVANT_SQLFILE_PATH);
			FString Content;
			FFileHelper::LoadFileToString(Content, *FullPath);
			Content = Content.Replace(TEXT("SchemaName"), *SchemaName);
			if (!stmt->execute(TCHAR_TO_UTF8(*Content)))
			{
				IsSucceed = true;
			}
		/*}
		catch (const sql::SQLException& e)
		{
			UE_LOG(LogTemp, Warning, TEXT("SQLException %s"), *FString(e.what()));
		}*/
	}
	return IsSucceed;
}

bool UMySqlAPI::IsDataExist(sql::ResultSet* Data, FSQLQueryResult& Result)
{
	if (!Data)
	{
		Result.Success = false;
		Result.ErrorMessage = TEXT("Result Was Null");
		return false;
	}
	return true;
}

bool UMySqlAPI::IsDatabaseExist()
{
	auto Result = stmt->executeQuery("SELECT * FROM Configuration");
	if (Result)
	{
		return true;
	}
	return false;
}
