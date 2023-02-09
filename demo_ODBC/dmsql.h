#ifndef _DMSQL_H_
#define _DMSQL_H_

#include <sql.h>
#include <string>
#include <sqltypes.h>
#include <sqlext.h>

class DMConnector{
public:
	// 弃用无参构造
	DMConnector() = delete;

	/// DMConnect 构造函数
	/// 参数:
	///		target		: string -> windows ODBC配置的名称，控制面板内设置
	///		user_name   : string -> 登录数据库用户名
	///     user_passwd	: string -> 登录数据库密码
	DMConnector(std::string target, std::string user_name, std::string user_passwd);

	// DMConnect 析构函数，实现RAII。
	~DMConnector();

	// DMPing 函数，可用做心跳包，也可用作连接成功与否测试。
	bool DMPing();

	// DMExecute 执行sql语句，返回DMResult
	DMResult DMExecute(std::string sql);

private:
	// static 句柄，整个程序仅需要一个。
	static 	SQLHENV henv;  /* 环境句柄 */
	SQLHDBC hdb_con;  /* 连接句柄 */
	SQLHDBC hstatement; /* 语句句柄 */
	SQLRETURN sret; /* 返回代码 */
};

class DMResult {

};

#endif