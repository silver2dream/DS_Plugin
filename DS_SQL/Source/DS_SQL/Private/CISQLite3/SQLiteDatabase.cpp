#include "SQLiteDatabase.h"
#include "PlatformFilemanager.h"
#include "Paths.h"

#include "FileHelper.h"

constexpr char* RELEVANT_SQLITEFILE_PATH = u8"DS_SQL/Resources/SQL/CispoDatabase.SQLite.sql";
constexpr char* RELEVANT_SQLITE_CREATE_TABLE_FILE_PATH = u8"DS_SQL/Resources/SQL/CreateTable.SQLite.sql";

TMap<FString, FString> USQLiteDatabase::Databases;

//--------------------------------------------------------------------------------------------------------------

USQLiteDatabase::USQLiteDatabase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::RegisterDatabase(FString Name, FString Filename, bool RelativeToGameContentDirectory)
{
	FString actualFilename = Filename;

	if (RelativeToGameContentDirectory)
	{
		actualFilename = FPaths::ProjectContentDir() + Filename;
	}
		
	if (!IsValidDatabase(actualFilename, true))
	{
		FString message = "Unable to add database '" + actualFilename + "', it is not valid (problems opening it)!";
		return false;
	}

	if (IsDatabaseRegistered(Name))
	{
		FString message = "Database '" + actualFilename + "' is already registered, skipping.";
		return true;
	}

	Databases.Add(Name, actualFilename);

	FString successMessage = "Registered SQLite database '" + actualFilename + "' successfully.";
	return true;

}

//--------------------------------------------------------------------------------------------------------------

//bool USQLiteDatabase::GetDataIntoObject(const FString& DatabaseName, const FString& Query, UObject* ObjectToPopulate)
//{
//	//////////////////////////////////////////////////////////////////////////
//	// Check input validness.
//	//////////////////////////////////////////////////////////////////////////
//
//	if (ObjectToPopulate == NULL)
//	{
//		////LOGSQLITE(Error, TEXT("ObjectToPopulate needs to be set to get any results!"));
//		return false;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// Validate the database
//	//////////////////////////////////////////////////////////////////////////
//
//	if (!IsDatabaseRegistered(DatabaseName) ||
//		!IsValidDatabase(Databases[DatabaseName], false))
//	{
//		////LOGSQLITE(Error, *FString::Printf(TEXT("Unable to get data into object, invalid database '%s'"), *DatabaseName));
//		return false;
//	}
//
//	if (!CanOpenDatabase(Databases[DatabaseName]))
//	{
//		////LOGSQLITE(Error, *FString::Printf(TEXT("Unable to open database '%s'"), *DatabaseName));
//		return false;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// Get the results
//	//////////////////////////////////////////////////////////////////////////
//
//	SQLiteQueryResult queryResult = RunQueryAndGetResults(DatabaseName, Query);
//
//	if (queryResult.Success && queryResult.Results.Num() > 0)
//	{
//		AssignResultsToObjectProperties(queryResult.Results[0], ObjectToPopulate);
//		return true;
//	}
//	else if (!queryResult.Success)
//	{
//		////LOGSQLITE(Error, *FString::Printf(TEXT("Query resulted in an error: '%s'"), *queryResult.ErrorMessage));
//		return false;
//	}
//	else if (queryResult.Results.Num() == 0)
//	{
//		////LOGSQLITE(Error, TEXT("Query returned zero rows, no data to assign to object properties."));
//		return false;
//	}
//
//	return false;
//}

//--------------------------------------------------------------------------------------------------------------

//bool USQLiteDatabase::GetDataIntoObjectBP(const FSQLiteDatabaseReference& DataSource, TArray<FString> Fields,
//	FSQLiteQueryFinalizedQuery Query, UObject* ObjectToPopulate)
//{
//	//////////////////////////////////////////////////////////////////////////
//	// Check input validness.
//	//////////////////////////////////////////////////////////////////////////
//
//	if (ObjectToPopulate == NULL)
//	{
//		////LOGSQLITE(Error, TEXT("ObjectToPopulate needs to be set to get any results!"));
//		return false;
//	}
//
//	if (DataSource.Tables.Num() == 0)
//	{
//		////LOGSQLITE(Error, TEXT("The query needs the table name!"));
//		return false;
//	}
//
//	if (Fields.Num() == 0)
//	{
//		////LOGSQLITE(Error, TEXT("The query needs fields! You may use * to get all fields."));
//		return false;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// Validate the database
//	//////////////////////////////////////////////////////////////////////////
//
//	if (!IsDatabaseRegistered(DataSource.DatabaseName) ||
//		!IsValidDatabase(Databases[DataSource.DatabaseName], true))
//	{
//		////LOGSQLITE(Error, TEXT("Unable to get data to object, database validation failed!"));
//		return false;
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// Get the results
//	//////////////////////////////////////////////////////////////////////////
//
//	FString constructedQuery = ConstructQuery(DataSource.Tables, Fields, Query, 1, 0);
//
//	SQLiteQueryResult queryResult = RunQueryAndGetResults(DataSource.DatabaseName, constructedQuery);
//
//	if (queryResult.Success && queryResult.Results.Num() > 0)
//	{
//		AssignResultsToObjectProperties(queryResult.Results[0], ObjectToPopulate);
//		return true;
//	}
//	else if (!queryResult.Success)
//	{
//		////LOGSQLITE(Error, *FString::Printf(TEXT("Query resulted in an error: '%s'"), *queryResult.ErrorMessage));
//		return false;
//	}
//	else if (queryResult.Results.Num() == 0)
//	{
//		////LOGSQLITE(Error, TEXT("Query returned zero rows, no data to assign to object properties."));
//		return false;
//	}
//
//	return false;
//
//}

//--------------------------------------------------------------------------------------------------------------

TMap<FString, UProperty*> USQLiteDatabase::CollectProperties(UObject* SourceObject)
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

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::IsDatabaseRegistered(FString DatabaseName)
{
	return Databases.Contains(DatabaseName);
}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::CanOpenDatabase(FString DatabaseFilename)
{
	sqlite3* db;
	if (sqlite3_open(TCHAR_TO_ANSI(*DatabaseFilename), &db) == SQLITE_OK)
	{
		sqlite3_close(db);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::IsValidDatabase(FString DatabaseFilename, bool TestByOpening)
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*DatabaseFilename))
	{
		if (TestByOpening)
		{
			return CanOpenDatabase(DatabaseFilename);
		}
		else
		{
			return true;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------

FSQLQueryResult USQLiteDatabase::GetData(const FString& DatabaseName, const FString& Query)
{
	FSQLQueryResult result;

	//////////////////////////////////////////////////////////////////////////
	// Validate the database
	//////////////////////////////////////////////////////////////////////////

	if (!IsDatabaseRegistered(DatabaseName) ||
		!IsValidDatabase(Databases[DatabaseName], true))
	{
		////LOGSQLITE(Error, TEXT("Unable to get data to object, database validation failed!"));
		result.Success = false;
		result.ErrorMessage = TEXT("Database validation failed");
		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	// Get the results
	//////////////////////////////////////////////////////////////////////////

	SQLiteQueryResult queryResult = RunQueryAndGetResults(DatabaseName, Query);
	result.Success = queryResult.Success;
	result.ErrorMessage = queryResult.ErrorMessage;

	for (auto row : queryResult.Results)
	{
		FSQLQueryResultRow outRow;
		for (auto field : row.Fields)
		{
			FSQLKeyValuePair outField;
			outField.Key = field.Name;
			outField.Value = field.ToString();

			outRow.Fields.Add(outField);
		}
		result.ResultRows.Add(outRow);
	}

	return result;

}

//--------------------------------------------------------------------------------------------------------------

FSQLQueryResult USQLiteDatabase::GetDataBP(const FSQLiteDatabaseReference& DataSource,
	TArray<FString> Fields, FSQLiteQueryFinalizedQuery Query, int32 MaxResults, int32 ResultOffset)
{

	FSQLQueryResult result;

	//////////////////////////////////////////////////////////////////////////
	// Check input validness.
	//////////////////////////////////////////////////////////////////////////

	if (DataSource.Tables.Num() == 0)
	{
		//LOGSQLITE(Error, TEXT("The query needs at least one table name!"));
		result.Success = false;
		result.ErrorMessage = TEXT("No table given");
		return result;
	}

	if (Fields.Num() == 0)
	{
		//LOGSQLITE(Error, TEXT("The query needs fields! You can use * to get all fields."));
		result.Success = false;
		result.ErrorMessage = TEXT("No fields given");
		return result;
	}

	FString constructedQuery = ConstructQuery(DataSource.Tables, Fields, Query, MaxResults, ResultOffset);

	return GetData(DataSource.DatabaseName, constructedQuery);

}

//--------------------------------------------------------------------------------------------------------------

FString USQLiteDatabase::ConstructQuery(TArray<FString> Tables, TArray<FString> Fields,
	FSQLiteQueryFinalizedQuery QueryObject, int32 MaxResults, int32 ResultOffset)
{
	FString fieldString;
	for (int32 i = 0; i < Fields.Num(); i++)
	{
		fieldString.Append(Fields[i] + (i < Fields.Num() - 1 ? "," : ""));
	}

	FString tableList = FString::Join(Tables, TEXT(","));
	TArray<FString> allQueryParams;

	allQueryParams.Add(FString::Printf(TEXT("SELECT %s FROM %s"), *fieldString, *tableList));

	if (QueryObject.Query.Len() > 0)
	{
		allQueryParams.Add(FString::Printf(TEXT("WHERE %s"), *QueryObject.Query));
	}

	if (MaxResults >= 0)
	{
		allQueryParams.Add(FString::Printf(TEXT("LIMIT %i"), MaxResults));
	}

	if (ResultOffset > 0)
	{
		allQueryParams.Add(FString::Printf(TEXT("OFFSET %i"), ResultOffset));
	}

	FString finalQuery = FString::Join(allQueryParams, TEXT(" "));
	return finalQuery;

}

//--------------------------------------------------------------------------------------------------------------

void USQLiteDatabase::PrepareStatement(const FString Query, sqlite3** db, int32** SqlReturnCode, sqlite3_stmt** PreparedStatement)
{
	**SqlReturnCode = sqlite3_open(TCHAR_TO_UTF8(*Databases[CurrentDatabaseName]), db);

	**SqlReturnCode = sqlite3_prepare_v2(*db, TCHAR_TO_UTF8(*Query), -1, PreparedStatement, NULL);
}

//--------------------------------------------------------------------------------------------------------------

FSQLiteTable USQLiteDatabase::CreateTable(const FString DatabaseName, const FString TableName,
	const TArray<FSQLiteTableField> Fields, const FSQLitePrimaryKey PK)
{
	FSQLiteTable t;
	t.DatabaseName = DatabaseName;
	t.TableName = TableName;
	t.Fields = Fields;
	t.PK = PK;

	FString query = "";
	query += "CREATE TABLE IF NOT EXISTS ";
	query += TableName;
	query += "(";

	bool singlePrimaryKeyExists = false;

	for (const FSQLiteTableField& field : Fields)
	{
		if (field.ResultStr.Len() > 2) {

			if (field.ResultStr.Contains("PRIMARY KEY")) {
				singlePrimaryKeyExists = true;
			}

			query += field.ResultStr + ", ";

		}

	}

	if (singlePrimaryKeyExists) {
		query = query.Left(query.Len() - 2);

		query += ");";
	}
	else {
		if (PK.ResultStr.Len() > 2) {
			query += " " + PK.ResultStr + " ";
		}
		else {
			query = query.Left(query.Len() - 2);
		}

		query += ");";
	}

	////LOGSQLITE(Warning, *query);

	t.Created = ExecSql(DatabaseName, query);

	return t;

}


bool USQLiteDatabase::IsTableExist(FString Name)
{
	FStatement Statement;
	Statement.QueryString.Append("SELECT Id From ").Append(Name);
	const auto Result = Select(Statement);
	if (Result.ResultRows.Num() > 0) return true;
	
	return false;
}

bool USQLiteDatabase::CreateTable(FString Name)
{
	FString FullPath = FPaths::ProjectPluginsDir().Append(RELEVANT_SQLITE_CREATE_TABLE_FILE_PATH);
	FString Content;
	FFileHelper::LoadFileToString(Content, *FullPath);
	Content = Content.Replace(TEXT("TableName"), *Name);
	if (!ExecSql(CurrentDatabaseName, TCHAR_TO_UTF8(*Content)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Table %s Fail !"), *Name);
		return false;
	}
		
	return true;
}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::DropTable(const FString DatabaseName, const FString TableName)
{
	bool idxCrSts = true;


	FString query = "DROP TABLE " + TableName;

	////LOGSQLITE(Warning, *query);

	idxCrSts = ExecSql(DatabaseName, query);

	return idxCrSts;

}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::TruncateTable(const FString DatabaseName, const FString TableName)
{
	bool idxCrSts = true;


	FString query = "DELETE FROM " + TableName + ";";

	////LOGSQLITE(Warning, *query);

	idxCrSts = ExecSql(DatabaseName, query);

	return idxCrSts;

}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::Vacuum(const FString DatabaseName)
{
	bool idxCrSts = true;


	FString query = "VACUUM; ";

	////LOGSQLITE(Warning, *query);

	idxCrSts = ExecSql(DatabaseName, query);

	return idxCrSts;

}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::ExecSql(const FString DatabaseName, const FString Query) {

	bool execStatus = false;

	char *zErrMsg = 0;
	sqlite3 *db;

	ANSICHAR* dbNameAsUtf8 = TCHAR_TO_UTF8(*Databases[DatabaseName]);
	int32 i = sqlite3_open(dbNameAsUtf8, &db);

	if (i == SQLITE_OK){

		int32 k = sqlite3_exec(db, TCHAR_TO_UTF8(*Query), NULL, 0, &zErrMsg);

		if (k == SQLITE_OK){
			execStatus = true;
		}
		else {
			FString errStr(zErrMsg);
			//LOGSQLITE(Warning, *errStr);
		}

	}
	else {
		//LOGSQLITE(Warning, TEXT("CreateTable - DB Open failed.."));
	}

	sqlite3_close(db);

	return execStatus;
}

FSQLQueryResult USQLiteDatabase::Select(FStatement Statement)
{
	FSQLQueryResult Result;
	Result = GetData(CurrentDatabaseName, Statement.GetQuery());
	return Result;
}

FSQLImmediateMessage USQLiteDatabase::Insert(FString TableName, TArray<FString>& Values)
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

	sqlite3 *db{};
	sqlite3_stmt *PreStatement{};
	int32 SQLReturnCode = 0;
	int32 *PreCode = &SQLReturnCode;
	PrepareStatement(InsertCmd, &db, &PreCode, &PreStatement);
	SQLReturnCode = *PreCode;

	FSQLImmediateMessage Result;
	if (SQLReturnCode == SQLITE_OK)
	{
		auto FieldNum = 0;
		for (auto Field : Values)
		{
			++FieldNum;
			if (Field.Equals("NULL") || Field.Equals("Null"))
			{
				sqlite3_bind_null(PreStatement, FieldNum);
			}
			else if (Field.IsNumeric())
			{
				if (Field.Contains("f") || Field.Contains("."))
				{
					sqlite3_bind_double(PreStatement, FieldNum, FCString::Atod(*Field));
					continue;
				}
				sqlite3_bind_int(PreStatement, FieldNum, FCString::Atoi(*Field));
			}
			else
			{
				sqlite3_bind_text(PreStatement, FieldNum, TCHAR_TO_UTF8(*Field), -1, SQLITE_TRANSIENT);
			}
		}
		SQLReturnCode = sqlite3_step(PreStatement);
		Result.Success = SQLReturnCode;
		if (SQLReturnCode != SQLITE_OK)
		{
			Result.ErrorMessage = TEXT("Insert Error");
		}
		sqlite3_finalize(PreStatement);
	}
	else
	{
		Result.Success = SQLReturnCode;
		Result.ErrorMessage = TEXT("Prepare Error");
	}
	sqlite3_close(db);

	return Result;
}

FSQLImmediateMessage USQLiteDatabase::Update(FString TableName, FString Id, TMap<FString, FString> ValuesMap)
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

	sqlite3 *db{};
	sqlite3_stmt *PreStatement{};
	int32 SQLReturnCode = 0;
	int32 *PreCode = &SQLReturnCode;
	PrepareStatement(Cmd, &db, &PreCode, &PreStatement);
	SQLReturnCode = *PreCode;
	
	FSQLImmediateMessage Result;
	if (SQLReturnCode == SQLITE_OK)
	{
		auto FieldNum = 0;
		for (auto Field : ValuesMap)
		{
			++FieldNum;
			if (Field.Value.Equals("NULL") || Field.Value.Equals("Null"))
			{
				sqlite3_bind_null(PreStatement, FieldNum);
			}
			else if (Field.Value.IsNumeric())
			{
				if (Field.Value.Contains("f")|| Field.Value.Contains("."))
				{
					sqlite3_bind_double(PreStatement, FieldNum, FCString::Atod(*Field.Value));
					continue;
				}
				sqlite3_bind_int(PreStatement, FieldNum, FCString::Atoi(*Field.Value));
			}
			else
			{
				sqlite3_bind_text(PreStatement, FieldNum, TCHAR_TO_UTF8(*Field.Value), -1, SQLITE_TRANSIENT);
			}
		}
		SQLReturnCode = sqlite3_step(PreStatement);
		Result.Success = SQLReturnCode;
		if (SQLReturnCode != SQLITE_OK)
		{
			Result.ErrorMessage = TEXT("Update Error");
		}
		sqlite3_finalize(PreStatement);
	}
	else
	{
		Result.Success = SQLReturnCode;
		Result.ErrorMessage = TEXT("Prepare Error");
	}
	sqlite3_close(db);
	
	return Result;
}

FSQLImmediateMessage USQLiteDatabase::Delete(FStatement Statement)
{
	FSQLImmediateMessage Result;
	char *ErrMsg = 0;
	sqlite3 *db{};

	auto DatabaseName = TCHAR_TO_UTF8(*Databases[CurrentDatabaseName]);
	int32 ReturnCode = sqlite3_open(DatabaseName, &db);

	if (ReturnCode == SQLITE_OK) {

		ReturnCode = sqlite3_exec(db, TCHAR_TO_UTF8(*Statement.GetDelete()), NULL, 0, &ErrMsg);
		if (ReturnCode == SQLITE_OK) {
			Result.Success = true;
		}
		else {
			FString errStr(ErrMsg);
			Result.ErrorMessage.Append(errStr);
		}
	}
	else {
		Result.ErrorMessage.Append("Open SQLiteDatabase Fail!");
	}
	sqlite3_close(db);

	return Result;
}

bool USQLiteDatabase::IsConnect(TMap<int32, FString> ConnectProperties)
{
	TArray<FString> PathPart;
	ConnectProperties[EConnectProperty::DatabasePath].ParseIntoArray(PathPart, TEXT("/"), true);
	auto DbName = PathPart.Pop(true);

	if (!USQLiteDatabase::IsDatabaseRegistered(DbName))
	{
		if (!USQLiteDatabase::RegisterDatabase(DbName, *ConnectProperties[EConnectProperty::DatabasePath], false))
			return false;
	}
	CurrentDatabaseName = DbName;
	return true;
}

bool USQLiteDatabase::TestingConnect(TMap<int32, FString> ConnectProperties)
{
	FString TextToSave = FString("");
	if (FFileHelper::SaveStringToFile(TextToSave, *ConnectProperties[EConnectProperty::DatabasePath]))
	{
		if (IsConnect(ConnectProperties))
			return true;
	}

	return false;
}

bool USQLiteDatabase::CloneDatabase(FString SchemaName)
{
	FString FullPath = FPaths::ProjectPluginsDir().Append(RELEVANT_SQLITEFILE_PATH);
	FString Content;
	FFileHelper::LoadFileToString(Content, *FullPath);
	if (!ExecSql(CurrentDatabaseName, TCHAR_TO_UTF8(*Content))) return false;

	return true;
}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::CreateIndexes(const FString DatabaseName, const FString TableName, const TArray<FSQLiteIndex> Indexes)
{
	bool idxCrSts = true;

	for (const FSQLiteIndex& idx : Indexes)
	{
		if (idx.ResultStr.Len() > 2) {
			FString query = idx.ResultStr.Replace(TEXT("$$$TABLE_NAME$$$"), *TableName);

			////LOGSQLITE(Warning, *query);

			idxCrSts = ExecSql(DatabaseName, query);
			if (!idxCrSts) {
				////LOGSQLITE(Warning, TEXT("ExecSql break"));
				break;
			}
		}

	}

	return idxCrSts;
}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::CreateIndex(const FString DatabaseName, const FString TableName, const FSQLiteIndex Index)
{
	bool idxCrSts = true;

	FString query = Index.ResultStr.Replace(TEXT("$$$TABLE_NAME$$$"), *TableName);

	////LOGSQLITE(Warning, *query);

	idxCrSts = ExecSql(DatabaseName, query);

	return idxCrSts;

}

//--------------------------------------------------------------------------------------------------------------

bool USQLiteDatabase::DropIndex(const FString DatabaseName, const FString IndexName)
{
	bool idxCrSts = true;

	FString query = "DROP INDEX " + IndexName;

	////LOGSQLITE(Warning, *query);

	idxCrSts = ExecSql(DatabaseName, query);

	return idxCrSts;

}

//--------------------------------------------------------------------------------------------------------------

//bool USQLiteDatabase::IsTableExists(const FString DatabaseName, const FString TableName)
//{
//
//	//sqlite3* db;
//	//int32 sqlReturnCode = 0;
//	//int32* sqlReturnCode1 = &sqlReturnCode;
//	//sqlite3_stmt* preparedStatement;
//
//	//FString Query = "SELECT * FROM sqlite_master WHERE type='table' AND name='" + TableName + "';";
//
//	//PrepareStatement(&DatabaseName, &Query, &db, &sqlReturnCode1, &preparedStatement);
//	//sqlReturnCode = *sqlReturnCode1;
//
//	//if (sqlReturnCode != SQLITE_OK)
//	//{
//	//	const char* errorMessage = sqlite3_errmsg(db);
//	//	FString error = "SQL error: " + FString(UTF8_TO_TCHAR(errorMessage));
//	//	//LOGSQLITE(Error, *error);
//	//	//LOGSQLITE(Error, *FString::Printf(TEXT("The attempted query was: %s"), *Query));
//	//	sqlite3_finalize(preparedStatement);
//	//	sqlite3_close(db);
//	//}
//
//	//bool tableExists = false;
//
//	//for (sqlReturnCode = sqlite3_step(preparedStatement);
//	//	sqlReturnCode != SQLITE_DONE && sqlReturnCode == SQLITE_ROW;
//	//	sqlReturnCode = sqlite3_step(preparedStatement))
//	//{
//	//	tableExists = true;
//	//	break;
//	//}
//
//	////////////////////////////////////////////////////////////////////////////
//	//// Release the statement and close the connection
//	////////////////////////////////////////////////////////////////////////////
//
//	//sqlite3_finalize(preparedStatement);
//	//sqlite3_close(db);
//
//	//return tableExists;
//
//}

void USQLiteDatabase::InsertRowsIntoTable(const FString DatabaseName, const FString TableName, TArray<FSQLiteTableRowSimulator> rowsOfFields){
	for (FSQLiteTableRowSimulator row : rowsOfFields) {
		FString query = "INSERT INTO " + TableName + " (";
		for (FSQLiteTableField field : row.rowsOfFields) {
			query += field.FieldName + ", ";
		}

		query = query.Left(query.Len() - 2);

		query = query + ") VALUES (";
		for (FSQLiteTableField field : row.rowsOfFields) {
			if (field.FieldType.Equals(TEXT("TEXT"))) {
				query = query + "'" + field.FieldValue + "', ";
			}
			else {
				query = query + field.FieldValue + ", ";
			}
		}

		query = query.Left(query.Len() - 2);
		query = query + ");";

		////LOGSQLITE(Warning, *query);

		ExecSql(DatabaseName, query);

	}
}

//--------------------------------------------------------------------------------------------------------------

SQLiteQueryResult USQLiteDatabase::RunQueryAndGetResults(FString DatabaseName, FString Query)
{
	SQLiteQueryResult result;

	sqlite3* db;
	int32 sqlReturnCode = 0;
	int32* sqlReturnCode1 = &sqlReturnCode;
	sqlite3_stmt* preparedStatement;

	PrepareStatement(Query, &db, &sqlReturnCode1, &preparedStatement);
	sqlReturnCode = *sqlReturnCode1;

	if (sqlReturnCode != SQLITE_OK)
	{
		const char* errorMessage = sqlite3_errmsg(db);
		FString error = "SQL error: " + FString(UTF8_TO_TCHAR(errorMessage));
		//LOGSQLITE(Error, *error);
		//LOGSQLITE(Error, *FString::Printf(TEXT("The attempted query was: %s"), *Query));
		result.ErrorMessage = error;
		result.Success = false;
		sqlite3_finalize(preparedStatement);
		sqlite3_close(db);
		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	// Get and assign the data
	//////////////////////////////////////////////////////////////////////////

	TArray<SQLiteResultValue> resultRows;

	for (sqlReturnCode = sqlite3_step(preparedStatement);
		sqlReturnCode != SQLITE_DONE && sqlReturnCode == SQLITE_ROW;
		sqlReturnCode = sqlite3_step(preparedStatement))
	{
		SQLiteResultValue row;

		//LOGSQLITE(Verbose, TEXT("Query returned a result row."));
		int32 resultColumnCount = sqlite3_column_count(preparedStatement);
		for (int32 c = 0; c < resultColumnCount; c++)
		{
			int32 columnType = sqlite3_column_type(preparedStatement, c);
			const char* columnName = sqlite3_column_name(preparedStatement, c);
			FString columnNameStr = UTF8_TO_TCHAR(columnName);
			SQLiteResultField val;
			val.Name = columnNameStr;
			switch (columnType)
			{
			case SQLITE_INTEGER:
				val.Type = SQLiteResultValueTypes::Integer;
				val.IntValue = sqlite3_column_int64(preparedStatement, c);
				break;
			case SQLITE_TEXT:
				val.Type = SQLiteResultValueTypes::Text;
				val.StringValue = UTF8_TO_TCHAR(sqlite3_column_text(preparedStatement, c));
				break;
			case SQLITE_FLOAT:
				val.Type = SQLiteResultValueTypes::Float;
				val.DoubleValue = sqlite3_column_double(preparedStatement, c);
				break;
			case SQLITE_NULL:
			default:
				val.Type = SQLiteResultValueTypes::UnsupportedValueType;
				val.StringValue = FString("NULL");
			}

			/*if (val.Type != SQLiteResultValueTypes::UnsupportedValueType)
			{
				row.Fields.Add(val);
			}*/
			row.Fields.Add(val);
		}

		resultRows.Add(row);
	}

	//////////////////////////////////////////////////////////////////////////
	// Release the statement and close the connection
	//////////////////////////////////////////////////////////////////////////

	sqlite3_finalize(preparedStatement);
	sqlite3_close(db);

	result.Results = resultRows;
	result.Success = true;
	return result;

}

//--------------------------------------------------------------------------------------------------------------

void USQLiteDatabase::AssignResultsToObjectProperties(const SQLiteResultValue& ResultValue, UObject* ObjectToPopulate)
{
	auto propertyMap = CollectProperties(ObjectToPopulate);
	for (SQLiteResultField field : ResultValue.Fields)
	{
		if (propertyMap.Contains(field.Name))
		{
			UProperty* targetProperty = propertyMap[field.Name];

			if (field.Type == SQLiteResultValueTypes::Integer)
			{
				UInt64Property* int64prop = NULL;
				UIntProperty* int32prop = NULL;
				UInt16Property* int16prop = NULL;
				UInt8Property* int8prop = NULL;
				UBoolProperty* boolProp = NULL;

				if ((int64prop = Cast<UInt64Property>(targetProperty)) != NULL)
				{
					int64prop->SetPropertyValue_InContainer(ObjectToPopulate, field.IntValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((int32prop = Cast<UIntProperty>(targetProperty)) != NULL)
				{
					int32prop->SetPropertyValue_InContainer(ObjectToPopulate, (int32)field.IntValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((int16prop = Cast<UInt16Property>(targetProperty)) != NULL)
				{
					int16prop->SetPropertyValue_InContainer(ObjectToPopulate, (int16)field.IntValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((int8prop = Cast<UInt8Property>(targetProperty)) != NULL)
				{
					int8prop->SetPropertyValue_InContainer(ObjectToPopulate, (int8)field.IntValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
				else if ((boolProp = Cast<UBoolProperty>(targetProperty)) != NULL)
				{
					boolProp->SetPropertyValue_InContainer(ObjectToPopulate, field.IntValue > 0);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%d'"), *field.Name, field.IntValue));
				}
			}

			else if (field.Type == SQLiteResultValueTypes::Float)
			{
				UDoubleProperty* doubleProp = NULL;
				UFloatProperty* floatProp = NULL;
				if ((doubleProp = Cast<UDoubleProperty>(targetProperty)) != NULL)
				{
					doubleProp->SetPropertyValue_InContainer(ObjectToPopulate, field.DoubleValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%f'"), *field.Name, field.DoubleValue));
				}
				else if ((floatProp = Cast<UFloatProperty>(targetProperty)) != NULL)
				{
					floatProp->SetPropertyValue_InContainer(ObjectToPopulate, (float)field.DoubleValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%f'"), *field.Name, field.DoubleValue));
				}
			}

			else if (field.Type == SQLiteResultValueTypes::Text)
			{
				UStrProperty* strProp = NULL;
				if ((strProp = Cast<UStrProperty>(targetProperty)) != NULL)
				{
					strProp->SetPropertyValue_InContainer(ObjectToPopulate, field.StringValue);
					//LOGSQLITE(Verbose, *FString::Printf(TEXT("Property '%s' was set to '%s'"), *field.Name, *field.StringValue.Mid(0, 64)));
				}
			}

		}
	}
}

//--------------------------------------------------------------------------------------------------------------
