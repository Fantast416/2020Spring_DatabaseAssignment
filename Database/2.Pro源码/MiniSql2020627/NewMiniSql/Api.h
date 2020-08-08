#pragma once
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : API.H                                                        *
 *                                                                                             *
 *                   Programmer : Fantast(Lvkesheng Shen)&& Yuzhe Luo                          *
 *                                                                                             *
 *                   Start Date : 06/08/2020                                                   *
 *                                                                                             *
 *                  Last Update : 06/21/2020			                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 /*
 * Copyright C Fantast
 * https://github.com/Fantast416
 */
#ifndef __API_H__
#define __API_H__
#include "Record.h"
#include <algorithm>
#include"Global.h"
#include"Catlog.h"
#include"Error.h"
#include"BTree.h"
#include"Interpreter.h"
#include"Timer.h"
//class Cell_Compare;
//
////记录查找关键符
//enum Operator_Type { B, BE, L, LE, E, NE };
//Operator_Type GetOperatorType(std::string str);


///************************************************************************
//*    操作集成
//************************************************************************/
//创建数据库
bool Create_DB(std::string db_name, CatalogPosition& cp);

//选择数据库
bool Use_DB(std::string db_name, CatalogPosition& cp);

//显示当前所有数据库
std::vector<std::string> Show_DB(CatalogPosition& cp);

//删除数据库
bool Drop_DB(std::string db_name, CatalogPosition& cp);

void Del_File(std::string folder_path);

//创建表
bool Create_Table(TB_Create_Info tb_create_info, std::string path = std::string("./"));
void Check_TB_Create_Info(const TB_Create_Info &tb_create_info);

//删除表
bool Drop_Table(std::string table_name, std::string path = std::string("./"));

//插入记录
bool Insert_Record(TB_Insert_Info tb_insert_info, std::string path = std::string("./"));
void Check_TB_Insert_Info(const TB_Insert_Info &tb_insert_info);
void Check_Unique_Info(std::string table_name, std::string column_name, std::string column_value, char KeyType, int column_id, std::string path = std::string("./"));
void Check_Type_I(std::string value);
void Check_Type_F(std::string value);

//创建索引
bool Create_Index(Idx_Create_Info idx_create_info, std::string path = std::string("./"));

//删除索引
bool Drop_Index(std::string index_name, std::string path = std::string("./"));

//*******************查找/更新/删除记录时需要用到的相关函数*****************************************************************************************************
// 范围查找单个字段 返回查找的值在数据文件中的地址																											//**
//单条件选择																																				//**
std::vector<std::pair<Key_Attr, FileAddress>> Condition_Select(Cell_Compare cell_compare, std::string table_name, std::string path = std::string("./"));	//**
//关键字选择																																				//**
std::vector<std::pair<Key_Attr, FileAddress>> Key_Select(Cell_Compare cell_compare, std::string table_name, std::string path = std::string("./"));			//**
//范围选择																																					//**
std::vector<std::pair<Key_Attr, FileAddress>> Range_Select(Cell_Compare cell_compare, std::string table_name, std::string path = std::string("./"));		//**
////取出指定地址的数据																																		//**
Column_Head Get_DB_Record(std::string table_name, FileAddress file_address, std::string path = std::string("./"));											//**
																																							//**
////返回表中各个字段名称与类型																																//**
std::vector<std::pair<std::string, Column_Type>> GetColumn_Name_Type(std::string table_name, std::string table_path);										//**
Column_Type GetType(std::string name, std::vector<std::pair<std::string, Column_Type>> pair);																//**
//**************************************************************************************************************************************************************

//*******************查找记录*******************************************************************************
SelectPrintInfo Select_Record(TB_Select_Info tb_select_info, std::string path = std::string("./")); //	****
void Check_TB_Select_Info(const TB_Select_Info& tb_select_info);//表查找信息检查						****
//**********************************************************************************************************


//*******************删除记录*******************************************************************************
bool Delete_Record(TB_Delete_Info tb_delete_info, std::string path = std::string("./"));			//	****
void Check_TB_Delete_Info(const TB_Delete_Info &tb_delete_info);                                    //  ****
//**********************************************************************************************************


//*******************更新记录 ******************************************************************************
bool Update_Record(TB_Update_Info tb_update_info, std::string path = std::string("./"));			//	****
//**********************************************************************************************************


#endif