/***************************************
** WAVPLAYER Player Class Header File **
****************************************/

#ifndef CBSE_WAVPLAYER_PLAYER_HPP
#define CBSE_WAVPLAYER_PLAYER_HPP

class Player : public Queue
{
private:
    void Open(SHORT channels, DWORD sampleRate, SHORT bitsPerSample);
    bool static CALLBACK WaveOut_Proc(HWAVEOUT hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
    waveheader_t Descriptor;
    waveformat_t Format;
    HWAVEOUT WaveOut;
    WAVEHDR WaveHeader;         ///< Defined in MMSystem.h
    LPBYTE Data;
    DWORD Size;
    songpath_t CurrentSong;
    bool Stopped;
    bool Paused;
    repeatstate rState;
    shufflestate sState;
public:
    Player(void);
    virtual ~Player(void);
    void Load(songpath_t&);
    void Repeat();
    void Play();
    void Stop();
    void Pause();
    enum repeatstate RepeatState();
    enum shufflestate ShuffleState();
    bool IsValid();
    bool IsPlaying();
    bool IsStopped();
    bool IsPaused();
    void Display();
    void ToggleRepeat();
    void ToggleShuffle();
    void Next();
    void Prev();
    void Close();
};

#endif
