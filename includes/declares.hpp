/************************************************
** WAVPLAYER Function Declarations Header File **
*************************************************/

#ifndef CBSE_WAVPLAYER_DECLARES_HPP
#define CBSE_WAVPLAYER_DECLARES_HPP

void quickload();
bool gotoxy(short const, short const);
void log(std::string const&);
void kill(const int);
void ignoreline();
bool isLegit(std::string const&);
bool exists(std::string const&);
bool pathexists(std::string const&);
void showmeta(songpath_t&);
void allsongs();
void nowplaying();
void settings();
void playlists();
void about();
void getall();
bool CtrlHandler(int);
int strtoint(std::string const&);
void quickload();

#endif
