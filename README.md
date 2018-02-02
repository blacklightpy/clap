# CLAP - Console Lossless Audio Player
A CLI based music player for Windows built as part of CBSE XIIth grade project work in C++ w/ Code::Blocks 17.10 IDE

# Introduction
This project was done as part of CBSE project work for XIIth grade. The standard is C++17.

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
