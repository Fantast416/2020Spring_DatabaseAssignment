#pragma once
#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : Timer.H                                                      *
 *                                                                                             *
 *                   Programmer : Shuting Xing                                                 *
 *                                                                                             *
 *                   Start Date : 06/08/2020                                                   *
 *                                                                                             *
 *                  Last Update : 06/21/2020			                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "API.h"
#include "Error.h"
//#include"Catalog.h"
using namespace std;

// 打印命令行窗口，使底层实现和GUI分离，便于扩展
#define PRINTLENGTH 63
class PrintWindow
{
public:
	void CreateTable(bool is_created);
	void CreateIndex(bool is_created);
	void DropIndex(bool is_dropped);
	void DropTable(bool is_dropped);

	void SelectTable(SelectPrintInfo select_table_print_info);
	void InsertRecord(bool is_inserted);
	void UpdateTable(bool isUpdated);
	void DeleteTable(bool isDeleted);
	void ShowAllTable(vector<string> sen_str, string path);

	void CreateDB(bool is_created);
	void DropDB(bool is_dropped);
	void ShowDB(std::vector<std::string> db_names);
	void UseDB(bool isUsed);


private:
	void Print(int len, string s); // 打印 |xxxx        | 其中竖线内长度为 len
	int GetColumnLength(std::string name, std::vector<std::string> col_name, std::vector<int> col_len);

};

/***********将命令字符串解析为有意字串***************/
class SensefulStr
{
public:
	SensefulStr();
	void SetStr(string _srcstr);
	vector<string> GetSensefulStr()const;

private:
	void Parse();                                             // 解析字符串
	string src_str;                                           // 原始字符串
	vector<string> sen_str;                                   // 解析后字符串
	string key_char = ";,()=<>\012\015\040";
	bool IsKeyChar(char c);
};

// 主程序的交互接口 输入
void Interpreter(vector<string> sen_str, CmdType cmd_type, PrintWindow print_window);

CmdType GetOpType(vector<string> sen_str);

//// 表操作相关信息
//bool CreateShowTableInfo(std::vector<std::string> sen_str);
TB_Create_Info CreateTableInfo(std::vector<std::string> sen_str);          //从所给的有意字符串中，形成新创建的表的结构体，传给下一层函数
std::string DropTableInfo(std::vector<std::string> sen_str);  //删除一张表

TB_Insert_Info CreateInsertInfo(std::vector<std::string> sen_str);

TB_Select_Info TableSelectInfo(std::vector<std::string> sen_str);
// 比较的字段名称，比较的字段类型，比较关系，比较的值
Cell_Compare CreateCmpCell(std::string column_name, Column_Type column_type, Operator_Type Optype, std::string value);

//生成表删除结构体
TB_Delete_Info TableDeleteInfo(std::vector<std::string> sen_str);

//生成表更新结构体
TB_Update_Info TableUpdateInfo(std::vector<std::string> sen_str);


//
//数据库相关信息操作
string ShowDbInfo(std::vector<string> sen_str);// 显示数据库
string CreateDbInfo(std::vector<string> sen_str);// 创建数据库,返回要创建的名称
string DeleteDbInfo(std::vector<string> sen_str);// 删除数据库,返回要删除的名称
string UseDbInfo(std::vector<string> sen_str);// 使用数据库

//索引相关操作
Idx_Create_Info CreateIndexInfo(std::vector<std::string> sen_str);
std::string DropIndexInfo(std::vector<std::string> sen_str);

#endif//__INTERPRETER_H__