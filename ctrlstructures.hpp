/*********************************************
** WAVPLAYER Control Structures Header File **
**********************************************/

#ifndef CBSE_WAVPLAYER_CTRLSTRUCTURES_HPP
#define CBSE_WAVPLAYER_CTRLSTRUCTURES_HPP

///Enumerations for Player Controls
enum MenuControl    { HOME, NOWPLAYING, ALLSONGS, PLAYLISTS, SETTINGS, ABOUT, EXIT };
enum repeatstate    { none, one, all };
enum shufflestate   { off, on };

///Class used to store song Number and playlist in memory using Queue class
struct song_t
{
    int SongNo;
    std::string file;
    song_t * prev;
    song_t * next;
    song_t& operator= (song_t const&);
    song_t();
    song_t(song_t const&);
};


///Class used to write playlist files
class songpath_t
{
public:
    std::string path;
    std::string file;
    songpath_t& operator= (songpath_t const &);
    bool operator== (songpath_t const & );
    std::string filepath();
    songpath_t();
};

///Class Used to read WAVE File Header
struct waveheader_t
{
    DWORD riff;
    DWORD size;
    DWORD wave;

};

///Class used to read WAVE "fmt" sub chunk
struct waveformat_t
{
    DWORD id;
    DWORD size;
    WORD format;
    WORD channels;
    DWORD sampleRate;
    DWORD  byteRate;
    WORD blockAlign;
    WORD bitsPerSample;

};

///RIFF chunk structure
struct riffchunk_t
{
    DWORD chunkID;
    DWORD chunkSize;
    DWORD chunkType;          ///< RIFF form type
};

///RIFF sub-chunk structure
struct riffsubchunk_t
{
    DWORD Id;
    DWORD Size;
};

///Class to store metadata
struct metadata_t
{
    std::string Artist;
    std::string Track;
    std::string Album;
    std::string Genre;
    std::string TrackNo;
    std::string Year;
    std::string CopyRight;
    std::string Comments;
    std::string Software;
};

///Out Of Range Functor - Check whether integers lie outside (low,high)
class outofrange
{
private:
    int const low_;
    int const high_;
public:
    inline outofrange(int low, int high);
    inline bool operator()(int test) const;
};

#endif
