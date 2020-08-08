#pragma once
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  2 0 2 0 D B S - G R O U P W O R K             ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : MiniSql		                                               *
 *                                                                                             *
 *                    File Name : Error.H                                                      *
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

#ifndef __ERROR_H__
#define __ERROR_H__
#include <iostream>
#include <fstream>
#include <string>
	extern std::fstream log_file;
	class Error
	{
	public:
		void PrintError()const;
		Error(const std::string s);
	protected:
		std::string ErrorInfo;
		std::string ErrorPos;
		
	};
	// ´íÎó´¦Àíº¯Êý
	void DispatchError(const Error &error);

#endif
