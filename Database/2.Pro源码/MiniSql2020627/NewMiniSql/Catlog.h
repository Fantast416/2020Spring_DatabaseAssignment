/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : Timer.H                                                      *
 *                                                                                             *
 *                   Programmer : YuZhe Luo                                                    *
 *                                                                                             *
 *                   Start Date : 06/08/2020                                                   *
 *                                                                                             *
 *                  Last Update : 06/21/2020			                                       *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef __CATLOG_H__
#define __CATLOG_H__
#include"BTree.h"
#include"Error.h"
class CatalogPosition;
// 目录定位和切换 用于数据库和表的使用
class CatalogPosition
{
	friend bool Use_DB(std::string db_name, CatalogPosition& cp);//选择数据库

public:
	CatalogPosition();
	bool ResetRootCatalog(std::string root_new);//重置根目录
	void SwitchToDb();//转到数据库列表目录下
	bool SwitchToDb(std::string db);//转到指定的数据库
	std::string GetCurrentPath()const;//获得当前访问路径
	std::string GetRootPath()const;//获得根路径
	std::string SetCurrentPath(std::string path);//设定路径

	bool IsInSpeDb()
	{
		return isInSpeDb;
	}
	bool SetIsInSpeDb(bool tmp)
	{
		isInSpeDb = tmp;
		return tmp;
	}

	static bool isInSpeDb;//是否在具体的数据库目录下
	std::string root;//根目录
	std::string current_catalog;
};
CatalogPosition& GetCp();

/************************************************************************
*    索引文件头信息管理
************************************************************************/
class TableIndexHeadInfo
{
private:
	BTree& tree;
public:
	TableIndexHeadInfo(BTree &in_tree):tree(in_tree){}//构造函数要求传入索引B+树
	unsigned int GetColumnCount()const;                 //获得表的字段个数
	unsigned int GetIndexCount()const;					//获得表的索引个数
	std::vector<std::string> GetIndexNames()const;		//各个索引名字
	std::vector<std::string> GetColumnNames()const;     //各个字段的名字
	std::vector<Column_Type> GetColumnType()const;      //各个字段的类型
	std::vector<int> GetColumnSize()const;              //各个字段的大小
	int GetPrimaryIndex()const;                         //主键字段的索引
	std::vector<int> GetUniqueIndex()const;				//unique字段的索引 
	std::vector<int> GetIndexInfo()const;				//获得建立索引字段的位置信息
	int GetColumnSizeByIndex(int i)const;               //第i个字段的数据大小
	bool IsColumnName(std::string column_name)const;    //判断该字段是不是表的字段
	bool IsPrimary(std::string column_name)const;       //判断字段名是否为主键字段
	bool IsUnique(std::string column_name)const;       //判断字段名是否为unique字段
	bool hasIndex(std::string column_name)const;		//判断该字段有没有建索引
	Column_Type GetColumnType(std::string column_name)const;//返回该字段的类型
	int GetIndex(std::string column_name)const;         //返回该字段在所有字段中的索引位置
	int GetIndex2(std::string column_name)const;         //返回该字段在索引中的索引位置
	int GetColumnOffset(std::string column_name);       //返回给定字段名字段距离数据头地址的偏移
};
/************************************************************************
*    索引字段信息
************************************************************************/
struct Idx_Create_Info                                                         //
{                                                                             //
	std::string index_name;								//索引名
	std::string table_name;                            // 表名          //
	std::string column_name;							//列名    //
};

/************************************************************************
*    表字段信息
************************************************************************/
//定义记录各个字段的类型
//字段类型 I-- - int  C-- - char  F-- - float
/********************************************************************************/
//新建表要用到的两个结构体
typedef struct ColumnInfo               // 新建表的字段信息                   //*
{																			  //*
	std::string name;                                                         //
	Column_Type type;                                                         //
	bool isPrimary;                                // 是否主键                //
	bool isUnique;                                 // 是否允许重复            //
	int RequiredLength;                            // 用户定义字段数据长度    //
}ColumnInfo;																  //
/************************************************************************     //
*    表操作信息结构体
************************************************************************/     //
struct TB_Create_Info                                                         //
{                                                                             //
	std::string table_name;                            // 新建的表名          //
	std::vector<ColumnInfo> columns_info;              // 表的各个字段        //
};

/********************************************************************************/

/********************************************************************************/
//插入记录要用到的两个结构体
typedef struct InsertInfo {
	std::string column_name; //插入的字段
	std::string column_value;//插入的值
}InsertInfo;

struct TB_Insert_Info
{
	bool IsOrder;
	std::string table_name;
	std::vector<InsertInfo> insert_info;
};
/********************************************************************************/

/********************************************************************************/
//删除记录要用到的两个结构体
typedef struct Expr {			//条件匹配表达式     where name = 'name1';   即 field = ‘name’  op = ‘=’value = 'name1'
	std::string field;			//字段名
	std::string op;				//符号
	std::string value;			//值
}Expr;

struct TB_Delete_Info
{
	std::string table_name;
	std::vector<Expr> expr;             // 删除的字段条件
};
/********************************************************************************/

/********************************************************************************/
//查找记录要用到的结构体
enum Operator_Type { B, BE, L, LE, E, NE };
Operator_Type GetOperatorType(std::string s);  //将运算符< > >=此类转换成 L B 此类的函数
//字段比较单元类 
class Cell_Compare  
{
public:
	Cell_Compare(Operator_Type ot, Column_Cell cc) :operator_type(ot),cmp_value(cc){}
	Operator_Type operator_type;
	Column_Cell cmp_value;
	bool operator()(const Column_Cell &cc)//传入进行比较的另外字段
{
	switch (cmp_value.column_type)
	{
	case Value_Type::I:
		switch(operator_type)
		{
		case B:
			return cc.column_value.IntValue > cmp_value.column_value.IntValue;
			break;
		case BE:
			return cc.column_value.IntValue >= cmp_value.column_value.IntValue;
			break;
		case L:
			return cc.column_value.IntValue < cmp_value.column_value.IntValue;
			break;
		case LE:
			return cc.column_value.IntValue <= cmp_value.column_value.IntValue;
			break;
		case E:
			return cc.column_value.IntValue == cmp_value.column_value.IntValue;
			break;
		case NE:
			return cc.column_value.IntValue != cmp_value.column_value.IntValue;
			break;
		default:
			return false;
			break;
		}
		break;
	case Value_Type::F:
		switch (operator_type)
		{
		case B:
			return cc.column_value.FloatValue > cmp_value.column_value.FloatValue;
			break;
		case BE:
			return cc.column_value.FloatValue >= cmp_value.column_value.FloatValue;
			break;
		case L:
			return cc.column_value.FloatValue < cmp_value.column_value.FloatValue;
			break;
		case LE:
			return cc.column_value.FloatValue <= cmp_value.column_value.FloatValue;
			break;
		case E:
			return cc.column_value.FloatValue == cmp_value.column_value.FloatValue;
			break;
		case NE:
			return cc.column_value.FloatValue != cmp_value.column_value.FloatValue;
			break;
		default:
			return false;
			break;
		}
		break;
	case Value_Type::C:
		switch (operator_type)
		{
		case B:
			return std::string(cc.column_value.CharValue) > std::string(cmp_value.column_value.CharValue);
			break;
		case BE:
			return std::string(cc.column_value.CharValue) >= std::string(cmp_value.column_value.CharValue);
			break;
		case L:
			return std::string(cc.column_value.CharValue) < std::string(cmp_value.column_value.CharValue);
			break;
		case LE:
			return std::string(cc.column_value.CharValue) <= std::string(cmp_value.column_value.CharValue);
			break;
		case E:
			return std::string(cc.column_value.CharValue) == std::string(cmp_value.column_value.CharValue);
			break;
		case NE:
			return std::string(cc.column_value.CharValue) != std::string(cmp_value.column_value.CharValue);
			break;
		default:
			return false;
			break;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
  }
};
//在interpreter里用到，将一个sql语句解析成为这么一个结构体，传入api中，进行操作的执行
struct TB_Select_Info
{
	std::string table_name; //选择的表名
	std::vector <std::string> name_select_column;//选择的字段名字
	std::vector<Cell_Compare> vec_cmp_cell;//选择条件
};
//api中操作执行完以后，将结果生成一个SelectPrintInfo 结构体，重新传入interpreter的printwindow类中，进行输出结果的打印
struct SelectPrintInfo
{
	std::string table_name;
	std::vector<std::string> name_selected_column;
	std::vector<std::pair<Key_Attr, FileAddress>> key_fd;  // keys 和 fds 保存着对应的关键字以及该关键字对应的记录地址

};

/********************************************************************************/
//更新记录要用到的结构体
typedef struct NewValue {
	std::string field;
	std::string value;
}NewValue;
//Expr 是上述删除时定义的结构体,用于记录 一个 表达式例如：  name = 'name1'    field op value
struct TB_Update_Info
{
	std::string table_name;
	std::vector<NewValue> field_value;  // 字段——值
	std::vector<Expr> expr;             // 更新的字段条件
};

#endif
