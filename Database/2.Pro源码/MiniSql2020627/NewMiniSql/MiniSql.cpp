#include <iostream>
#include <random>
#include <math.h>
#include"Interpreter.h"
#include"Error.h"
void RunMiniSQL();//运行程序
void PrimtWelcome();
string GetCommand();//得到输入命令
#include <stdio.h>
#include<ctime>
const char sj[27] = "abcdefghijklmnopqrstuvwxyz";
int main()
{
	//printf("%d", sizeof(BTNode));
	//FILE* fp = fopen("../test4.txt", "wb+");
	//int j = 0;
	//for (int j = 0; j < 10000; j++) {
	//	double value = 0;
	//	srand(j);
	//	int num = rand() % 8+1;
	//	char k[10] = "";
	//	int i;
	//	for (i = 0; i < num; i++) {
	//		int num2 = rand() % 26;
	//		k[i] = sj[num2];
	//	}
	//	k[i + 1] = '\0';
	//	double x = (rand() % 100000000 + 1);
	//	double y = (rand() % 100000000 + 1);
	//	value = x / y + y/x -x + y* 9.921 - x*1.023;
	//	double value2 = 10000 / value + 0.1234* value + value / 10000 - x * 1.023 + 2.21378231;
	//	printf("insert into test values(%d,%.7lf,%.7lf,%s);\n", j,value,value2,k);
	//	fprintf(fp,"insert into test values(%d,%.7lf,%.7lf,%s);\n", j, value,value2, k);
	//}
	PrimtWelcome();
	RunMiniSQL();
	return 0;
}
void PrimtWelcome() {
	cout << "+---------------------------------------------------+" << endl;
	cout << "Welcome to the minisql system!" << endl;
	cout << "+---------------------------------------------------+" << endl;
}
void RunMiniSQL()
{
	int num = 0;
	SensefulStr senstr;
	PrintWindow print_window;
	while (true)
	{
		try{
		string cmd = GetCommand();      //获得一行完整的命令
		senstr.SetStr(cmd);  
		auto cmd_type = GetOpType(senstr.GetSensefulStr());

		if (cmd_type == CmdType::QUIT)break;
		if (cmd_type == CmdType::HELP)
		{
			//Help();
			continue;
		}
		if (cmd_type == CmdType::FILE) {
			FILE* fp = fopen(senstr.GetSensefulStr()[1].c_str(),"rb+");
			if (fp == NULL) {
				throw Error("File not existed!");
			}
			while (!feof(fp)) {
				num++;
				char cmd[120] = "";
				fgets(cmd, 120, fp);
				cout << "Cmd Line" << num << ":" << cmd <<  endl;
				senstr.SetStr(cmd);
				auto cmd_type = GetOpType(senstr.GetSensefulStr());
				Interpreter(senstr.GetSensefulStr(), cmd_type, print_window);
			}
		}

		Interpreter(senstr.GetSensefulStr(), cmd_type, print_window);
		}
		catch (Error &e)
		{
			DispatchError(e);
			std::cout << std::endl;
			continue;
		}
	}
}
string GetCommand()
{
	string res;
	string tmp;
	int n = 0;
	do {
		if (n == 0) {
			cout << "Minisql>";
		}
		n++;
		getline(cin, tmp);
		res += tmp;
		if (tmp[tmp.size() - 1] != ';')
			res += " ";
	} while (tmp[tmp.size() - 1] != ';');

	return res;
}
/* BufferManager  & Record Test Code */
//int main() {
//	Key_Attr a(1);
//	Key_Attr b(2);
//	Key_Attr c((char*)"BOOA");
//	Key_Attr d((char*)"BOOB");
//	Key_Attr e((float)2.0);
//	Key_Attr f((float)3.65);
//	Column_Cell ca(a), cb(b), cc(c), cd(d), ce(e), cf(f);
//	Column_Head ch;
//	cout << a << b << c << d << e << f << endl;
//	ch.attachCell(ca);
//	ch.attachCell(cb);
//	ch.attachCell(cc);
//	ch.attachCell(cd);
//	ch.attachCell(ce);
//	ch.attachCell(cf);
//	cout << ch << endl;
//	Buffer B1;
//	GetGlobalBuffer().CreateFile("../test.txt");
//	Record R1;
//	R1.InsertRecord("../test.txt", ch);
//}