#pragma once
#pragma once
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : Record.H                                                     *
 *                                                                                             *
 *                   Programmer : Lunium Luk (Ziyi Lu)                                         *
 *                                                                                             *
 *                   Start Date : 06/11/2020                                                   *
 *                                                                                             *
 *                  Last Update : ***			                                               *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * class Record {                                                                              *
 *		FileAddress InsertRecord(const string file_name, Column_Head& h);					   *
 *		FileAddress DeleteRecord(const string file_name, FileAddress fa, size_t);			   *
 *		bool UpdateRecord(const string file_name, Column_Head& h, FileAddress fa);			   *
 * };			                                                                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef RECORD_H_
#define RECORD_H

#include <iostream>
#include <string.h>
#include <string>
#include "BufferManager.h"
#include "Error.h"
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

enum class Value_Type { I, F, C, ERROR = -1 };

Value_Type ConvertStringToValueType(std::string str);

union KeyValue {
	int     IntValue;
	char    CharValue[18];
	double   FloatValue;
};

/*
*
*	Key_Attr类，记录一个键值
*
*/

class Key_Attr {
public:

	Value_Type type;
	KeyValue value;
	Key_Attr();
	Key_Attr(int value) {
		type = Value_Type::I;
		this->value.IntValue = value;
	}
	Key_Attr(double value) {
		type = Value_Type::F;
		this->value.FloatValue = value;
	}
	Key_Attr(char* value) {
		type = Value_Type::C;
		strcpy(this->value.CharValue, value);
	}
	Key_Attr(const Key_Attr& k);
	Key_Attr& operator=(const Key_Attr& k);

	bool operator<(const Key_Attr& k)const;
	bool operator>(const Key_Attr& k)const;
	bool operator==(const Key_Attr& k)const;
	bool operator<=(const Key_Attr& k)const;
	bool operator>=(const Key_Attr& k)const;
	bool operator!=(const Key_Attr& k)const;
	friend ostream& operator<<(std::ostream& os, const Key_Attr& key);
};

std::ostream& operator<<(std::ostream& os, const Key_Attr& key);

/*
*
*	Column_Cell类，用于记录单条数据
*
*/
union Column_Value
{
	int   		        IntValue;		 //整形值
	double 		        FloatValue;     //浮点型值
	char*               CharValue;	     //字符串指针 
};

class Column_Cell
{
public:
	Value_Type 		column_type;
	string 			column_name;
	Column_Value 		column_value;
	Column_Cell* next;
	size_t 			sz;

	Column_Cell();
	Column_Cell(Key_Attr k);
	Column_Cell(const Column_Cell& c);
	Column_Cell& operator=(const Column_Cell& c);

	size_t size()const {
		if (column_type == Value_Type::I) {
			return sizeof(int);
		}else if (column_type == Value_Type::F) {
			return sizeof(double);
		}
		else {
			return sz;
		}
	}
	void* data()const;

	operator Key_Attr()const;
};

/*
*
*	Column_Head类，用于记录一组数据
*
*	这里attachCell未使用拷贝构造，使用移动构造 --> 可能会有问题
*
*/

class Column_Head {
private:
	Column_Cell* front;
	Column_Cell* rear;
public:
	Column_Head();
	Column_Head(Column_Cell c);
	Column_Head(const Column_Head& h);
	Column_Head& operator=(const Column_Head& h);

	void attachCell(Column_Cell& c);
	size_t size();
	Column_Cell* firstCell();
};

std::ostream& operator<<(std::ostream& os, Column_Head& h);

/*
*
*	Record类，用于记录的插入/更新/删除
*
*/

class Record
{
public:
	FileAddress InsertRecord(const string file_name, Column_Head& h);
	FileAddress DeleteRecord(const string file_name, FileAddress fa, size_t);
	bool UpdateRecord(const string file_name, Column_Head& h, FileAddress fa);
	//private:
	std::pair<unsigned long, char*> GetRecordData(Column_Head& h);
};

#endif // !RECORD_H_