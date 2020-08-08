#include"Interpreter.h"
std::string UseDbInfo(std::vector<std::string> sen_str)
{
	if ((sen_str.size() != 3)
		|| (tolower(sen_str[0]) != "use")
		|| (tolower(sen_str[1]) != "database")
		) {
		throw Error("Use DB command format error!");
	}
	return sen_str[2];
}

std::string ShowDbInfo(std::vector<std::string> sen_str)
{

	// show databases
	if ((sen_str.size() != 2)
		|| (tolower(sen_str[0]) != "show")
		|| (tolower(sen_str[1]) != "database")
		)
		throw Error("Show DB command format error!T");
	return std::string();
}

std::string CreateDbInfo(std::vector<std::string> sen_str)
{
	if ((sen_str.size() != 3)
		|| (tolower(sen_str[0]) != "create")
		|| (tolower(sen_str[1]) != "database")
		)
		throw Error("Create DB command format error!");
	return sen_str[2];
}

std::string DeleteDbInfo(std::vector<std::string> sen_str)
{
	if ((sen_str.size() != 3)
		|| (tolower(sen_str[0]) != "drop")
		|| (tolower(sen_str[1]) != "database")
		)
		throw Error("Delete DB command format error!");
	return sen_str[2];
}

void PrintWindow::CreateDB(bool is_created) {
	if (is_created)
		std::cout << "Create succeed!" << std::endl;
	else
		std::cout << "Create failed!" << std::endl;
}

void PrintWindow::DropDB(bool is_dropped) {
	if (is_dropped)
		std::cout << "Drop database succeed!" << std::endl;
	else
		std::cout << "Drop database failed!" << std::endl;
}
void PrintWindow::UseDB(bool isUsed) {
	if (isUsed) {
		std::cout << "databae changed!" << std::endl;
		std::cout << "Now Current Path Is In:" << GetCp().GetCurrentPath() << std::endl;
	}
	else
		std::cout << "database absent" << std::endl;
}

void PrintWindow::ShowDB(std::vector<std::string> db_names)
{
	std::cout << "There are all the databases !" << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	Print(PRINTLENGTH, "Database");
	std::cout << "*****************************************************************" << std::endl;
	for (auto e : db_names)
	{
		Print(PRINTLENGTH, e);
	}
	std::cout << "*****************************************************************" << std::endl;
	std::cout << db_names.size() << " rows in result." << std::endl;
}

void PrintWindow::Print(int len, std::string s)   //打印长度为len的字符串s 并且剩下的位置用空格补齐
{
	std::cout << "*";
	std::cout << s;
	for (int i = 0; i < (PRINTLENGTH - s.size()); i++) std::cout << " ";
	std::cout << "*";
	std::cout << std::endl;

}

int PrintWindow::GetColumnLength(std::string name, std::vector<std::string> col_name, std::vector<int> col_len)
{
	for (int j = 0; j < col_name.size(); j++)
	{
		if (name == col_name[j])
		{
			return col_len[j] > col_name[j].size() ? col_len[j] : col_name[j].size();
		}
	}
	return 0;
}

SensefulStr::SensefulStr() {
	src_str = "";
	Parse();
}

void SensefulStr::SetStr(string _srcstr) {
	src_str = _srcstr;
	//sen_str.clear();
	Parse();
}

void SensefulStr::Parse() {
	int i = 0;
	sen_str.clear();
	string token;
	while (i < src_str.size())
	{
		if (src_str[i] == 34 || src_str[i] == 39)// " '
		{
			token.clear();
			i++;
			while ((src_str[i] != 34) && (src_str[i] != 39))
			{
				token += src_str[i];
				i++;
			}
			i++;
			sen_str.push_back(token);
			token.clear();
			continue;
		}
		if (IsKeyChar(src_str[i]))
		{
			if (!token.empty())
				sen_str.push_back(token);
			token.clear();
			// 跳过关键字符，除了>=<比较符
			while (IsKeyChar(src_str[i]))
			{
				std::string tmp_token;
				if (src_str[i] == '>' || src_str[i] == '=' || src_str[i] == '<')  // 比较符号
				{
					tmp_token += src_str[i];
					if (src_str[i + 1] == '=')
					{
						tmp_token += src_str[i + 1];
						i += 2;
					}
					else
					{
						i++;
					}
					sen_str.push_back(tmp_token);
				}
				else
				{
					i++;
				}
			}

		}
		else
		{
			token += src_str[i];
			i++;
		}
	}
	//for (int i = 0; i < sen_str.size(); i++) {
	//	cout << sen_str[i] << endl;
	//}
}
vector<string> SensefulStr::GetSensefulStr() const
{
	return sen_str;
}

bool SensefulStr::IsKeyChar(char c)
{
	auto it = find(key_char.begin(), key_char.end(), c);

	return (it != key_char.end());
}

CmdType GetOpType(vector<string> sen_str)
{
	for (auto&e : sen_str)
		tolower(e);
	if (sen_str.size() == 0) {
		throw Error("No Command Input");
	}
	if (sen_str[0] == "create"&&sen_str[1] == "table")
	{
		return CmdType::TABLE_CREATE;
	}

	if (sen_str[0] == "create"&&sen_str[1] == "index")
	{
		return CmdType::INDEX_CREATE;
	}

	if (sen_str[0] == "drop"&&sen_str[1] == "table")
	{
		return CmdType::TABLE_DROP;
	}
	if (sen_str[0] == "drop"&&sen_str[1] == "index")
	{
		return CmdType::INDEX_DROP;
	}


	if (sen_str[0] == "create"&&sen_str[1] == "database")
	{
		return CmdType::DB_CREATE;
	}


	if (sen_str[0] == "drop"&&sen_str[1] == "database")
	{
		return CmdType::DB_DROP;
	}

	if (sen_str[0] == "show"&&sen_str[1] == "tables")
	{
		return CmdType::TABLE_SHOW;
	}

	if (sen_str[0] == "show"&&sen_str[1] == "database")
	{
		return CmdType::DB_SHOW;
	}

	if (sen_str[0] == "use")
	{
		return CmdType::DB_USE;
	}

	if (sen_str[0] == "select")
	{
		return CmdType::TABLE_SELECT;
	}

	if (sen_str[0] == "insert")
	{
		return CmdType::TABLE_INSERT;
	}

	if (sen_str[0] == "update")
	{
		return CmdType::TABLE_UPDATE;
	}

	if (sen_str[0] == "delete")
	{
		return CmdType::TABLE_DELETE;
	}

	if (sen_str[0] == "select")
	{
		return CmdType::TABLE_SELECT;
	}

	if (sen_str[0] == "quit")
	{
		return CmdType::QUIT;
	}
	if (sen_str[0] == "help")
	{
		return CmdType::HELP;
	}
	if (sen_str[0] == "execfile")
	{
		return CmdType::FILE;
	}
	throw Error("Comand is Not Supported!");

}

void Interpreter(vector<string> sen_str, CmdType cmd_type, PrintWindow print_window) {

	CatalogPosition &cp = GetCp();
	//TB_Select_Info tb_select_info;
	vector<FileAddress> fds;
	switch (cmd_type)
	{
	case CmdType::DB_CREATE:         // 创建数据库                          // 创建数据库  先调用在interpreter里的函数CreateDbInfo来进行语法判断并且返回要创建的数据库名称，
																			//然后再调用api里的Create_DB真正的执行操作，返回一个操作结果bool
							 // api        interpreter											//最后再调用print_window的CreateDB 函数通过返回的bool值判断命令是否执行成功	
		print_window.CreateDB(Create_DB(CreateDbInfo(sen_str), cp));
		break;

	case CmdType::DB_DROP:           // 删除数据库
		print_window.DropDB(Drop_DB(DeleteDbInfo(sen_str), cp));
		break;

	case CmdType::DB_SHOW:           // 列出所有数据库
		print_window.ShowDB(Show_DB(cp));
		break;

	case CmdType::DB_USE:            // 使用数据库
		print_window.UseDB(Use_DB(UseDbInfo(sen_str), cp));
		break;
	case CmdType::TABLE_CREATE:      // 创建表                              // 创建表  先调用在interpreter里的函数CreateTableInfo来将有意字符串里的信息变成一个表的结构体
																			//然后再调用GetCurrentPath()获取当前在哪个数据库内
																		    //再调用api里的Create_Table创建表，返回创建结果
																			//最后调用print_window的CreateTable 函数通过返回的bool值判断命令是否执行成功	
		print_window.CreateTable(Create_Table(CreateTableInfo(sen_str), cp.GetCurrentPath()));  
		break;

	case CmdType::TABLE_DROP:        // 删除表
		print_window.DropTable(Drop_Table(DropTableInfo(sen_str), cp.GetCurrentPath()));
		break;
	case CmdType::TABLE_SHOW:        // 列出当前数据库下所有表
		print_window.ShowAllTable(sen_str, cp.GetCurrentPath());
		break;

	case CmdType::TABLE_INSERT:      // 插入新的记录
		print_window.InsertRecord(Insert_Record(CreateInsertInfo(sen_str), cp.GetCurrentPath())); 

		                                                                    //先调用在interpreter里的函数CreateInsertInfo来将有意字符串里的信息变成一个插入记录的结构体
																			//然后再调用GetCurrentPath()获取当前在哪个数据库内
																		    //再调用api里的Insert_Record插入记录，返回创建结果
																			//最后调用print_window的InsertRecord 函数通过返回的bool值判断命令是否执行成功	
		break;

	case CmdType::INDEX_CREATE:      // 创建索引
		print_window.CreateIndex(Create_Index(CreateIndexInfo(sen_str), cp.GetCurrentPath()));
		break;

	case CmdType::INDEX_DROP:        // 删除索引
		print_window.DropIndex(Drop_Index(DropIndexInfo(sen_str), cp.GetCurrentPath()));
		break;


	case CmdType::TABLE_SELECT:      // 选择表的特定记录
		print_window.SelectTable(Select_Record(TableSelectInfo(sen_str), cp.GetCurrentPath()));
		GetGlobalTimer().PrintTime();
		break;


	case CmdType::TABLE_UPDATE:      // 更新表的记录
		print_window.UpdateTable(Update_Record(TableUpdateInfo(sen_str), cp.GetCurrentPath()));
		break;

	case CmdType::TABLE_DELETE:      // 删除表的记录
		print_window.DeleteTable(Delete_Record(TableDeleteInfo(sen_str), cp.GetCurrentPath()));
		break;

	default:
		throw Error("CmdType Error");
		break;
	}

}


/***********************************************************************************************
 ***                                   表操作相关函数                                        ***
 ***********************************************************************************************                                                                        
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
		                                                                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**********************************创建表*************************************************************/
void PrintWindow::CreateTable(bool is_created) {
	if (is_created)
		std::cout << "table create succeed!" << std::endl;
	else
		std::cout << "table create failed!" << std::endl;
}

TB_Create_Info CreateTableInfo(std::vector<std::string> sen_str)
{
	TB_Create_Info tb_create_info;
	if (sen_str.size() < 3 || tolower(sen_str[0]) != "create" || tolower(sen_str[1]) != "table") {
		//std::cout << "Command format error!The command is not exist!" << std::endl;
		throw Error("The command is not exist!");
	}
// 表名
	tb_create_info.table_name = sen_str[2];
	bool HasPrimary = false;
	// 添加各个字段 
	for (int j = 3; j < sen_str.size();)   //sen_str 格式如下： create table 'name' 类型1 名字1 ……
	{
		ColumnInfo column_info;
		column_info.isPrimary = false;
		column_info.isUnique = false;
		// 列名
		column_info.name = sen_str[j];
		// 列类型
		if (j + 1 >= sen_str.size()) {
			//std::cout << "Command format error!The command is not exist!" << std::endl;
			throw Error("The command is not exist!");
		}

		if (tolower(sen_str[j + 1]) == "int")    // int num
		{
			column_info.type = Column_Type::I;
			column_info.RequiredLength = sizeof(int);
			j += 2;
		}
		else if (tolower(sen_str[j + 1]) == "float")
		{
			column_info.type = Column_Type::F;
			column_info.RequiredLength = sizeof(double);
			j += 2;
		}
		else if (tolower(sen_str[j + 1]) == "char")
		{
			column_info.type = Column_Type::C;
			if (j + 2 >= sen_str.size()) { 
				throw Error("The command is not exist!");
			}
			column_info.RequiredLength = stoi(sen_str[j + 2]);   //因为其输入格式为 char name(10)  
			//stoi函数：把数字字符串转换成int输出
			if (column_info.RequiredLength >= 255 || column_info.RequiredLength <= 0) {
				throw Error("The length of char is not supported!");
			}
			j += 3;
		}
		else
		{
			throw Error("Unsupported data types!");
		}

		// 是否Unique
		if (j < sen_str.size() && (sen_str[j] == "unique"))
		{
				column_info.isUnique = true;
				j++;
		}
		tb_create_info.columns_info.push_back(column_info);
		// 是否主键
		if (j < sen_str.size() && (sen_str[j] == "primary" )&& (sen_str[j+1] == "key"))
		{
			//cout << sen_str[j] << sen_str[j + 1] << sen_str[j + 2] << endl;
			if (HasPrimary)
				throw Error("More than one primary key!");

			HasPrimary = true;
			for (int i = 0; i < tb_create_info.columns_info.size(); i++)
			{
				if (tb_create_info.columns_info[i].name == sen_str[j+2])
				{
					tb_create_info.columns_info[i].isPrimary = true;
					
				}
			}
			j+=3;
			//column_info.isPrimary = true;
		}
	}
	if (!HasPrimary)
		tb_create_info.columns_info[0].isPrimary = true;  // 默认输入的第一个字段为主键

	return tb_create_info;          
}

/**********************************显示所有表*************************************************************/
void PrintWindow::ShowAllTable(vector<string> sen_str, string path)
{
	std::cout << "There are all the tables !" << std::endl;
	std::cout << "+***************************************************************+" << std::endl;

	if (!GetCp().IsInSpeDb() || sen_str.size() < 2)
	{
		//std::cout << "Not use database or Command format error" << std::endl;
		throw Error("Not use database");
	}

	std::vector<std::string> tables;

	_finddata_t FileInfo;
	path += "*.*";
	decltype(_findfirst(path.c_str(), &FileInfo)) k;
	decltype(_findfirst(path.c_str(), &FileInfo)) HANDLE;
	k = HANDLE = _findfirst(path.c_str(), &FileInfo);

	while (k != -1)
	{
		// 如果是普通文件夹则输出
		if (!(FileInfo.attrib & _A_SUBDIR) && strcmp(FileInfo.name, ".") != 0 && strcmp(FileInfo.name, "..") != 0)
		{
			// 只检查后缀.dbs的文件  dbs
			std::string tmp_file(FileInfo.name);
			int index = tmp_file.size() - 1;
			if (tmp_file.size() < 4 || tmp_file[index] != 's' || tmp_file[index - 1] != 'b' || tmp_file[index - 2] != 'd' || tmp_file[index - 3] != '.')
			{
				;
			}
			else
			{
				tables.push_back(std::string(tmp_file.begin(), tmp_file.begin() + tmp_file.size() - 4));
			}
		}

		k = _findnext(HANDLE, &FileInfo);
	}
	_findclose(HANDLE);
	// 排序
	std::sort(tables.begin(), tables.end());
	for (auto e : tables)
		Print(PRINTLENGTH, e);
	std::cout << "+***************************************************************+" << std::endl;
	std::cout << tables.size() << " rows in result." << std::endl;
}

/**********************************删除某张表*************************************************************/

void PrintWindow::DropTable(bool is_dropped) {
	if (is_dropped)
		std::cout << "Drop table succeed!" << std::endl;
	else
		std::cout << "Drop table failed!" << std::endl;
}
std::string DropTableInfo(std::vector<std::string> sen_str)
{
	if ((sen_str.size() < 3)
		|| (tolower(sen_str[0]) != "drop")
		|| (tolower(sen_str[1]) != "table")
		)
		throw Error("The command is not exist!");
	return sen_str[2];
}

/**********************************向表中添加记录*************************************************************/
TB_Insert_Info CreateInsertInfo(std::vector<std::string> sen_str)
{
	TB_Insert_Info tb_insert_info;

	if (sen_str.size() < 3 || tolower(sen_str[0]) != "insert" || tolower(sen_str[1]) != "into") {
		//std::cout << "format error!The command is not exist! " << std::endl;
		throw Error("The command is not exist!");
	}

	int values_index = -1;
	for (int i = 0; i < sen_str.size(); i++)
	{
		if (tolower(sen_str[i]) == "values")
		{
			values_index = i;
			break;
		}
	}
	if (values_index <= 0) {
		//std::cout << "format error!The command is not exist! " << std::endl;
		throw Error("The command is not exist!");
	}

// 读取表名
	tb_insert_info.table_name = sen_str[2];

	// 读取字段
	if (values_index == 3) {
		tb_insert_info.IsOrder = false;
		int i;
		for (i = values_index + 1; i < sen_str.size();i++)
		{
			tb_insert_info.insert_info.push_back({ "",sen_str[i] });
		}
	}
	else {
		tb_insert_info.IsOrder = true;
		int p, q;
		for (p = 3, q = values_index + 1; p < values_index && q < sen_str.size(); p++, q++)
		{
			tb_insert_info.insert_info.push_back({ sen_str[p],sen_str[q] });
		}
		if ((p - 3) != (sen_str.size() - 1 - values_index)) {
			//std::cout << "The size of fields is not match the size of values! " << std::endl;
			throw Error("The size of fields is not match the size of values!");
		}
	}
	return tb_insert_info;
}
void PrintWindow::InsertRecord(bool is_inserted) {
	if (is_inserted)
		std::cout << "Insert succeed!" << std::endl;
	else
		std::cout << "Insert failed!" << std::endl;
}
/**********************************查找表中记录*************************************************************/
	TB_Select_Info TableSelectInfo(std::vector<std::string> sen_str)  //select xx&xx&xx from ? where (filed op value)
	{
		TB_Select_Info tb_select_info;
		// 选择的字段名称
		if (tolower(sen_str[0]) != "select") {
			//std::cout << "format error!The command is not exist! " << std::endl;
			throw Error("The command is not exist!");
		}
		int name_L_index = 1;
		int name_R_index = 0;   //记录其要选择的是哪些列 
		for (int i = 0; i < sen_str.size(); i++)
		{
			if (tolower(sen_str[i]) == "from")
			{
				name_R_index = i - 1;
				break;
			}
		}
		if (!name_R_index) {
			//std::cout << "format error!The command is not exist! " << std::endl;
			throw Error("The command is not exist!");
		}

		for (int i = name_L_index; i <= name_R_index; i++)
		{
			tb_select_info.name_select_column.push_back(sen_str[i]);
		}
	//至此name_select_column中有了要选择的所有列的信息

		if (sen_str.size() - 1 < (name_R_index + 2)) {             
			//std::cout << "format error!The command is not exist! " << std::endl;
			throw Error("The command is not exist!");
		}

		tb_select_info.table_name = sen_str[name_R_index + 2];  //表名即为 列信息结束后的两个 因为 select * from tablename
	
		int name_where_index = name_R_index + 3;        //name_where_index 标识 where这个关键字所在的index为多少     

		if (sen_str.size() - 1 < name_where_index)   //如果选择语句是没有where的，那么直接返回，非常方便
			return tb_select_info;
	
		std::vector<std::pair<std::string, Column_Type>> mpair = GetColumn_Name_Type(tb_select_info.table_name, GetCp().GetCurrentPath());
		//获得了这张表的所有的字段名称和字段类型，GetColumn_Name_Type函数是通过表名去index文件里找到的

		// 打包查找条件
		for (int i = name_where_index + 1; i < sen_str.size();)  //从where字段向后开始遍历查找条件
		{
			if (tolower(sen_str[i]) == ";")
				break;
			Cell_Compare cmp_cell = CreateCmpCell(sen_str[i], GetType(sen_str[i], mpair), GetOperatorType(sen_str[i + 1]), sen_str[i + 2]);  
			//通过column_name, column_type, Optype, value 这四个值，生成一个字段的比较单元
			//GetType 即从之前找到的所有字段名称和类型pair中进行查找，找到该字段的字段类型，通过GetOperatorType来将字符比较转化为运算符信息 如将 < 转换为 L

			tb_select_info.vec_cmp_cell.push_back(cmp_cell);  //全部推进tb_select_info 这个vector中，这个vector里面全部都是查找条件
	
			// 下一个查找条件
			if ((i + 3) < sen_str.size() && tolower(sen_str[i + 3]) == "and")   //由于此处要求只考虑and，所以暂时不考虑or的情况，只支持and
			{
				i += 4;
			}
			else
			{
				break;
			}
	
		}
		return tb_select_info;               //最终返回一个tb_select_info，内含： std::string table_name; //选择的表名
																				//std::vector <std::string> name_select_column;//选择的字段名字
																				//std::vector<Cell_Compare> vec_cmp_cell;//选择条件

	}
Cell_Compare CreateCmpCell(std::string column_name, Column_Type column_type, Operator_Type Optype, std::string value)
	{

		Column_Cell column_cell;
		column_cell.column_name = column_name;
		Column_Type tmp = column_type;
		char*pChar = nullptr;
		switch (tmp)
		{
		case Column_Type::I:
			column_cell.column_type = Value_Type::I;
			column_cell.column_value.IntValue = stoi(value);
			break;

		case Column_Type::C:
			column_cell.column_type = Value_Type::C;
			pChar = (char*)malloc(value.size() + 1);
			strcpy(pChar, value.c_str());
			column_cell.column_value.CharValue = pChar;
			break;

		case Column_Type::F:
			column_cell.column_type = Value_Type::F;
			column_cell.column_value.FloatValue = stod(value);
			break;
		default:
			break;
		}
		Cell_Compare cmp_cell(Optype, column_cell);

		return cmp_cell;
	}
void PrintWindow::SelectTable(SelectPrintInfo select_table_print_info)
{  
	for (auto it = select_table_print_info.name_selected_column.begin(); it != select_table_print_info.name_selected_column.end();)
	{
		if (*it == ",")
		{
			select_table_print_info.name_selected_column.erase(it);
		}
		else
		{
			it++;
		}
	}
	if (select_table_print_info.key_fd.size() < 1) {
		cout << "No Relevant Records" << endl;
		return;
	}
	std::string idx_file = GetCp().GetCurrentPath() + select_table_print_info.table_name + ".index";
	std::string dbf_file = GetCp().GetCurrentPath() + select_table_print_info.table_name + ".dbs";
	BTree tree(idx_file);
	TableIndexHeadInfo table_index_head_info(tree);
	int total_length = 0;
	auto key_fd = select_table_print_info.key_fd;

	auto all_column_name = table_index_head_info.GetColumnNames();
	auto all_column_len = table_index_head_info.GetColumnSize();
	std::vector<std::string> out_name = select_table_print_info.name_selected_column;
	if (out_name.size() == 1 && out_name[0] == "*")
		out_name = all_column_name;
	int n_output_col = out_name.size();

	int total_record_len = 2;
	std::vector<int> out_len;
	for (int i = 0; i < out_name.size(); i++)
	{
		auto it = find(all_column_name.begin(), all_column_name.end(), out_name[i]);
		Column_Type type = table_index_head_info.GetColumnType()[it - all_column_name.begin()];
		switch (type)
		{
		case Column_Type::I:
			out_len.push_back(8);
			total_record_len += 8;
			break;
		case Column_Type::C:
			out_len.push_back(all_column_len[it - all_column_name.begin()]);
			total_record_len += all_column_len[it - all_column_name.begin()];
			break;
		case Column_Type::F:
			out_len.push_back(15);
			total_record_len += 15;
			break;
		default:
			break;
		}
	}

	total_record_len += (out_name.size() - 1);
	//打印头部
	std::cout << "+";
	for (int i = 0; i < out_name.size(); i++)
	{
		int len = out_len[i] + 1;
		if (i == out_name.size() - 1)len--;
		for (int j = 0; j < len; j++)
		{
			std::cout << "-";
		}

	}
	std::cout << "+";


	std::cout << std::endl;
	// 打印列名
	for (int i = 0; i < out_name.size(); i++)
	{
		std::cout << "|" << std::left << std::setw(out_len[i]) << out_name[i];
	}
	std::cout << "|" << std::endl;
	// 分割线
	std::cout << "+";
	for (int i = 0; i < total_record_len - 2; i++)std::cout << "-";
	std::cout << "+" << std::endl;

	// 打印每一条记录
	for (int i = 0; i < key_fd.size(); i++)
	{
		cout.precision(14);
		Column_Head record_head = Get_DB_Record(select_table_print_info.table_name, key_fd[i].second, GetCp().GetCurrentPath());
		auto pColumn = record_head.firstCell();
		while (pColumn)
		{
			auto it = find(out_name.begin(), out_name.end(), pColumn->column_name);
			if (it != out_name.end())
			{
				switch (pColumn->column_type)
				{
				case Value_Type::I:
					std::cout << "|" << std::left << std::setw(out_len[it - out_name.begin()]) << pColumn->column_value.IntValue;
					break;
				case Value_Type::F:

					std::cout << "|" << std::left << std::setw(out_len[it - out_name.begin()]) << pColumn->column_value.FloatValue;
					break;
				case Value_Type::C:

					std::cout << "|" << std::left << std::setw(out_len[it - out_name.begin()]) << pColumn->column_value.CharValue;
					break;
				default:
					break;
				}
			}
			pColumn = pColumn->next;
		}
		std::cout << "|" << std::endl;
	}



	// 输出最后一行
	std::cout << "+";
	for (int i = 0; i < total_record_len - 2; i++)std::cout << "-";
	std::cout << "+";
	std::cout << std::endl;

	std::cout << select_table_print_info.key_fd.size() << " row in set"<< std::endl;
}

/**********************************将表中记录删除*************************************************************/
void PrintWindow::DeleteTable(bool isDeleted) {
	if (isDeleted)
		std::cout << "Delete succeed!" << std::endl;
	else
		std::cout << "Delete failed" << std::endl;
}
TB_Delete_Info TableDeleteInfo(std::vector<std::string> sen_str)
{
	TB_Delete_Info tb_delete_info;
	tb_delete_info.table_name = sen_str[2];

	for (int i = 4; i < sen_str.size(); )
	{
		if (sen_str[i] == ";")
			break;
		Expr expr;
		expr.field = sen_str[i];
		expr.op = sen_str[i + 1];
		expr.value = sen_str[i + 2];
		tb_delete_info.expr.push_back(expr);
		i += 4;
	}
	return tb_delete_info;
}

/**********************************将表中记录更新************************************************************/
void PrintWindow::UpdateTable(bool isUpdated) {
	if (isUpdated)
		std::cout << "Update succeed!" << std::endl;
	else
		std::cout << "Update failed" << std::endl;
}

TB_Update_Info TableUpdateInfo(std::vector<std::string> sen_str)
{
	// TODO 语法检擦

	TB_Update_Info tb_update_info;
	int UPDATE = 0;
	int SET = 2;
	int WHERE = 0;
	for (int i = 0; i < sen_str.size(); i++)
	{
		if (tolower(sen_str[i]) == "where")
		{
			WHERE = i;
			break;
		}
	}

	// 新的字段值
	for (int j = SET + 1; j < WHERE;)
	{
		NewValue new_value;

		new_value.field = sen_str[j];
		new_value.value = sen_str[j + 2];
		tb_update_info.field_value.push_back(new_value);

		if (sen_str[j + 3] == ",")
			j += 4;
		else
			j += 3;
	}

	// 需要更新的字段条件
	for (int j = WHERE + 1; j < sen_str.size();)
	{
		Expr expr;
		expr.field = sen_str[j];
		expr.op = sen_str[j + 1];
		expr.value = sen_str[j + 2];
		tb_update_info.expr.push_back(expr);
		j += 4;
	}
	tb_update_info.table_name = sen_str[UPDATE + 1];
	return tb_update_info;
}

/***********************************************************************************************
 ***                                  索引操作相关函数                                        ***
 ***********************************************************************************************
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
																					   *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 /**********************************创建索引*************************************************************/

void PrintWindow::CreateIndex(bool is_created) {
	if (is_created)
		std::cout << "index create succeed!" << std::endl;
	else
		std::cout << "index create failed!" << std::endl;
}
Idx_Create_Info CreateIndexInfo(std::vector<std::string> sen_str)
{
	Idx_Create_Info idx_create_info;
	if (sen_str.size() < 3 || tolower(sen_str[0]) != "create" || tolower(sen_str[1]) != "index" || tolower(sen_str[3]!="on")) {
		//std::cout << "Command format error!The command is not exist!" << std::endl;
		throw Error("The command is not exist!");
	}
	
	idx_create_info.index_name = sen_str[2];//索引名
	idx_create_info.table_name = sen_str[4];//表名
	idx_create_info.column_name = sen_str[5];//列名

	return idx_create_info;
}
/**********************************删除索引*************************************************************/
void PrintWindow::DropIndex(bool is_dropped) {
	if (is_dropped)
		std::cout << "index drop succeed!" << std::endl;
	else
		std::cout << "index drop failed!" << std::endl;
}

std::string DropIndexInfo(std::vector<std::string> sen_str) {
	if ((sen_str.size() < 3)
		|| (tolower(sen_str[0]) != "drop")
		|| (tolower(sen_str[1]) != "index")
		)
		throw Error("The command is not exist!");
	return sen_str[2];
}