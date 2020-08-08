#pragma once
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
#ifndef __BPTREE_H__
#define __BPTREE_H__
#include "BufferManager.h"
#include "Record.h"
#include"Error.h"
#include <queue>
#include <assert.h>

class IndexHeadNode
{
public:
	FileAddress    root;                                       // the address of the root
	FileAddress    MostLeftNode;                               // the address of the most left node
	int         KeyTypeIndex;                               // 关键字字段的位置  即第几个
	char        RecordTypeInfo[RecordColumnCount];          // 记录字段类型信息
	char        RecordColumnName[RecordColumnCount / 4 * ColumnNameLength];
	int			UniqueKeyIndex[RecordColumnCount];							//记录Unique位置

	char        RecordIndexName[RecordColumnCount / 4 * ColumnNameLength]; //记录索引名
	int			RecordIndexInfo[RecordColumnCount];					//记录索引位置
};

// define B+tree Node
enum class NodeType { ROOT, INNER, LEAF };
class BTNode
{
public:
	NodeType node_type;                              // 节点类型
	int count_valid_key;                             // 该节点中储存的关键字数目

	Key_Attr key[MaxKeyCount];                        // array of keys
	FileAddress children[MaxChildCount];             // if the node is not a leaf node, children store the children pointer
													 // otherwise it store record address;

	FileAddress next;                                 // if leaf node
	void PrintSelf();
};

class BTree
{
	//friend vector<Column_Head> ShowTable(string table_name, string path);
	//friend Column_Head GetDbfRecord(std::string table_name, FileAddress fd, string path);
public:
	// 参数：索引文件名称， 关键字类型， 记录各个类型信息数组， 记录各个字段名称信息数组
	BTree(string idx_name);
	BTree(const string idx_name, int KeyTypeIndex, int(&_UniqueKeyIndex)[RecordColumnCount], char(&_RecordTypeInfo)[RecordColumnCount], char(&_RecordColumnName)[RecordColumnCount / 4 * ColumnNameLength]);          // 创建索引文件的B+树
	BTree(const string idx_name, int KeyTypeIndex, char(&_RecordTypeInfo)[RecordColumnCount], char(&_RecordColumnName)[RecordColumnCount / 4 * ColumnNameLength]);          // 创建索引文件的B+树
	~BTree() { }
	FileAddress Search(Key_Attr search_key);                                        // 查找关键字是否已经存在
	bool Insert(Key_Attr k, FileAddress k_fd);                                      // 插入关键字k
	FileAddress UpdateKey(Key_Attr k, Key_Attr k_new);                               // 返回关键字对应的记录地址
	FileAddress Delete(Key_Attr k);                                                 // 返回该关键字记录在数据文件中的地址
	void PrintBTreeStruct();                                                    // 层序打印所有结点信息
	void PrintAllLeafNode();
	IndexHeadNode *GetPtrIndexHeadNode();
	BTNode *FileAddrToMemPtr(FileAddress node_fd);                                 // 文件地址转换为内存指针
	//void InsertIndexName();

private:
	FileAddress DeleteKeyAtInnerNode(FileAddress x, int i, Key_Attr key);              // x的下标为i的结点为叶子结点
	FileAddress DeleteKeyAtLeafNode(FileAddress x, int i, Key_Attr key);               // x的下标为i的结点为叶子结点
	void InsertNotFull(FileAddress x, Key_Attr k, FileAddress k_fd);					// 在一个非满结点 x, 插入关键字 k, k的数据地址为 k_fd
	void SplitChild(FileAddress x, int i, FileAddress y);                             // 分裂x的孩子结点x.children[i] , y
	FileAddress Search(Key_Attr search_key, FileAddress node_fd);                      // 判断关键字是否存在
	FileAddress SearchInnerNode(Key_Attr search_key, FileAddress node_fd);             // 在内部节点查找
	FileAddress SearchLeafNode(Key_Attr search_key, FileAddress node_fd);              // 在叶子结点查找


private:
	FILE* file_id;
	string str_idx_name;
	IndexHeadNode idx_head;
};


#endif