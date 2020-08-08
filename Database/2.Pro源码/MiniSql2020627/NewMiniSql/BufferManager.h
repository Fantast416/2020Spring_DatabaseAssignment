#pragma once
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : BufferManager.H                                              *
 *                                                                                             *
 *                   Programmer : Fantast(Lvkesheng Shen)                                      *
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

#ifndef _BUFFER_H_
#define _BUFFER_H_
#include<sys/types.h>
#include<sys/stat.h>
#include <vector>
#include "global.h"
#include "Error.h"
#include <io.h>
#include <fcntl.h>

#define PageHeadSize 8   // 为8个字节 
#define FileHeadSize 540   // 为540个字节
#define FileAddressSize 8 // 为8个字节
//一个文件中有许多页，有一个文件头，每一页有一个页头
//文件头包含在第一个文件页中
class Clock;
class Buffer;
Clock* GetGlobalClock(); //获取全局的内存页管理类的方法
Buffer& GetGlobalBuffer();
class FileHead;
class PageHead {
public:
	void SetPageId(unsigned long PageId);                         //文件页头  文件头  
	void SetFixed(bool IsFixed);
	void Initialize();
private:
	unsigned long PageId;
	bool IsFixed;  //内存页是否被锁定，如果被锁定则不会被关闭
};

class Page {
	friend class Buffer;
	friend class Clock;
	friend class File;
public:
	PageHead* Pagehead;                // 页头指针
	FILE* FilePointer;                 // 文件指针，当其置为NULL时，该页被抛弃
	unsigned long FilePageID;          // 文件页号
	mutable bool IsLastUsed;           // 最近一次访问内存是否被使用，用于Clock算法
	mutable bool IsModified;           // 是否脏页
	void *PtrtoPageBeginAddress;       // 内存中实际保存物理文件数据的起始地址
public:
	//function()
	void WriteMemToFile() const;            // 把内存中的页写回到文件中
	FileHead* GetFileHead();           // 获取文件头指针（当文件页为0的时候调用此函数，来返回对应的文件头指针，即指向文件头开始的地址）
	Page();
	~Page();
	void SetModified(bool State);
	void SetLastUsed(bool State);

};

const unsigned int FILECOND_RESERVE_SPACE = 512;  // 文件头预留空间

class FileAddress {         //文件内某数据的地址 = 文件内页编号 + 页内偏移量 
	friend class File;
public:
	void SetFileAddress(unsigned long FilePageID, unsigned int  OffSet);   //设置FileAddress的变量值
	bool operator==(const FileAddress &rhs) const;
	bool operator!=(const FileAddress &rhs) const;
	bool operator<(const FileAddress &rhs)const;
	bool operator>(const FileAddress &rhs)const;
	unsigned long FilePageID;     // 文件页编号
	unsigned int  OffSet;         // 页内偏移量

};

class FileHead {
public:
	void Initialize();                           //初始化新的文件的文件头信息
	FileAddress DelFirst;                         // 第一条被删除记录地址  
	FileAddress DelLast;                          // 最后一条被删除记录地址  
	FileAddress NewInsert;                        // 文件末尾可插入新数据的地址

	unsigned long TotalPage;                     // 目前文件中共有页数
	char Reserve[FILECOND_RESERVE_SPACE];         // 预留空间
};
#define MAX_FILENAME_LEN 100
class File {
	friend class Buffer;
	friend class Page;
public:
	char* GetFileName();
	char FileName[MAX_FILENAME_LEN];
	FILE* FilePointer;                                                 // 文件指针
	unsigned long TotalPage;                                      // 目前文件中共有页数

//function()
	File(const char *FileName, FILE* FilePointer);
	Page* AddExtraPage();                                       // 当前文件添加一页空间
	Page* GetFileFirstPage();                                    // 得到文件首页

	void* ReadMem(FileAddress *ReadAddress);                           // 读取内存文件,返回读取位置指针
	FileAddress WriteMem(const void* Source, unsigned int Length);           // 在可写入地址写入数据
	FileAddress WriteMem(const void* Source, unsigned int Length, FileAddress* Dest);
	//void WipeMem(void*Source, unsigned int WipeSize, FileAddress *WipePos);

public:  //对外接口函数
	const void* ReadRecord(FileAddress *RecordAddress)const;         // 读取某条记录,返回记录指针(包括记录地址数据)
	//void* ReadWriteRecord(FileAddress *RecordAddress);              // 读取某条记录,返回记录指针(包括记录地址数据)
	FileAddress AddRecord(const void* RecordSource, unsigned int SizeofRecord);                    // 返回记录所添加的位置
	FileAddress DeleteRecord(FileAddress *DeleteAddress, unsigned int SizeofRecord);               // 返回删除的位置
	bool UpdateRecord(FileAddress *DeleteAddress, void *UpdateRecordData, unsigned int SizeofRecord);

};


class Clock {  //内存页管理类
	friend class File;
	friend class Buffer;
public:
	Clock();
	~Clock();
	Page* MemPages[MemPageCount + 1];  // 内存页对象数组  MemPageCount是内存页数量

//function()
	Page* GetMemAddr(FILE* FilePointer, unsigned long FilePageID);  //获得物理文件页（某一物理文件页用文件指针和页编号表示）在内存中的地址

	// 返回一个可替换的内存页索引
	// 原页面内容该写回先写回
	unsigned int GetUseablePage();

	// 创建新页，适用于创建新文件或者添加新页的情况下
	Page* CreateNewPage(FILE* FilePointer, unsigned long FilePageId);
};

class Buffer {       //读写文件用的最底层类
	friend class Clock;

public:
	~Buffer();
	File* operator[](const char *fileName);      // 打开文件，打开失败返回 nullptr
	void CreateFile(const char *FileName);
	File* GetFile(const char *FileName);

	void CloseFile(const char *FileName);
	void CloseAllFile();
private:
	std::vector<File*> MemFiles;  // 保存已经打开的文件列表,存的是文件指针

};

#endif