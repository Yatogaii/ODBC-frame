// FROM https://blog.csdn.net/cn_lyt/article/details/121418359

#include<iostream>
#include<Windows.h>
#include<sql.h>
#include<sqltypes.h>
#include<sqlext.h>
#include "dm_demo.h"
using  namespace std;
#define RC_SUCCESSFUL(rc) ((rc) == SQL_SUCCESS || (rc) == SQL_SUCCESS_WITH_INFO)// #define SQL_SUCCEEDED(rc)  (((rc)&(~1))==0) 

#define RC_NOTSUCCESSFUL(rc) (!(RC_SUCCESSFUL(rc)))


int dm_test()
{
	SQLHENV henv;/* 环境句柄 */
	SQLHDBC hdbc;/* 连接句柄 */
	SQLHDBC hstmt;/* 语句句柄 */
	SQLRETURN sret; /* 返回代码 */
	void getError(SQLHDBC hhandle, SQLSMALLINT TYPE);
	/* 申请一个环境句柄 */
	sret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	/* 设置环境句柄的 ODBC 版本 */
	sret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3,
		SQL_IS_INTEGER);
	/* 申请一个连接句柄 */
	sret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	sret = SQLConnectW(hdbc, (SQLWCHAR*)L"dm8-docker", SQL_NTS, (SQLWCHAR*)L"SYSDBA", SQL_NTS, (SQLWCHAR*)L"SYSDBA001", SQL_NTS);
	if (RC_NOTSUCCESSFUL(sret)) {
		getError(hdbc, SQL_HANDLE_DBC);
		return 0;
	}
	SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	wchar_t sql[1024];

	swprintf(sql, 1024, L"%hs", "INSERT INTO box_baseinfo VALUES (86251025,4,0.80,1,'2022-11-03 11:26:36')");

	sret = SQLExecDirect(hstmt, sql, sizeof(sql) / 2);

	if (RC_NOTSUCCESSFUL(sret)) {
		getError(hstmt, SQL_HANDLE_STMT);
		return 0;
	}

	/* 释放语句句柄 */
	SQLFreeHandle(SQL_HANDLE_DBC, hstmt);
	/* 释放连接句柄 */
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	/* 释放环境句柄 */
	SQLFreeHandle(SQL_HANDLE_ENV, henv);

	return 0;
}

void getError(SQLHDBC hhandle, SQLSMALLINT TYPE)
{
	/* 连接数据源失败! */
	SQLINTEGER   NumRecords = 0;
	SQLGetDiagField(TYPE,
		hhandle,
		0,
		SQL_DIAG_NUMBER,
		&NumRecords,
		SQL_IS_INTEGER,
		NULL);
	printf("Total Number of diagnostic records: %d\n", NumRecords);

	SQLSMALLINT   Counter = 0;
	SQLINTEGER     NativeErr = 0;
	SQLWCHAR      SQLState[6];
	SQLWCHAR      ErrMsg[512];
	SQLSMALLINT  ErrMsgLen = 0;
	for (Counter = 1; Counter <= NumRecords; Counter++)
	{
		SQLGetDiagRec(TYPE,
			hhandle,
			Counter,
			SQLState,
			&NativeErr,
			ErrMsg,
			sizeof(ErrMsg) / 2,
			&ErrMsgLen);
		printf("SQLSTATE : %ls\n", SQLState);
		printf("%ls\n", ErrMsg);
	}
}