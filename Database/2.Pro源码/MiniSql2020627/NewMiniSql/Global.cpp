#include"Global.h"

std::string tolower(std::string str)
{
	for (auto &c : str)
		tolower(c);
	return str;
}
