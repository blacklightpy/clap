# CLAP - Console Lossless Audio Player [DOCUMENTATION]
A CLI based music player for Windows built as part of the CBSE project work for 12<sup>th</sup> grade in C++ w/ Code::Blocks 17.10 IDE

# Introduction
This project was done as part of CBSE project work for the 12<sup>th</sup> grade. The standard is C++17. The primary focus of the project was not limited to creating a music player, but was also towards utilizing the various features of the programming language and thereby gaining a good exposure with it.

This is a CLI based program which can play and manage WAVE files (others are either propreitary or use complex encoding, so we did not support them since this is a school project).

This is a multi source project which uses various OOP methodologies. We perform R/W operations on binary files, operator and function overloading, serialization, memory management, exit handling, use DLLs and callbacks, Windows API, limited the use of non ANSI code, optimized UI, optimized performance, multithreading, use of configuration file, better PRNGs and other algorithms, bitmasks, etc.

We also created a data structure similar to std::deque to store the play queue (we didn't know of std::dequeue until we were halfway through the project, and due to lack of time, we didn't include it) in which we can insert or remove nodes from any end. It is the Queue class, which was fun designing (It crashed many systems because of simple bugs). The Player class inherits the queue from the Queue class. We found out while coding that OOP is not a simple task and requires a good amount of brainwork and skill.

# Features

* Play/Pause/Stop/Next/Previous
* Shuffle/Repeat
* CLI based interface
* Edit Libraries
* List all songs
* View audio technical and metadata information
* Play Queue which can be dynamically edited
* Delete/Rename audio files and playlists
* Quick Play
* Use of log files and exit codes
* Supports command line arguments
* Simplified a CLI program for an average user (could've added field selection, will be doing that in later builds)

# Technical Information

The WAVE format was developed by the Microsoft and IBM in the 1980s. This audio format is capable of storing audio in uncompressed format and compressed formats too. WAVE files are stored in chunks based file format called **RIFF** (Resource interchange File Format). The container is (.wav). Linear PCM (Linear Pulse Code Modulation) form is the most straightforward way of storing digital audio, as the notes are directly stored without any form of encoding. Other popular formats of digital audio include FLAC (Free and Lossless Audio Codec), ALAC (Apple Lossless Audio Codec), MP3, AAC etc. WAVE, FLAC and ALAC are lossless audio formats capable of storing CD Quality audio without loss digitally, whereas MP3 (deprecated), AAC etc. are lossy formats. This program demonstrates playback of L-PCM Wave files only.

For example, MP3 at 320Kbps has a loss of 78% of audio compared to uncompressed WAVE files. But WAVE files have a huge size because of storing CDQ Audio in an un-compressed form, which is the very reason why FLAC was developed. Typical CDQ Audio files have a bit rate of 1441Kbps. This is because CDQ Audio has a sample rate of 44.1 KHz, 16 Bits per Second and 2 Channels (left & right) which gives 
> 2 x 16 x 44,100 = 1,411,200 bits per second (bps); ***or approximately 1411 kbps***

Moving to the technicals, WAVE files are stored in RIFF format which has the following structure

![Waveform Structure](https://i.ibb.co/c6L7JLv/image.png)
 
Here, one can observe that the file is divides into various sections. They are called chunks. The first chunk is the header. All other chunks are subchunks of this chunk, which is called RIFF chunk.
The general structure of a subchunk is
 
```
struct subChunk
{
    FOURCC ChunkID;
    DWORD ChunkSize;
    LPBYTE Data;
};
```

Major chunks like **RIFF** and **LIST** chunks have the form:
```
struct Chunk
{
    FOURCC ChunkID;
    DWORD ChunkSize;
    union fccType
    {
        FOURCC ChunkType;
        LPBYTE ChunkData;
    };
};
```
***FOURCC*** refers to the four-character hex-code of the Chunk ID in ASCII stored in little-endian form

![RIFF Chunk](https://i.ibb.co/YfmzdTx/image.png)

The chunk type of the **RIFF** chunk in WAVE file is `WAVE`. The first subchunk is the `fmt` subchunk, which stores the file details such as SampleRate, ByteRate, Number of Channels, Bits per second etc.

The next chunk stores the audio data in the form of coordinates of the sound wave in hex code for each channels. Then there is an optional chunk called **LIST** chunk, which is a major chunk (still inside **RIFF** chunk) which stores audio metadata. People generally say WAVE files cannot be tagged, but there actually is this chunk which allows tagging. Also, recently the ID3v1 tagging format which is used for MP3 format was made available to other audio formats including WAVE. While **LIST/INFO** tags can store metadata only in text form and only up to around 30 characters per tag, which are also only a few (20-30) tags, ID3v1 can hold around 100s of tags and album art too. LIST chunks with type `INFO` are the ones that hold metadata. In the data part of the **LIST/INFO** chunk, there are many sub chunks holding audio metadata. Some of them are listed below:

|Chunk ID|Metadata           |
| ------ | ----------------- |
|**INAM**|*Title*            |
|**IART**|*Artist*           |
|**ITRK**|*Track No.*        |
|**ICRD**|*Year*             |
|**ISFT**|*Encoding Software*| 

Another technology which we have used is a 623-dimensionally equidistributed uniform pseudo-random number generator (***PRNG***) named ***Mersenne-Twister*** (*std::mt19937*) in honour of scientist Marin Mersenne. In Computer Science, there’s no means of generating a truly random number on its own. They require a seed or a value which varies, on which the computer performs a set of operations to generate a set of random values. The typical C-method is to seed the **PRNG** with system time. But this method has drawbacks; i.e. when two instances of the software is started simultaneously, the random numbers will be the same. Modern C++ thankfully has many new operators in `<random>` header to overcome this, `std::random_device` copies the state of a random variable of a random hardware connected to the system, which will be more variable than system time. It seeds the **PRNG** with this value. C++ has many PRNGs out of which `std::mt19937` (Mersenne twister with state size of 19937 bytes). We can’t use the std::random_device because its entropy is exhausted quickly and has large overheads.

Audio playback is done by the player class with the help of ***winmm.dll*** plugin. This plugin provides access to the Windows multimedia Library. We have internally used Waveform Functions of Windows API to facilitate audio controls. The data chunk of the WAVE file is loaded to memory using a **BYTE** (*unsigned char*) array in `Player::Load`. `Player::Play()` function passes the address of the callback function and the loaded data subchunk buffer to `WaveOutPrepare()` and internally calls `WaveOutOpen()`. Any message from the DLL invokes the callback function `Player::WaveOut_Proc()`

Windows API uses certain typedefs to ensure code portability across processor architectures. Some of them used in this program are

|Windows Data Type  |***typedef***                                 |
| ----------------- | -------------------------------------------- |
|**WORD**           |int with exactly 2 bytes                      |
|**DWORD**          |int with exactly 4 bytes                      |
|**BYTE**           |unsigned char                                 |
|**LPBYTE**         |unsigned char *                               |
|**BOOL**           |bool                                          |
|**DWORD_PTR**      |**DWORD**  *                                  |

You can get more information about such typedefs at [MSDN](https://docs.microsoft.com/en-us/windows/desktop/WinProg/windows-data-types)

# Conclusion

The Source code of the program was compiled successfully rectifying all errors. The program will run successfully. The whole program is menu driven and is user friendly. This program is developed as easy as possible for the sake of the user.
        It also has more options for future development. Some of the possible areas for development are:
⦁	Could’ve added Windows API to generate GUI (Which we couldn’t because of lack of adequate time and resources)
⦁	Could’ve created playlists in a standard format to make it playable in any standard audio player
⦁	Could’ve added volume controls and equalizer
⦁	Could’ve used different colors in different lines of text in a random fashion to make a better design
⦁	Could’ve used selectable menu fields instead of commands
⦁	Could’ve used FOSS libraries to allow portability to Linux and Macintosh systems and also to allow FLAC playback (MP3 is proprietary and also deprecated) as Windows Waveform API only allows playback of Linear PCM WAVE files. This is an absolute necessity because internally any form of audio is converted into LPCM before playback
⦁	Could’ve created the code in a better way so as to allow code portability to other users

We are sure we will keep better standards in our future programs and we conclude by hoping that our project will be useful for many enthusiasts.

