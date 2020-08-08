#pragma once
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : Timer.H                                                      *
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
#ifndef TIMER_H_
#define TIMER_H
#include"Global.h"
#include"Error.h"
#include<iostream>
using namespace std;
//#define CLOCKS_PER_SEC ((clock_t)1000)
class Timer;
Timer& GetGlobalTimer();
class Timer {
	clock_t starttime;
	clock_t endtime;
public:
	void Start() {
		starttime = clock();
	}
	void End() {
		endtime = clock();
	}
	void PrintTime() {
		cout << "select time:" << (double)(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;
	}
};
#endif
