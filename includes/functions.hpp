/************************************
** WAVPLAYER Functions Header File **
*************************************/

#ifndef CBSE_WAVPLAYER_FUNCS_HPP
#define CBSE_WAVPLAYER_FUNCS_HPP

///Operator Overloads
std::istream& operator>>  (std::istream& in,songpath_t & SP)
{
    int s1(0),s2(0);
    in.read((char*)&s1,sizeof(s1));
    SP.path.resize(s1);
    in.read(&SP.path[0],s1);
    in.read((char*)&s2,sizeof(s2));
    SP.file.resize(s2);
    in.read(&SP.file[0],s2);
    return in;
}
std::ostream& operator <<  (std::ostream &out,songpath_t const &SP)
{
    int s1(SP.path.size()),s2(SP.file.size());
    out.write((char*)&s1,sizeof(s1));
    out.write(SP.path.c_str(),s1);
    out.write((char*)&s2,sizeof(s2));
    out.write(SP.file.c_str(),s2);
    return out;
}

inline bool pathexists(std::string const& dirName)
{
    /// Get file attributes in bitmask format (Perform bitwise & to compare)
    unsigned long attribs = GetFileAttributesA(dirName.c_str());
    if (attribs == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

///Read the WAVE file, checks if contains a valid format. WAVE files start with the first 4 unsigned chars
///containing the letters "RIFF" in ASCII form (Called ChunkID). Thus the function.

inline bool isLegit (const std::string& name)
{
    std::fstream file(name.c_str(), std::ios::in|std::ios::binary);
    waveheader_t whdr;
    file.read(reinterpret_cast<char *>(&whdr), sizeof(whdr));
    file.close();
    return(whdr.riff == RIFF && whdr.wave == WAVE);
}

///To check whether a file exists
inline bool exists (const std::string& name)
{
    std::ifstream file(name.c_str(), std::ios::in);
    return (file.good());
}

void ignoreline()
{
    std::cin.ignore(std::numeric_limits<int>::max(), '\n');
}

void log(std::string const& str)
{
    std::fstream out("player.log", std::ios::out|std::ios::app);
    out << str << '\n';
}

bool gotoxy(short const x, short const y)
{
    COORD xy;
    xy.X = x;
    xy.Y = y;
    return(SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy));
}

/// Custom Program Destructor
void kill(int code)
{
    std::cout << errorcodes[code];
    log(errorcodes[code]);
    P.Flush();
    remove("songscache.tmp");
    exit(code);
}
void quickload()
{
    songpath_t toplay;
    toplay = P.Get(1);
    P.Load(toplay);
}
void showmeta(songpath_t& filename)
{
    std::cout << std::endl;
    waveheader_t whdr     {0, 0, 0};
    waveformat_t wfmt     {0, 0, 0, 0, 0, 0, 0, 0};
    riffsubchunk_t m_subchunk {0, 0};
    metadata_t meta       {"N.A", "N.A", "N.A", "N.A", "N.A", "N.A", "N.A", "N.A", "N.A"};
    std::string subckdata;
    std::string format ("Non LPCM WAVE [Unplayable]");
    std::map <uint32_t, std::string> extras;
    long double filesize(0);
    int offset(0);
    int listsize(0);
    uint32_t subcktype;
    std::fstream wave(filename.filepath().c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    filesize = wave.tellg();
    if(!wave)
    {
        std::cout << "File does not exist!" << std::endl;
        return;
    }
    std::cout << std::left << std::setw(24) << "Details" << std::endl
              << std::left << std::setw(24) << " _______" << std::endl << std::endl
              << std::left << std::setw(20) << "File Name" << ": " << filename.file << std::endl
              << std::left << std::setw(20) << "Path" << ": " << filename.filepath() << std::endl
              << std::left << std::setw(20) << "Size" << ": " << std::fixed << std::setprecision(2) << filesize / (1000 * 1000)
              << " MB" << std::endl << std::endl << std::endl;
    std::cout.unsetf(std::ios::fixed);
    wave.seekg(0);
    wave.clear();
    wave.read(reinterpret_cast<char *>(&whdr), sizeof(whdr));
    wave.read(reinterpret_cast<char *>(&wfmt), sizeof(wfmt));
    if(whdr.riff != RIFF && whdr.wave != WAVE)
    {
        std::cout << "Corrupt file!" << std::endl;
        return;
    }
    if(wfmt.format == PCM_CODE)
        format = "Linear PCM WAVE [Playable]";
    std::cout << std::left << std::setw(20) << "Bitrate" << ": " << (wfmt.byteRate * 8) / 1000 << "Kbps" << std::endl
              << std::left << std::setw(20) << "Format" << ": " << format         << std::endl << std::endl;
    while(wave.read(reinterpret_cast<char *>(&m_subchunk), sizeof(m_subchunk)) && !wave.eof())
    {
        if(m_subchunk.Id == LIST)
        {
            wave.read(reinterpret_cast<char *>(&subcktype), sizeof(subcktype));
            if(subcktype == INFO)
                break;
        }
        wave.clear();
        wave.seekg(m_subchunk.Size, std::ios::cur);
    }
    if(subcktype == INFO)
    {
        offset += 8;
        listsize = m_subchunk.Size;
        while(offset < listsize)
        {
            wave.read(reinterpret_cast<char *>(&m_subchunk), sizeof(m_subchunk));
            subckdata.clear();
            subckdata.resize(m_subchunk.Size);
            wave.read(reinterpret_cast<char *>(&subckdata[0]), m_subchunk.Size);
            switch (m_subchunk.Id)
            {
            case IART:
                meta.Artist = subckdata;
                break;
            case INAM:
                meta.Track = subckdata;
                break;
            case IPRD:
                meta.Album = subckdata;
                break;
            case ITRK:
                meta.TrackNo = subckdata;
                break;
            case ICRD:
                meta.Year = subckdata;
                break;
            case IGNR:
                meta.Genre = subckdata;
                break;
            case ICMT:
                meta.Comments = subckdata;
                break;
            case ICOP:
                meta.CopyRight = subckdata;
                break;
            case ISFT:
                meta.Software = subckdata;
                break;
            default:
                if(infotags.find(m_subchunk.Id) != infotags.end())
                    extras[m_subchunk.Id] = subckdata;
                break;
            }
            offset += sizeof(m_subchunk) + m_subchunk.Size;
        }
    }

    std::cout << std::left << std::setw(20) << "Metadata" << std::endl
              << std::left << std::setw(24) << "________" << std::endl << std::endl
              << std::left << std::setw(20) << "Artist" << ": " << meta.Artist    << std::endl
              << std::left << std::setw(20) << "Track" << ": " << meta.Track     << std::endl
              << std::left << std::setw(20) << "Album" << ": " << meta.Album     << std::endl
              << std::left << std::setw(20) << "Track Number" << ": " << meta.TrackNo   << std::endl
              << std::left << std::setw(20) << "Year" << ": " << meta.Year      << std::endl
              << std::left << std::setw(20) << "Genre" << ": " << meta.Genre     << std::endl
              << std::left << std::setw(20) << "Comments" << ": " << meta.Comments  << std::endl
              << std::left << std::setw(20) << "Copyright" << ": " << meta.CopyRight << std::endl
              << std::left << std::setw(20) << "Software" << ": " << meta.Software  << std::endl;
    for(std::map <uint32_t, std::string>::iterator i (extras.begin());
            i != extras.end(); ++i)
        std::cout << std::left << std::setw(20) << infotags[i->first]
                  << ": " << extras[i->first] << std::endl;
    std::cout << std::endl;
}

///Handle all exit events
bool CtrlHandler(int control)
{
    switch (control)
    {

    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        kill(0);
    }
    return true;
}

void getall()
{
    songpath_t filler;
    std::vector<std::string> pathlist;
    std::string cur_path;
    _finddata_t found;
    int x(0), more(0);
    if(!exists("libraries.cfg"))
        kill(1);
    std::fstream lib("libraries.cfg", std::ios::in);
    getline(lib, cur_path, ':');
    if(cur_path != "PATH")
        kill(2);
    cur_path.clear();
    while(!lib.eof())
    {
        getline(lib, cur_path, ';');
        replace(cur_path.begin(), cur_path.end(), '/', '\\');
        if(pathexists(cur_path))
            pathlist.push_back(cur_path);
    }
    lib.close();
    std::fstream cache("songscache.tmp", std::ios::binary|std::ios::out|std::ios::trunc);
    for(std::vector<std::string>::iterator i(pathlist.begin()); i != pathlist.end(); ++i)
    {
        x = _findfirst((*i + "*.wav").c_str(), &found);
        if(x != -1)
        {
            if(isLegit(*i + found.name))
            {
                filler.path = *i;
                filler.file = found.name;
                cache << filler;
            }
            more = _findnext(x, &found);
            while(more == 0)
            {
                if(isLegit(*i + found.name))
                {
                    filler.path = *i;
                    filler.file = found.name;
                    cache << filler;
                }
                more = _findnext(x, &found);
            }
        }
        _findclose(x);
    }
}

int  strtoint(std::string const& text)
{
    return atoi(text.c_str());
}

///Output manipulators
std::ostream& leftarrow (std::ostream& output)
{
    std::string larrow;
    larrow.push_back((char)27);
    larrow.push_back((char)196);
    output << larrow << ' ';
    return (output);
}

std::ostream& rightarrow (std::ostream& output)
{
    std::string rarrow;
    rarrow.push_back((char)196);
    rarrow.push_back((char)26);
    output << rarrow << ' ';
    return (output);
}

#endif
