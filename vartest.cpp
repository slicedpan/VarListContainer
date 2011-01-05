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
    varListContainer vlc;
    int i = 6;
    char tmpc;
    //vlc.addtype<int>();
    vlc.set<int>("rara", 1);
    vlc.set("rara2", i);

    if(!vlc.get("rara", &i))
    {
        std::cout << "error1!" << std::endl;
    }
    std::cout << i << std::endl;

    if (!vlc.get<int>("rara2", &i))
    {
        std::cout << "error2!" << std::endl;
    }

    vlc.set<char>("mych", 'h');
    vlc.set<float>("myfl", 54.334);

    if (!vlc.get<char>("myfl", &tmpc))
    {
        std::cout << "error3!" << std::endl;
    }

    if (!vlc.get("myfl", &tmpc))
    {
        std::cout << "error4!" << std::endl;
    }

    /*if (!vlc.get<float>("myfl", &tmpc)) //This won't compile at all thankfully
    {
        std::cout << "error5!" << std::endl;
    }
    */

    if (!vlc.get<char>("mych", (char*)&i))
    {
        std::cout << "error5" << std::endl;
    }
    std::cout << "i = " << i << std::endl;
   
    return (EXIT_SUCCESS);
}

