/***********************************************************************************
**    C++ WAVE Player
**    Authors: @Jyothish and @Abhishek
**    For best results, compile in VC++
**
**    Sources: Anatomy of WAVE format: http://soundfile.sapp.org/doc/waveformat_t/
**             Microsoft Developer Network
**             StackOverflow Community
**             RIFF, LIST and other chunks
**             Exploring C++ by Ray Lischner
***********************************************************************************/

///Next two lines define build target for Windows 2000 and above
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#include <iostream>                     ///< For std::cin, std::cout
#include <ostream>                      ///< For operator  <<
#include <istream>                      ///< For operator  >>
#include <fstream>                      ///< For file streams
#include <sstream>                      ///< For std::string stream
#include <cstdio>                       ///< For C I/O functions
#include <cstdlib>                      ///< For C Library functions
#include <thread>                       ///< For multi threading
#include <map>                          ///< For std::map
#include <iomanip>                      ///< For I/O manipulators
#include <vector>                       ///< For std::vector
#include <random>                       ///< For random number generators
#include <algorithm>                    ///< For STL algorithms
#include <tchar.h>                      ///< For wide characters
#include <io.h>                         ///< For file path functions from Windows API
#include <windows.h>                    ///< For Windows API
#include <mmsystem.h>                   ///< For Multimedia functions
#include <cstring>                      ///< For primitive string functions
#include <string>                       ///< For std::string class
#include <iterator>                     ///< For special iterators
#include <limits>                       ///< For numeric limit values
#include <locale>                       ///< For text functions
#include <conio.h>                      ///< For Console Functions [Exception to ANSI c++]

#include "includes/defines.hpp"
#include "includes/ctrlstructures.hpp"
#include "includes/queue.hpp"
#include "includes/player.hpp"
#include "includes/declares.hpp"
#include "includes/global.hpp"
#include "includes/maps.hpp"
#include "includes/functions.hpp"
#include "includes/subroutines.hpp"
#include "includes/classmethods.hpp"

#pragma comment(lib, "winmm")           ///< To link winmm.dll if MS VC++ is used


///Main Function
int main(int argc, char * argv[])
{
    argcount = argc;
    if(argc > 2)
    {
        kill(3);
    }
    else if(argc == 2)
    {
        std::string arg(argv[1]);
        size_t lastdot(arg.find_last_of('.'));
        if(lastdot == std::string::npos)
            kill(3);
        std::string ext = arg.substr(lastdot + 1, arg.size());
        if(ext == "pls")
        {
            if(!exists(arg))
                kill(4);
            system(("copy " + arg + " playlists").c_str());
            if(arg.find_last_of('\\') != std::string::npos)
                arg.erase(arg.find_last_of('\\'));
            songpath_t reader;
            std::ifstream fin(arg.c_str(), std::ios::in|std::ios::binary);
            for(int i(1); fin >> reader && !fin.eof(); ++i)
            {
                P.Push(i, arg);
            }
            P.Play();
        }
        if(ext == "wav")
        {
            if(!exists(arg))
                kill(4);
            songpath_t loadr;
            loadr.file = arg;
            P.Load(loadr);
            P.Play();
        }
        else
            kill(3);
    }
    system("MODE CON COLS=80 LINES=50");    ///< For setting Console Window Size
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, true );    ///< For initializing control handler
    SetConsoleTitle("CLAP: Console Lossless Audio Player");    ///< Setting Console Window Title
    if(argc == 1)
        getall();
    char ch;
    do
    {
        system("@echo off");
        system("COLOR 0F");
        system("CLS");                      ///< Clear Console Window
        std::cout << std::right << std::setw(44) << "____________________" << std::endl << std::endl
                  << std::right << std::setw(44) << "C++ WAV Music Player" << std::endl
                  << std::right << std::setw(44) << "____________________" << std::endl << std::endl
                  << std::right << std::setw(64) << "-By Abhishek and Jyothish"
                  << std::right << std::endl << std::endl << std::endl << std::endl;
        std::cout << "1. Now Playing" << std::endl
                  << "2. All Songs" << std::endl
                  << "3. Playlists" << std::endl
                  << "4. Settings" << std::endl
                  << "5. About" << std::endl
                  << "6. Exit" << std::endl;
        ch = getch();
        switch(ch)
        {
        case '1':
            mControl = NOWPLAYING;
            break;
        case '2':
            mControl = ALLSONGS;
            break;
        case '3':
            mControl = PLAYLISTS;
            break;
        case '4':
            mControl = SETTINGS;
            break;
        case '5':
            mControl = ABOUT;
            break;
        case '6':
            mControl = EXIT;
            break;
        default :
            mControl = HOME;
            break;
        }

        switch(mControl)
        {
        case NOWPLAYING :
            nowplaying();
            break;
        case ALLSONGS   :
            allsongs();
            break;
        case PLAYLISTS  :
            playlists();
            break;
        case SETTINGS   :
            settings();
            break;
        case ABOUT      :
            about();
            break;
        case EXIT       :
        case HOME       :
            break;
        }
    }
    while(mControl != EXIT);
    return 0;
}
