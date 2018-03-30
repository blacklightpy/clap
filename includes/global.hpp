/*******************************************
** WAVPLAYER Global Variables Header File **
*******************************************/

#ifndef CBSE_WAVPLAYER_GLOBAL_HPP
#define CBSE_WAVPLAYER_GLOBAL_HPP

MenuControl mControl(HOME);             ///< MenuControl Object
Player P;                               ///< Player Object
int argcount;
bool didgetall(false);
songpath_t NULLPATH;                    ///< Empty songpath_t for comparison
int cur(0);                             ///< Current song number in Queue
std::random_device rd;                  ///< Non-deterministic random number generator to seed Mersenne Twister
std::mt19937 generator(rd());           ///< Initializing Mersenne Twister with std::random_device as seed

#endif
