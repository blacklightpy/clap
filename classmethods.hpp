/****************************************
** WAVPLAYER Class Methods Header File **
*****************************************/

#ifndef CBSE_WAVPLAYER_CLASSMETHODS_HPP
#define CBSE_WAVPLAYER_CLASSMETHODS_HPP

song_t::song_t()
{
    SongNo = 0;
    file = "";
    prev = next = nullptr;
}

songpath_t::songpath_t()
{
    path = "";
    file = "-.";
}

songpath_t& songpath_t::operator= (songpath_t const & Other)
{
    path = Other.path;
    file = Other.file;
    return(*this);
}

bool songpath_t::operator== (songpath_t const & Other)
{
    return (path == Other.path && file == Other.file);
}

song_t& song_t::operator= (song_t const& other)
{
    this->SongNo = other.SongNo;
    this->file = other.file;
    this->prev = other.prev;
    this->next = other.next;
    return (*this);
}

std::string songpath_t::filepath()
{
    std::string ret(path + file);
    return (ret);
}

///Construct integer range
///If parameters are in wrong order, swap them to right order
///@param low the lower bound of the inclusive range
///@param high the higher bound of the inclusive range

inline outofrange::outofrange(int low, int high) : low_(std::min(low, high)), high_(std::max(low, high))
{ }

///Check whether value lies outside the inclusive range
///@param test the value to test
inline bool outofrange::operator()(int test)
const
{
    return(test <= low_ || test >= high_);
}

enum repeatstate Player::RepeatState()
{
    return rState;
}

enum shufflestate Player::ShuffleState()
{
    return sState;
}

bool Player::IsValid()
{
    return (Data != NULL);
}

bool Player::IsPlaying()
{
    return (!Stopped && !Paused);
}

bool Player::IsStopped()
{
    return Stopped;
}

bool Player::IsPaused()
{
    return Paused;
}

Queue::Queue()
{
    start = end = nullptr;
    total = 0;
}

Queue::~Queue()
{
    Flush();
}

void Queue::Push(int const& pos, std::string filename)
{
    if(filename != "songscache.tmp")
        filename.insert(0, "playlists\\");
    song_t * ptr = new song_t;
    song_t * bkp;
    ptr->file = filename;
    ptr->SongNo = pos;
    ptr->next = nullptr;
    if(start == nullptr)
        start = end = ptr;
    else
    {
        end->next = ptr;
        bkp = end;
        end = ptr;
        end->prev = bkp;
    }
    ++total;
    if(total == 1)
    {
        cur = 1;
        std::thread t(quickload);
        t.detach();
    }
}

void Queue::Pop(std::string const& filename)
{
    song_t * cp = start;
    songpath_t reader;
    int pos;
    std::ifstream fin;
    for(pos = 1; pos <= total; ++pos)
    {
        fin.open(cp->file);
        for(int i(0); i < cp->SongNo; ++i)
        {
            fin >> reader;
            if(reader.filepath() == filename)
                break;
            fin.close();
            cp = cp->next;

        }
    }
    if(pos > total)
        return;
    if(pos < cur)
        --cur;
    cp = start;
    song_t * bkp;

    for(int i = 1; i < pos; ++i)
    {
        cp = cp->next;
    }

    if(cp == start)
    {
        cp = cp->next;
        delete cp->prev;
        cp->prev = nullptr;
        start = cp;
    }

    else if(cp != end)
    {
        cp->prev->next = cp->next;
        bkp = cp->next->prev;
        cp->next->prev = cp->prev;
        delete bkp;
    }

    else
    {
        cp->prev->next = nullptr;
        end = cp->prev;
        delete cp;
    }
    --total;
}

void Queue::Pop(int const& pos)
{
    if(pos > total || pos < 1)
        return;
    if(pos < cur)
        --cur;
    song_t * cp = start;
    song_t * bkp;

    for(int i = 1; i < pos; ++i)
    {
        cp = cp->next;
    }

    if(cp == start)
    {
        cp = cp->next;
        delete cp->prev;
        cp->prev = nullptr;
        start = cp;
    }

    else if(cp != end)
    {
        cp->prev->next = cp->next;
        bkp = cp->next->prev;
        cp->next->prev = cp->prev;
        delete bkp;
    }

    else
    {
        cp->prev->next = nullptr;
        end = cp->prev;
        delete cp;
    }
    --total;
}

void Queue::Move(int const& old, int const& newpos)
{

    song_t * cp = start, bkp;
    if(old < 0 || old > total || newpos < 0 || newpos > total)
        return;
    for(int i(0); i < old; ++i)
        cp = cp->next;
    bkp = *cp;
    Pop(old);
    cp = start;
    for(int i(0); i < newpos; ++i)
        cp = cp->next;
    cp->next->prev = new song_t(bkp);
    cp->next->prev->next = cp->next;
    cp->next->prev->prev = cp;
    cp->next = cp->next->prev;
}

void Queue::Display()
{
    song_t * cp = start;
    for(int i(1); i <= total; ++i)
    {
        if (!exists(cp->file))
        {
            Pop(i);
            --i;
            if (cp->file == "songscache.tmp")
                kill(1);
            log("Unable to load song from playlist " + cp->file);
        }
        cp = cp->next;
    }
    cp = start;
    std::fstream filereader;
    songpath_t cur;
    for(int i(1); cp != nullptr; ++i)
    {
        filereader.open(cp->file.c_str(), std::ios::binary | std::ios::in);
        filereader.seekg(0);
        filereader.clear();
        for(int j = 0; j < cp->SongNo; ++j)
            filereader >> cur;
        std::cout << i << ". " << cur.file << std::endl;
        filereader.close();
        cp = cp->next;
    }
}


void Queue::Pushnext(int const& pos, std::string srcfile)
{
    if(srcfile != "songscache.tmp")
        srcfile.insert(0, "playlists\\");
    song_t *ptr = new song_t;
    song_t *cp = start;

    ptr->file = srcfile;

    ptr->SongNo = pos;

    if(start == nullptr)
        start = end = ptr;
    else
    {
        for(int i = 1; i < cur; ++i)
            cp = cp->next;
        ptr->next = cp->next;
        ptr->prev = cp;
        cp->next = ptr;
        ptr->next->prev = ptr;
    }
    ++total;
    if(total == 1)
    {
        cur = 1;
        std::thread t(quickload);
        t.detach();
    }
}

void Queue::Flush()
{
    song_t * cp = start;;
    while(cp != nullptr)
    {
        start = cp->next;
        delete cp;
        cp = start;
    }
    start = end = nullptr;
    total = 0;
}

song_t::song_t(song_t const& other)
{
    this->SongNo = other.SongNo;
    this->file = other.file;
    this->prev = other.prev;
    this->next = other.next;
}

songpath_t Queue::Get(int const& pos)
{
    if(pos < 1 || pos > total)
        return NULLPATH;
    song_t * cp = start;
    songpath_t found;
    for(int i = 1; i < pos; ++i)
        cp = cp->next;
    if(!exists(cp->file))
    {
        if(cp->file == "songscache.tmp")
            kill(1);
        return (NULLPATH);
    }
    std::fstream finder(cp->file, std::ios::in|std::ios::binary);
    for(int j = 0; j < cp->SongNo; ++j)
        finder >> found;
    return found;
}




/// Initialize members
Player::Player(void)
{
    memset(&Descriptor, 0, sizeof(waveheader_t));
    memset(&Format, 0, sizeof(waveformat_t));
    Data = NULL;
    WaveOut = NULL;
    memset(&WaveHeader, 0, sizeof(WAVEHDR));
    Paused = false;
    Stopped = TRUE;
    rState = none;
    sState = off;
    Size = 0;
    CurrentSong = NULLPATH;
}

Player::~Player(void)
{
    /// Close output device
    Close();
}

void Player::Open(SHORT channels, DWORD sampleRate, SHORT bitsPerSample)
{
    /// Open output device
    SHORT format = WAVE_FORMAT_PCM;
    SHORT blockAlign = channels << 1;
    WAVEFORMATEX wfex;                  ///<WAVEFORMATEX is Defined in MMSystem.h
    wfex.wFormatTag = format;
    wfex.nChannels = channels;
    wfex.nSamplesPerSec = sampleRate;
    wfex.nAvgBytesPerSec = blockAlign * sampleRate;
    wfex.nBlockAlign = blockAlign;
    wfex.wBitsPerSample = bitsPerSample;
    wfex.cbSize = 0;
    waveOutOpen(&WaveOut, WAVE_MAPPER, &wfex, (DWORD_PTR)&Player::WaveOut_Proc, (DWORD_PTR)this, CALLBACK_FUNCTION);
    Paused = false;
    Stopped = true;

}

void Player::Close()
{
    /// Stop playback
    Stop();
    /// Close output device
    waveOutClose(WaveOut);
    SetConsoleTitle("CLAP: Console Lossless Audio Player");

    /// Check for valid sound data
    if (IsValid())
    {
        /// Clear sound data buffer
        delete [] Data;
        Data = NULL;
        Size = 0;
    }

}

void Player::Load(songpath_t& File)
{
    CurrentSong = File;
    if(mControl == NOWPLAYING)
        Display();
    if(CurrentSong == NULLPATH)
    {
        Close();
        return;
    }
    std::string toplay = File.filepath();

    /// Close output device
    Close();
    /// Load .WAV file

    std::ifstream file(toplay.c_str(), std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char *>(&Descriptor), sizeof(Descriptor));
    file.read(reinterpret_cast<char *>(&Format), sizeof(Format));
    DWORD id;
    DWORD size;

    file.read(reinterpret_cast<char *>(&id), sizeof(id));
    file.read(reinterpret_cast<char *>(&size), sizeof(size));
    char * signedData;
    signedData = new char[size];
    Data = new BYTE[size];

    file.read(signedData, size);
    memcpy(Data, signedData, size);
    delete [] signedData;
    Size = size;
    /// Open output device
    Open(Format.channels, Format.sampleRate, Format.bitsPerSample);

    /// Close .WAV file
    file.close();

}

void Player::Display()
{
    system("CLS");
    std::string filename(CurrentSong.file), rep, shuff;
    size_t lastdot = filename.find_last_of('.');
    if(lastdot != std::string::npos)
        filename.erase(lastdot);
    switch(RepeatState())
    {
    case off:
        rep = "Off";
        break;
    case one:
        rep = "One";
        break;
    case all:
        rep = "All";
        break;

    }
    switch(ShuffleState())
    {
    case off:
        shuff = "Off";
        break;
    case on:
        shuff = "On";
        break;
    }
    std::cout << std::left << std::setw(46) << "NOW PLAYING" << std::endl
              << std::left << std::setw(46) << "___________" << std::endl << std::endl
              << "Now Playing: " << filename << std::endl
              << "Shuffle: "    << shuff    << std::endl
              << "Repeat: "     << rep      << std::endl << std::endl
              << std::endl   << std::endl << std::endl
              << "Controls: " << std::endl
              << "_________" << std::endl
              << std::left << std::setw(10)  << leftarrow    << std::left << std::setw(10) << "\t:Previous"      << std::endl
              << std::left << std::setw(10)  << rightarrow   << std::left << std::setw(10) << "\t:Next"          << std::endl
              << std::left << std::setw(10)  << "[Spacebar]" << std::left << std::setw(10) << "\t:Play/Pause"    << std::endl
              << std::left << std::setw(10)  << "[Enter]"    << std::left << std::setw(10) << "\t:Stop"          << std::endl
              << std::left << std::setw(10)  << "/"          << std::left << std::setw(10) << "\t:Enter Command" << std::endl
              << std::endl   << std::endl << std::endl
              << "Commands: " << std::endl
              << "_________" << std::endl
              << std::left << std::setw(10) << "/view"    << std::left << std::setw(10) << "\t:View Queue" << std::endl
              << std::left << std::setw(10) << "/shuffle" << std::left << std::setw(10) << "\t:Toggle Shuffle" << std::endl
              << std::left << std::setw(10) << "/repeat"  << std::left << std::setw(10) << "\t:Toggle Repeat" << std::endl
              << std::left << std::setw(10) << "/flush"   << std::left << std::setw(10) << "\t:Clear Queue" << std::endl
              << std::left << std::setw(10) << "/back"    << std::left << std::setw(10) << "\t:Back to Menu"
              << std::endl;
    gotoxy(0, 7);

}

void Player::ToggleRepeat()
{
    if(rState == none)
        rState = one;
    else if(rState == one)
        rState = all;
    else
        rState = none;
}

void Player::ToggleShuffle()
{
    if(sState == off)
        sState = on;
    else
        sState = off;
}

void Player::Play()
{
    SetConsoleTitle(("[Playing] CLAP: Console Lossless Audio Player - " + CurrentSong.file).c_str());
    /// Check for valid sound data
    if (IsValid() && (Stopped || Paused))
    {
        if (Paused)
        {
            /// Continue playback
            (waveOutRestart(WaveOut));
        }
        else
        {
            /// Start playback
            WaveHeader.lpData = (LPSTR)Data;
            WaveHeader.dwBufferLength = Size;
            waveOutPrepareHeader(WaveOut, &WaveHeader, sizeof(WAVEHDR));
            waveOutWrite(WaveOut, &WaveHeader, sizeof(WAVEHDR));
            SetConsoleTitle(("[Playing] CLAP: Console Lossless Audio Player - " + CurrentSong.file).c_str());
        }
        Paused = false;
        Stopped = false;
    }

}

void Player::Stop()
{
    SetConsoleTitle("CLAP: Console Lossless Audio Player");
    /// Check for valid sound data
    if (IsValid() && (!Stopped))
    {
        /// Stop playback
        Stopped = true;
        waveOutReset(WaveOut);
        waveOutUnprepareHeader(WaveOut, &WaveHeader, sizeof(WAVEHDR));
    }
}

void Player::Pause()
{
    /// Check for valid sound data
    if (IsValid() && (!Paused) && (!Stopped))
    {
        /// Pause playback
        Paused = true;
        waveOutPause(WaveOut);
        SetConsoleTitle(("[Paused] CLAP: Console Lossless Audio Player - " + CurrentSong.file).c_str());
    }

}

bool Player::WaveOut_Proc(HWAVEOUT hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    bool bResult = TRUE;

    /// Get current object
    Player* Cur = (Player*)((DWORD_PTR)&dwInstance);
    if (Cur->IsStopped())
        return bResult;
    /// Check for playback finished
    if ((uMsg == WOM_DONE) && ((!Cur->Stopped) || (!Cur->Paused)))
    {
        /// Relase sound info
        if (waveOutUnprepareHeader(Cur->WaveOut, &Cur->WaveHeader, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
            bResult = false;
        if(Cur->RepeatState() == one)
            Cur->Repeat();
        else
        {
            if (Cur->ShuffleState() == on)
            {
                int tmp(cur);
                std::uniform_int_distribution<int> shuffle(1, Cur->size()); /// distribute results between 1 and 6 inclusive
                while(cur == tmp)
                    cur = shuffle(generator);
            }
            else if (Cur->RepeatState() == none)
            {
                if(++cur = Cur->size())
                    Cur->Stop();
            }
            else if (Cur->RepeatState() == all)
            {
                if (++cur > Cur->size())
                    cur = std::min(1, Cur->size());
            }
            else if (Cur->RepeatState() == one)
                /*empty*/;
            songpath_t loader = Cur->Get(cur);
            Cur->Load(loader);
            if(Cur->IsValid())
                Cur->Play();
        }
    }

    return bResult;
}

void Player::Next()
{
    if (sState == on)
    {
        int tmp(cur);
        std::uniform_int_distribution<int> shuffle(1, size()); /// distribute results between 1 and 6 inclusive
        while(cur == tmp)
            cur = shuffle(generator);
    }
    else if (rState == none)
    {
        if(++cur > size())
            cur = std::min(1, size());
    }
    else if (rState == all)
    {
        if (++cur > size())
            cur = std::min(1, size());
    }
    else if (rState == one)
    {
        if(++cur > size())
            cur = std::min(1, size());
    }
    songpath_t loader = Get(cur);
    Load(loader);
    Play();
}


void Player::Prev()
{
    if(sState == on)
    {
        int tmp = cur;
        std::uniform_int_distribution<int> shuffle(1, size()); /// distribute results between 1 and 6 inclusive
        while (cur == tmp)
            cur = shuffle(generator);

    }
    else
    {
        if(--cur < 1)
        {
            cur = size();
        }
    }
    songpath_t loader = Get(cur);
    Load(loader);
    Play();
}

void Player::Repeat()
{
    Stop();
    Play();
}

#endif
