#include"BufferManager.h"
#include <iostream>
#include <string>
#pragma warning(disable:4996)
Clock* GetGlobalClock() {
	static Clock MemClock;
	return &MemClock;
}
Buffer& GetGlobalBuffer() {
	static Buffer MemBuffer;
	return MemBuffer;
}
File* Buffer::operator[](const char *fileName)
{
	return GetFile(fileName);
}

void PageHead::SetPageId(unsigned long PageId) {
	this->PageId = PageId;
}
void PageHead::SetFixed(bool IsFixed) {
	this->IsFixed = IsFixed;
}
void FileHead::Initialize() {
	this->TotalPage = 1;

	FileAddress temp1;      //设置 DelFirst和DelLast位置初始化为0,0
	temp1.SetFileAddress(0, 0);
	this->DelFirst = temp1;
	this->DelLast = temp1;

	FileAddress temp2;    //设置可以插入记录的位置为该文件页的文件头和页头之后的偏移量的位置
	temp2.SetFileAddress(0, PageHeadSize + FileHeadSize);
	this->NewInsert = temp2;
	//将保留空间内存填为0，以待后续使用
	memset(this->Reserve, 0, FILECOND_RESERVE_SPACE);
}
void PageHead::Initialize() {
	this->PageId = 0;
	this->IsFixed = 1;
}
void Buffer::CreateFile(const char *FileName)
{
	// 文件存在 创建失败
	if (fopen(FileName, "rb") != NULL)
	{
		throw Error("File Is Existed");
	}
	//创建文件
	FILE* NewFile = fopen(FileName, "wb+"); // 新建文件(打开文件) 

	void *ptr = malloc(PageSize);    //PageSize为1页文件页所规定的大小
	memset(ptr, 0, PageSize);
	PageHead* PageHeadPointer = (PageHead *)(ptr);
	FileHead* FileHeadPointer = (FileHead *)((char*)ptr + PageHeadSize);
	PageHeadPointer->Initialize();
	FileHeadPointer->Initialize();
	// 写回
	fwrite(ptr, PageSize, 1, NewFile);  /*size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
										ptr-- 这是指向要被写入的元素数组的指针。
										size-- 这是要被写入的每个元素的大小，以字节为单位。
										nmemb-- 这是元素的个数，每个元素的大小为 size 字节。
										stream-- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。*/
	fclose(NewFile);
	delete ptr;
	return;
}
char* File::GetFileName() {
	return this->FileName;
}
File* Buffer::GetFile(const char *FileName) {
	//先查看该文件是否已经打开
	for (int i = 0; i < this->MemFiles.size(); i++) {
		if ((strcmp(MemFiles[i]->GetFileName(), FileName) == 0))
			return MemFiles[i];
	}
	//若循环结束还未return，则说明此文件尚未打开，则打开该文件
	FILE* fp = fopen(FileName, "rb+");
	if (fp == NULL)
	{
		return NULL;
	}
	else {
		File* NewFile = new File(FileName, fp);
		//????读取过程较为繁杂
		this->MemFiles.push_back(NewFile);
		return NewFile;
	}
}

File::File(const char *FileName, FILE* FilePointer)
{
	strcpy(this->FileName, FileName);
	this->FilePointer = FilePointer;
	this->TotalPage = GetGlobalClock()->GetMemAddr(this->FilePointer, 0)->GetFileHead()->TotalPage;
	//此处调用全局clock类，获取该文件的第0页文件页在内存中的对应内存页的指针，再通过内存页找到内存页页头的地址，拿到TotalPage这个属性，完成内存中一个文件的映射
}

Page* Clock::GetMemAddr(FILE* FilePointer, unsigned long FilePageID) {  //先从文件clock中查找该页是否已经在内存中，如果在则直接获取，不在则从磁盘读入

	Page* MemPage = NULL;
	//遍历所有的内存中已经有的文件页,查找文件页是否已经在内存中载入了
	for (int i = 1; i <= MemPageCount; i++)
	{
		if (MemPages[i] && MemPages[i]->FilePointer == FilePointer && MemPages[i]->FilePageID == FilePageID) {
			MemPage = MemPages[i];
			break;
		}
	}
	if (MemPage != NULL)
		return MemPage;

	// 否则，从磁盘载入
	unsigned int NewPage = GetUseablePage();   //从Clock中找一个可以替换的页出来
	//给该页进行对应的初始化信息记录
	MemPages[NewPage]->FilePointer = FilePointer;
	MemPages[NewPage]->FilePageID = FilePageID;
	MemPages[NewPage]->IsModified = false;
	MemPages[NewPage]->IsLastUsed = true;

	int temp = fseek(FilePointer, FilePageID*PageSize, SEEK_SET);       // 定位到将要取出的文件页的首地址

	//if (temp != 0) {
	//	printf("fseek Error");
	//	return NULL;
	//}

	long byte_count = fread(MemPages[NewPage]->PtrtoPageBeginAddress, PageSize, 1, FilePointer);          // 读到内存中
	/*注意：fread 返回成功读取的对象个数，若出现错误或到达文件末尾，则可能小于count。
		若size或count为零，则fread返回零且不进行其他动作。
		fread不区分文件尾和错误，因此调用者必须用feof和ferror才能判断发生了什么。*/
		/*	if (byte_count == 0) {
			printf("fseek Error");
			return NULL;
		}*/
	return MemPages[NewPage];

}

unsigned int Clock::GetUseablePage()  //找一个可以用来记录的页
{
	int flag = 0;
	// 先查找有没有没分配的内存页，如果有直接拿来用就行了
	for (int i = 1; i <= MemPageCount; i++)
	{
		if (MemPages[i] == NULL)
		{
			MemPages[i] = new Page();
			return i;
		}
		if (MemPages[i]->FilePointer == NULL) {
			flag = i;                    //此处暂时记录一个被抛弃的内存页，如果找完所有没有空的内存页的话，就使用被抛弃的内存页，不用再查找一遍了
		}
	}

	//执行到此处即没有未分配的内存页了，判断一下之前的循环中有没有找到被抛弃的内存页

	if (flag == 0 && MemPages[0]->FilePointer != NULL) {   //若flag没变，并且内存页中第0页未被抛弃则说明，没有已经被抛弃的内存页,要使用置换算法了

		unsigned int i = 1;
		static unsigned long index = 1;
		if (MemPages[index] != nullptr) {
			throw Error("Error!");
		}

		while (MemPages[index]->IsLastUsed)     // 最近被使用过
		{
			MemPages[index]->IsLastUsed = 0;
			index = (index + 1) % MemPageCount;
			if (index == 0)index++;
		}

		auto res = index;
		MemPages[index]->IsLastUsed = 1;
		index = (index + 1) % MemPageCount;
		if (index == 0)index++;
		i = res;

		//unsigned int i = rand() % MemPageCount;   //目前是随机置换 

		if (i == 0)i++;  //如果刚好凑巧碰到了第0页的话是不行的，往后移1页，（为什么不行？为什么要移？）

		MemPages[i]->WriteMemToFile();  //把这个下标对应的内存页写回文件中

		return i;

	}
	else {  //有被抛弃的内存页
		return flag;     //将被抛弃的内存页的数组坐标数返回
	}
}

void Page::WriteMemToFile() const {       // 把内存中的页写回到文件中

		// 只有脏页需要写回，而不是脏页的页是不需要写回的
	if (this->IsModified && this->FilePointer != NULL)
	{
		int temp = 0;
		temp = fseek(this->FilePointer, this->FilePageID*PageSize, SEEK_SET);  //从这个文件开始，向后移文件页 * 文件页大小，就到了当前需要写回的文件页开始

		//if (temp != 0) {
		//	printf("fseek Error");
		//	return;
		//}

		temp = fwrite(this->PtrtoPageBeginAddress, PageSize, 1, this->FilePointer); // 写回文件
		//for (int i = 0; i < PageSize; i++) {
		//	printf("%2X", *((char*)(this->PtrtoPageBeginAddress)+i));
		//}
		//fflush(this->FilePointer);
	/*	if (temp != PageSize) {
			printf("WriteBack Error");
			return;
		}*/
		this->IsModified = false;
		this->IsLastUsed = true;
	}

}

FileHead* Page::GetFileHead() {
	return (FileHead*)((char*)this->PtrtoPageBeginAddress + PageHeadSize);
}

void Buffer::CloseFile(const char *FileName) {
	// 需要把内存中所有保存该文件的页全部写回
	File* MemFile = GetFile(FileName);       //先通过文件名拿到该文件在内存中的一系列信息
	Clock* TempClock = GetGlobalClock();
	//遍历所有内存页，来查找哪些内存页是属于这个要写回的文件的,把他们写回
	for (int i = 1; i <= MemPageCount; i++)
	{
		if (TempClock->MemPages[i] && TempClock->MemPages[i]->FilePointer == MemFile->FilePointer)
		{
			TempClock->MemPages[i]->WriteMemToFile();
			TempClock->MemPages[i]->IsLastUsed = 0;
			TempClock->MemPages[i]->IsModified = false;
			TempClock->MemPages[i]->FilePointer = NULL;
		}
	}
	std::vector<File*>::iterator it;
	//在Clock记录所有已经打开的文件的vector中，找到要关闭的文件，把他关闭，并且把他从vector中剔除
	for (it = MemFiles.begin(); it != MemFiles.end();)
	{
		if (strcmp((*it)->FileName, FileName) == 0)
		{
			fclose((*it)->FilePointer);
			delete (*it);
			MemFiles.erase(it);
			break;
		}
		it++;
	}
}
void Buffer::CloseAllFile() {
	int Len = this->MemFiles.size();
	for (int i = 0; i < Len; i++) {
		this->CloseFile(MemFiles[0]->FileName);
	}
}
Buffer::~Buffer()
{
	this->CloseAllFile();
}
Page* File::GetFileFirstPage()
{
	return GetGlobalClock()->GetMemAddr(this->FilePointer, 0);  // 从Clock类中利用文件指针，获取物理文件的内存地址的第一页 首页 
}

Page* File::AddExtraPage()
{
	Clock *MemClock = GetGlobalClock();
	//获取文件首页,更新相关信息 
	Page* FirstPage = this->GetFileFirstPage();
	this->TotalPage = FirstPage->GetFileHead()->TotalPage + 1;  //获取第一页中页头部分的该文件的总页数+1，即为新文件的总页数
	FirstPage->GetFileHead()->TotalPage += 1; //页头中的信息也需要更新
	FirstPage->SetModified(true);
	FirstPage->SetLastUsed(true);

	//创建新内存页并且返回
	Page * NewPage = MemClock->CreateNewPage(this->FilePointer, FirstPage->GetFileHead()->TotalPage - 1);
	NewPage->SetModified(true);
	NewPage->SetLastUsed(true);

	return NewPage;
}

Page* Clock::CreateNewPage(FILE* FilePointer, unsigned long FilePageId)
{
	// 初始化新的内存页对象
	int index = GetUseablePage();   //找到可用页
	memset(MemPages[index]->PtrtoPageBeginAddress, 0, PageSize);  //设置可用页的数据存储区域数据为0
	MemPages[index]->FilePointer = FilePointer;
	MemPages[index]->FilePageID = FilePageId;
	MemPages[index]->SetModified(true);  // 新页设置为脏页，需要写回

	// 初始化新页的页头信息
	MemPages[index]->Pagehead->SetPageId(FilePageId); //设置页头中 页的id信息
	if (FilePageId != 0)
	{
		MemPages[index]->Pagehead->SetFixed(false);
	}
	else
	{
		MemPages[index]->Pagehead->SetFixed(true);     //如果新的页是一个文件的首页的话，会把他锁定在内存中，不会被释放写回
		MemPages[index]->GetFileHead()->Initialize();  //再把这个文件页的文件头的信息也进行初始化
	}
	return MemPages[index];

}

Clock::Clock()
{
	for (int i = 0; i <= MemPageCount; i++)
	{
		MemPages[i] = NULL;
	}
}

Clock::~Clock()
{
	for (int i = 0; i <= MemPageCount; i++)
	{
		if (MemPages[i] != nullptr)
			delete MemPages[i];
	}
}

void Page::SetModified(bool State) {
	this->IsModified = State;
}
void Page::SetLastUsed(bool State) {
	this->IsLastUsed = State;
}

void* File::ReadMem(FileAddress *ReadAddress) {  //读取内存中的文件，依据对应的文件地址类,并且返回一个指针，指向该文件在内存地址
	Page* MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, ReadAddress->FilePageID);
	MemPage->SetLastUsed(true);
	return (char*)MemPage->PtrtoPageBeginAddress + ReadAddress->OffSet;
}

FileAddress File::WriteMem(const void* Source, unsigned int Length, FileAddress* Dest) {           // 在Dest地址处写入数据，并且返回文件中某个地址变量
	Page* MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, Dest->FilePageID);             //返回的是真实文件地址的写入数据后的地址
	// 如果该页剩余空间不足
	if ((PageSize - Dest->OffSet) < Length)
	{
		return *Dest;
	}

	memcpy((void*)((char*)MemPage->PtrtoPageBeginAddress + Dest->OffSet), Source, Length);
	MemPage->SetLastUsed(true);
	MemPage->SetModified(true);

	//dest->offSet += length;
	FileAddress fd;
	fd.SetFileAddress(Dest->FilePageID, Dest->OffSet + Length);
	return fd;

}
// 在可写入地址写入数据，若空间不足则申请新的页, 返回数据写入的地址
FileAddress File::WriteMem(const void* Source, unsigned int Length)
{
	// 从该文件的文件头中获取该文件中可以写入地址
	FileAddress InsertPos = GetGlobalClock()->GetMemAddr(this->FilePointer, 0)->GetFileHead()->NewInsert;

	// 写入
	FileAddress write_res = WriteMem(Source, Length, &InsertPos);
	if (write_res.FilePageID == InsertPos.FilePageID && write_res.OffSet == InsertPos.OffSet)  //空间不足，需要开辟新的页
	{
		this->AddExtraPage();
		InsertPos.SetFileAddress(InsertPos.FilePageID + 1, PageHeadSize);
		write_res = WriteMem(Source, Length, &InsertPos);  // 重新写入
	}

	// 更新可写入位置
	GetGlobalClock()->GetMemAddr(this->FilePointer, 0)->GetFileHead()->NewInsert = write_res;
	GetGlobalClock()->GetMemAddr(this->FilePointer, 0)->SetModified(true);
	GetGlobalClock()->GetMemAddr(this->FilePointer, 0)->SetLastUsed(true);
	return InsertPos;
}
void FileAddress::SetFileAddress(unsigned long FilePageID, unsigned int  OffSet) {
	this->FilePageID = FilePageID;
	this->OffSet = OffSet;
}

//void File::WipeMem(void* Source, unsigned int WipeSize, FileAddress *WipePos)
//{
//	auto pMemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, WipePos->FilePageID);
//	// wipe
//	memcpy((char*)pMemPage->PtrtoPageBeginAddress + WipePos->OffSet, Source, WipeSize);
//	pMemPage->SetModified(true);
//	pMemPage->SetLastUsed(true);
//}

bool FileAddress::operator==(const FileAddress &rhs) const
{
	return (this->FilePageID == rhs.FilePageID && this->OffSet == rhs.OffSet);
}
bool FileAddress::operator!=(const FileAddress &rhs) const
{
	return !(this->FilePageID == rhs.FilePageID && this->OffSet == rhs.OffSet);
}
bool FileAddress::operator<(const FileAddress &rhs)const
{
	return (this->FilePageID < rhs.FilePageID) || ((this->FilePageID == rhs.FilePageID) && (this->OffSet < rhs.OffSet));
}
bool FileAddress::operator>(const FileAddress &rhs)const
{
	return (this->FilePageID > rhs.FilePageID) || ((this->FilePageID == rhs.FilePageID) && (this->OffSet > rhs.OffSet));
}


Page::Page()
{
	this->PtrtoPageBeginAddress = malloc(PageSize);
	this->Pagehead = (PageHead*)(this->PtrtoPageBeginAddress);
	this->FilePointer = NULL;
	this->SetModified(false);
	this->SetLastUsed(false);
}

Page::~Page()
{
	// 脏页且不是抛弃的页需要写回
	if (this->IsModified && this->FilePointer != NULL)
		this->WriteMemToFile();
	delete this->PtrtoPageBeginAddress;
}


const void* File::ReadRecord(FileAddress *RecordAddress)const {
	Page* MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, RecordAddress->FilePageID);    //在内存中通过文件指针和页号，找到要查询的记录的内存页的指针地址
	return (char*)(MemPage->PtrtoPageBeginAddress) + RecordAddress->OffSet;  //然后用此内存页中所记录的存数据的地址 + 页中数据偏移量  ==  某条记录的地址
}

//void* File::ReadWriteRecord(FileAddress *RecordAddress) {                   // 读取某条记录,返回记录指针(包括记录地址数据)
//	Page* MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, RecordAddress->FilePageID);
//	MemPage->SetModified(true);
//	return (char*)(MemPage->PtrtoPageBeginAddress) + RecordAddress->OffSet;
//}


// 返回新添加记录的地址quit
FileAddress File::AddRecord(const void* const Source, unsigned int SizeofRecord)
{
	Page* MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, 0);
	FileHead* Filehead = MemPage->GetFileHead();
	FileAddress fd; // AddRecord写入的位置
	void *Temp;
	if (Filehead->DelFirst.OffSet == 0 && Filehead->DelLast.OffSet == 0)
	{
		// 没有被删除过的空余空间，直接在文件尾插入数据
		// 将添加的新地址作为记录数据的一部分写入
		Temp = malloc(sizeof(FileAddress) + SizeofRecord);
		memcpy(Temp, &Filehead->NewInsert, sizeof(FileAddress));              //Temp == (记录着文件末尾可插入新数据的FileAddress)的地址 + 记录的数据
		memcpy((char*)Temp + sizeof(FileAddress), Source, SizeofRecord);
		FileAddress Pos = WriteMem(Temp, SizeofRecord + sizeof(FileAddress));  //将记录信息写入该内存文件中空余的部分,并且返回写入的物理文件中的地址信息
		WriteMem(&Pos, sizeof(FileAddress), &Pos);  //在Pos中把新地址也作为记录数据的一部分写入 ？
		fd = Pos;                //即返回了
	}
	else if (Filehead->DelFirst == Filehead->DelLast)
	{
		// 在第一个被删除的数据处，填加新数据
		Temp = malloc(SizeofRecord + sizeof(FileAddress));
		memcpy(Temp, &Filehead->DelFirst, sizeof(FileAddress));
		memcpy((char*)Temp + sizeof(FileAddress), Source, SizeofRecord);
		WriteMem(Temp, SizeofRecord + sizeof(FileAddress), &Filehead->DelFirst);
		fd = Filehead->DelFirst;
		Filehead->DelFirst.OffSet = 0;
		Filehead->DelLast.OffSet = 0;
	}
	else
	{
		FileAddress Pos = Filehead->DelFirst;
		fd = Filehead->DelFirst;
		Filehead->DelFirst = *(FileAddress*)ReadMem(&Filehead->DelFirst);

		Temp = malloc(SizeofRecord + sizeof(FileAddress));
		memcpy(Temp, &Pos, sizeof(FileAddress));
		memcpy((char*)Temp + sizeof(FileAddress), Source, SizeofRecord);
		WriteMem(Temp, SizeofRecord + sizeof(FileAddress), &Pos);  //将数据写入最近删除的记录处
	}
	delete Temp;
	MemPage->SetModified(true);
	return fd;
}

FileAddress File::DeleteRecord(FileAddress *DeleteAddress, unsigned int SizeofRecord)
{
	Page* MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, 0);
	FileHead* Filehead = MemPage->GetFileHead();

	// 如果待删除数据地址的地址标识和本身地址不等，则是已经删除过的数据 
	//返回的fd本来应该是返回的一个内存的地址
	FileAddress fd = *(FileAddress*)ReadMem(DeleteAddress);
	if (fd != *DeleteAddress)
	{
		FileAddress tmp;
		tmp.SetFileAddress(0, 0);
		return tmp;
	}
	else if (Filehead->DelFirst.OffSet == 0 && Filehead->DelLast.OffSet == 0)  // 之前没有删除过记录
	{
		Filehead->DelFirst = Filehead->DelLast = *DeleteAddress;
		FileAddress tmp;
		tmp.SetFileAddress(0, 0);
		WriteMem(&tmp, sizeof(FileAddress), &Filehead->DelLast);
	}
	else
	{
		// 删除记录
		WriteMem(DeleteAddress, sizeof(FileAddress), &Filehead->DelLast);
		Filehead->DelLast = *DeleteAddress;
		FileAddress tmp;
		tmp.SetFileAddress(0, 0);
		WriteMem(&tmp, sizeof(FileAddress), &Filehead->DelLast);
	}

	MemPage->SetModified(true);
	return *DeleteAddress;
}

bool File::UpdateRecord(FileAddress *address, void *UpdateRecordData, unsigned int SizeofRecord)
{
	auto MemPage = GetGlobalClock()->GetMemAddr(this->FilePointer, address->FilePageID);
	auto pdest = (char*)MemPage->PtrtoPageBeginAddress + address->OffSet + sizeof(FileAddress);
	memcpy(pdest, UpdateRecordData, SizeofRecord);
	MemPage->SetModified(true);
	return true;
}