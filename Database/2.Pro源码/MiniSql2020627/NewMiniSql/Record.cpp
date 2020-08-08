#include "Record.h"

Value_Type ConvertStringToValueType(std::string str)
{
	if (str == "Int")
	{
		return Value_Type::I;
	}
	else if (str == "Float")
	{
		return Value_Type::F;
	}
	else if (str == "Char")
	{
		return Value_Type::C;
	}
	else
	{
		cout << "Not an accepted Value Type!" << endl;
		return Value_Type::ERROR;
	}

}

std::ostream& operator<<(std::ostream& os, const Key_Attr& key)
{
	switch (key.type)
	{
	case Value_Type::I:
		os << key.value.IntValue << " ";
		break;

	case Value_Type::F:
		os << key.value.FloatValue << " ";
		break;

	case Value_Type::C:
		os << key.value.CharValue << " ";
		break;

	default:
		break;
	}
	return os;
}

Key_Attr::Key_Attr()
{
	type = Value_Type::ERROR;
	memset(&value, 0, sizeof(value));
}

Key_Attr::Key_Attr(const Key_Attr& k)
{
	type = k.type;
	switch (type)
	{
	case Value_Type::I:
		value.IntValue = k.value.IntValue;
		break;

	case Value_Type::F:
		value.FloatValue = k.value.FloatValue;
		break;

	case Value_Type::C:
		strcpy(value.CharValue, k.value.CharValue);
		break;

	default:
		break;
	}
}
Key_Attr& Key_Attr::operator=(const Key_Attr& k)
{
	type = k.type;
	switch (type)
	{
	case Value_Type::I:
		value.IntValue = k.value.IntValue;
		break;

	case Value_Type::F:
		value.FloatValue = k.value.FloatValue;
		break;

	case Value_Type::C:
		strcpy(value.CharValue, k.value.CharValue);
		break;

	default:
		break;
	}
	return *this;
}

bool Key_Attr::operator<(const Key_Attr& k)const
{
	if (this->type != k.type)
	{
		cout << "Attres of different types cannot be compared!" << endl;

		return false;
	}

	std::string s1;
	std::string s2;

	switch (this->type)
	{
	case Value_Type::I:
		return this->value.IntValue < k.value.IntValue;
		break;

	case Value_Type::F:
		return this->value.FloatValue < k.value.FloatValue;
		break;

	case Value_Type::C:
		s1 = std::string(this->value.CharValue);
		s2 = std::string(k.value.CharValue);
		return s1 < s2;
		break;

	default:
		break;
	}
	return true;
}

bool Key_Attr::operator>(const Key_Attr& k)const
{
	if (this->type != k.type)
	{
		cout << "Attres of different types cannot be compared!" << endl;
		return false;
	}

	std::string s1;
	std::string s2;

	switch (this->type)
	{
	case Value_Type::I:
		return this->value.IntValue > k.value.IntValue;
		break;

	case Value_Type::F:
		return this->value.FloatValue > k.value.FloatValue;
		break;

	case Value_Type::C:
		s1 = std::string(this->value.CharValue);
		s2 = std::string(k.value.CharValue);
		return s1 > s2;
		break;

	default:
		break;
	}
	return true;
}

bool Key_Attr::operator<=(const Key_Attr& k)const
{
	if (this->type != k.type)
	{
		cout << "Attres of different types cannot be compared!" << endl;
		return false;
	}

	std::string s1;
	std::string s2;

	switch (this->type)
	{
	case Value_Type::I:
		return this->value.IntValue <= k.value.IntValue;
		break;

	case Value_Type::F:
		return this->value.FloatValue <= k.value.FloatValue;
		break;

	case Value_Type::C:
		s1 = std::string(this->value.CharValue);
		s2 = std::string(k.value.CharValue);
		return s1 <= s2;
		break;

	default:
		break;
	}
	return true;
}

bool Key_Attr::operator>=(const Key_Attr& k)const
{
	if (this->type != k.type)
	{
		cout << "Attres of different types cannot be compared!" << endl;
		return false;
	}

	std::string s1;
	std::string s2;

	switch (this->type)
	{
	case Value_Type::I:
		return this->value.IntValue >= k.value.IntValue;
		break;

	case Value_Type::F:
		return this->value.FloatValue >= k.value.FloatValue;
		break;

	case Value_Type::C:
		s1 = std::string(this->value.CharValue);
		s2 = std::string(k.value.CharValue);
		return s1 >= s2;
		break;

	default:
		break;
	}
	return true;
}

bool Key_Attr::operator==(const Key_Attr& k)const
{
	if (this->type != k.type)
	{
		cout << "Attres of different types cannot be compared!" << endl;
		return false;
	}

	std::string s1;
	std::string s2;

	switch (this->type)
	{
	case Value_Type::I:
		return this->value.IntValue == k.value.IntValue;
		break;

	case Value_Type::F:
		return this->value.FloatValue == k.value.FloatValue;
		break;

	case Value_Type::C:
		s1 = std::string(this->value.CharValue);
		s2 = std::string(k.value.CharValue);
		return s1 == s2;
		break;

	default:
		break;
	}
	return true;
}

bool Key_Attr::operator!=(const Key_Attr& k)const
{
	if (this->type != k.type)
		return false;

	std::string s1;
	std::string s2;

	switch (this->type)
	{
	case Value_Type::I:
		return this->value.IntValue != k.value.IntValue;
		break;

	case Value_Type::F:
		return this->value.FloatValue != k.value.FloatValue;
		break;

	case Value_Type::C:
		s1 = std::string(this->value.CharValue);
		s2 = std::string(k.value.CharValue);
		return s1 != s2;
		break;

	default:
		break;
	}
	return false;
}

void* Column_Cell::data() const
{
	switch (column_type)
	{
	case Value_Type::I:
		return (void*)&column_value.IntValue;
		break;
	case Value_Type::F:
		return (void*)&column_value.FloatValue;
		break;
	case Value_Type::C:
		return (void*)column_value.CharValue;
		break;
	default:
		break;
	}
	return NULL;
}

Column_Cell::Column_Cell()
{
	column_name = "UNNAMED";
	column_type = Value_Type::ERROR;
	memset(&column_value, 0, sizeof(column_value));
	next = NULL;
	sz = 0;
}

Column_Cell::Column_Cell(Key_Attr k)
{
	column_name = "UNNAMED";
	column_type = k.type;
	next = NULL;
	switch (column_type)
	{
	case Value_Type::I:
		column_value.IntValue = k.value.IntValue;
		sz = sizeof(int);
		break;

	case Value_Type::F:
		column_value.FloatValue = k.value.FloatValue;
		sz = sizeof(double);
		break;
	case Value_Type::C:
		column_value.CharValue = (char*)malloc(strlen(k.value.CharValue) + 1);
		strcpy(column_value.CharValue, k.value.CharValue);
		sz = strlen(column_value.CharValue);
		break;

	default:
		break;
	}
}

Column_Cell::Column_Cell(const Column_Cell& c)
{
	column_name = c.column_name;
	column_type = c.column_type;
	next = NULL;
	sz = c.sz;
	switch (column_type)
	{
	case Value_Type::I:
		column_value.IntValue = c.column_value.IntValue;
		break;

	case Value_Type::F:
		column_value.FloatValue = c.column_value.FloatValue;
		break;

	case Value_Type::C:
		column_value.CharValue = (char*)malloc(strlen(c.column_value.CharValue));
		strcpy(column_value.CharValue, c.column_value.CharValue);
		break;

	default:
		break;
	}
}

Column_Cell& Column_Cell::operator=(const Column_Cell& c)
{
	column_name = c.column_name;
	column_type = c.column_type;
	next = NULL;
	sz = c.sz;
	switch (column_type)
	{
	case Value_Type::I:
		column_value.IntValue = c.column_value.IntValue;
		break;

	case Value_Type::F:
		column_value.FloatValue = c.column_value.FloatValue;
		break;

	case Value_Type::C:
		column_value.CharValue = (char*)malloc(strlen(c.column_value.CharValue) + 1);
		strcpy(column_value.CharValue, c.column_value.CharValue);
		break;

	default:
		break;
	}
	return *this;
}

Column_Cell::operator Key_Attr()const
{
	Key_Attr k;
	k.type = column_type;
	//memset(&key_attr, 0, sizeof(KeyAttr));
	switch (column_type)
	{
	case Value_Type::I:
		k.value.IntValue = column_value.IntValue;
		break;

	case Value_Type::F:
		k.value.FloatValue = column_value.FloatValue;
		break;

	case Value_Type::C:
		strcpy(k.value.CharValue,  column_value.CharValue);
		break;

	default:
		break;
	}
	return k;
}

Column_Head::Column_Head()
{
	front = rear = NULL;
}

Column_Head::Column_Head(Column_Cell c)
{
	c.next = NULL;
	front = rear = &c;
}

Column_Head::Column_Head(const Column_Head& h)
{
	front = h.front;
	rear = h.rear;
}

Column_Head& Column_Head::operator=(const Column_Head& h)
{
	front = h.front;
	rear = h.rear;
	return *this;
}

void Column_Head::attachCell(Column_Cell& c)
{
	if (!front)
	{
		front = new Column_Cell;
		*front = c;       //拷贝赋值
		front->next = NULL;
		rear = front;
	}
	else
	{
		rear->next = new Column_Cell;
		*(rear->next) = c;
		rear = rear->next;
		rear->next = NULL;
	}
}

size_t Column_Head::size()
{
	size_t sz = 0;
	Column_Cell* pt = front;
	while (pt)
	{
		sz += pt->size();
		pt = pt->next;
	}
	return sz;
}

Column_Cell* Column_Head::firstCell()
{
	return front;
}

std::ostream& operator<<(std::ostream& os, Column_Head& h)
{
	Column_Cell* pt = h.firstCell();
	while (pt)
	{
		os << Key_Attr(*pt);
		pt = pt->next;
	}
	return os;
}

std::pair<unsigned long, char*> Record::GetRecordData(Column_Head& h)
{
	//cout << h << endl;
	unsigned long data_size = h.size();
	char* data = new char[data_size];
	memset(data, 0, data_size);
	Column_Cell* p = h.firstCell();

	unsigned long offset = 0;
	while (p)
	{
		memcpy(data + offset, p->data(), p->size());
		offset += p->size();
		//for (int i = 0; i < offset; i++)
		//	printf("%02X ", data[i]);
		//printf("\n");
		p = p->next;
	}
	if (offset != data_size)
	{
		cout << "ERROR OCCURS (-1)" << endl;
	}
	//解决返回多条记录的问题
	pair<unsigned long, char*> sp(data_size, data);
	//std::tuple<unsigned long, char*> tp = std::make_tuple(data_size, data);
	//for(int i=0;i< data_size;i++)
	//	printf("%02X ", data[i]);
	return sp;
}

FileAddress Record::InsertRecord(const string file_name, Column_Head& h)
{
	pair<unsigned long, char*> sp = GetRecordData(h);
	// 插入记录
	return GetGlobalBuffer().GetFile(file_name.c_str())->AddRecord(sp.second, sp.first);
}

FileAddress Record::DeleteRecord(const std::string file_name, FileAddress fd, size_t record_size)
{
	return GetGlobalBuffer().GetFile(file_name.c_str())->DeleteRecord(&fd, record_size);
}

bool Record::UpdateRecord(const std::string file_name, Column_Head& h, FileAddress fd)
{
	pair<unsigned long, char*> sp = GetRecordData(h);
	return GetGlobalBuffer().GetFile(file_name.c_str())->UpdateRecord(&fd, sp.second, sp.first);
}