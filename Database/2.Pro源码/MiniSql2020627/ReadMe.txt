此Mini数据库系统由四人小组于2020年6月编写，小组成员为：沈吕可晟/陆子仪/邢书婷/罗昱哲
此文件夹根目录下存有两个根目录文件可供进行批量测试，
直接在工程文件中进行编译后执行 execfile ../FILE_NAME.txt;

此工程项目分为以下模块：
API —— api函数，调用底层代码
BTREE —— 索引B+树相关代码
BUFFERMANAGER —— 最底层与文件交互区域（缓冲区管理）
CATLOG —— 目录管理
ERROR —— 异常类管理
GLOBAL ——全局变量管理
INTERPRETER —— 解释器（编译器）管理
RECORD —— 记录相关代码
TIMER —— 计时器代码

其中Minisql.cpp为程序主入口
