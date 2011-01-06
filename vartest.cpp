/* 
 * File:   main.cpp
 * Author: Owen
 *
 * Created on 04 December 2010, 19:59
 */

#include <stdlib.h>
#include "varlist.h"
#include <iostream>

/*
 * 
 */
int main(int argc, char** argv)
{
	VarList::Container vlc;
	int i = 6;
	char tmpc;
	//vlc.addtype<int>();
	vlc.set<int>(std::string("rara"), 1);
	vlc.set("rara2", i);

	std::vector<std::string> strvec;
	strvec.push_back("rara2");
	//std::cout << vlc.getarray<int>(strvec)[0] << std::endl;
	try
	{
		std::cout << "i = " << vlc.get<int>(strvec[0]) << std::endl;
	}

	catch (VarList::VarNotFoundException& e)
	{
		std::cout << e.what();
	}
	return (EXIT_SUCCESS);
}

