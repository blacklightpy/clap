/**************************************
** WAVPLAYER Subroutines Header File **
***************************************/

#ifndef CBSE_WAVPLAYER_SUBROUTINES_HPP
#define CBSE_WAVPLAYER_SUBROUTINES_HPP

void nowplaying()
{
    system("COLOR 0D");
    char ch('\0');
    std::string cmd;
    int old,newpos;
    do
    {
        P.Display();
        ch = getch();
        switch(ch)
        {
        case ARROW:
            ch = getch();
            switch(ch)
            {
            case LEFT:
                P.Prev();
                break;
            case RIGHT:
                P.Next();
                break;
            }
            break;
        case SPACEBAR:
            if(P.IsPlaying())
                P.Pause();
            else
                P.Play();
            break;
        case ENTER:
            P.Stop();
            break;
        case '/':
            std::cout << "Enter Command: ";
            std::cin >> cmd;
            if(cmd == "view")
            {
                ignoreline();
                do
                {
                    system("cls");
                    std::cout << "Play Queue" << std::endl
                              << "__________" << std::endl << std::endl;
                    P.Display();
                    std::cout << std::endl    << std::endl
                              << "Commands:"  << std::endl
                              << "_________"  << std::endl     << std::endl
                              << std::left    << std::setw(10) << "remove" << rightarrow << "Removes selection from play queue" << std::endl
                              << std::left    << std::setw(10) << "move"   << rightarrow << "Moves selection to position"       << std::endl
                              << std::left    << std::setw(10) << "back"   << rightarrow << "Back to now playing"               << std::endl
                              << std::endl;
                    std::cin >> cmd;

                    if(cmd == "remove")
                    {
                        int no;
                        if(std::cin.get() == '\n')
                            std::cout  <<  "Enter number to be removed: ";
                        else
                            std::cin.unget();
                        std::cin >> no;
                        std::cin.ignore();
                        P.Pop(no);
                        if(cur < no)
                            --cur;
                        if(cur == no - 1);
                        if(P.IsPlaying())
                            P.Next();
                        else
                        {
                            songpath_t loader = P.Get(cur);
                            P.Load(loader);
                        }
                    }
                    else if(cmd == "move")
                    {
                        if(std::cin.get() == '\n')
                            std::cout  <<  "Enter number to be moved: ";
                        else
                            std::cin.unget();
                        std::cin >> old;
                        if(std::cin.get()=='\n')
                            std::cout << "Enter new position: ";
                        else
                            std::cin.unget();
                        std::cin >> newpos;
                        std::cin.ignore();
                        P.Move(old, newpos);
                    }
                    else
                        ignoreline();
                }
                while(cmd != "back");
            }
            else if(cmd == "shuffle")
            {
                P.ToggleShuffle();
                ignoreline();
            }
            else if(cmd == "repeat")
            {
                P.ToggleRepeat();
                ignoreline();
            }
            else if(cmd == "back")
                mControl = HOME;
            break;
        }
    }
    while(mControl == NOWPLAYING);
}

void allsongs()
{
    system("COLOR 0A");
    if (argcount == 2)
        if(!didgetall)
        {
            didgetall = true;
            getall();
        }
    songpath_t reader, deleter;
    std::fstream otemp;
    std::fstream temp;
    std::fstream file;
    std::string cmd,newname,dispname,subcmd;
    char sep('\0'),choice('\0');
    int low(0),high(0);
    int i(1);
    do
    {
        file.open("songscache.tmp", std::ios::in|std::ios::binary);
        file.seekg(0);
        file.clear();
        subcmd = "";
        system("cls");
        for(i = 1; file >> reader && !file.eof(); ++i)
        {
            dispname=reader.file;
            if(dispname.find_last_of('.') != std::string::npos)
                dispname.erase(dispname.find_last_of('.'));
            std::cout << std::right << std::setfill('0') << std::setw(4) << i << ".  " << dispname << std::endl;
        }
        std::cout << std::setfill(' ');
        file.clear();
        file.seekg(0);
        std::cout << std::endl << std::endl;
        std::cout << "Commands:" << std::endl
                  << "__________" << std::endl
                  << "add     " << rightarrow << "Add selection to queue" << std::endl
                  << "del     " << rightarrow << "Deletes selection from disk" << std::endl
                  << "ren     " << rightarrow << "Renames selection" << std::endl
                  << "refresh " << rightarrow << "Refresh list" << std::endl
                  << "back    " << rightarrow << "Back to main menu" << std::endl;
        std::cin >> cmd;
        if(strtoint(cmd) > 0 && strtoint(cmd) < i)
        {
            for(int z(0); z<strtoint(cmd); ++z)
                file >> reader;
            file.seekg(0);
            file.clear();
            do
            {
                system("cls");
                subcmd.clear();
                showmeta(reader);
                std::cout << std::endl << std::endl << std::endl
                          << "Commands:" << std::endl
                          << "_________" << std::endl;
                std::cout << "play  " << rightarrow << "Play Now(w/o adding in a queue)" << std::endl
                          << "next  " << rightarrow << "Play next" << std::endl
                          << "queue " << rightarrow << "Add to queue" << std::endl
                          << "padd  " << rightarrow << "Add to playlist" << std::endl
                          << "back  " << rightarrow << "Back to menu" << std::endl;
                std::cin  >> subcmd;
                ignoreline();

                if(subcmd == "play")
                {

                    if(P.IsPlaying())
                        P.Stop();
                    P.Load(reader);
                    if(P.IsValid())
                        P.Play();

                }
                if(subcmd == "next")
                    P.Pushnext(strtoint(cmd), "songscache.tmp");
                if(subcmd == "queue")
                    P.Push(strtoint(cmd),"songscache.tmp");
                if(subcmd == "padd")
                {
                    int i=0;
                    bool any(false);
                    _finddata_t found;
                    std::map <int, std::string> plist;
                    std::string pname;
                    int x = _findfirst("playlist\\*.pls", &found);
                    if(x != -1)
                    {
                        any = true;
                        pname = found.name;
                        pname.erase(pname.find_last_of('.'));
                        plist[++i]=pname;
                        int more=_findnext(x, &found);
                        while(more==0)
                        {
                            pname = found.name;
                            pname.erase(pname.find_last_of('.'));
                            plist[++i]=pname;
                            more = _findnext(x, &found);
                        }
                        _findclose(x);
                    }
                    if(!any)
                    {
                        std::cout << "No playlist to show!!" << std::endl;
                        getch();
                        continue;
                    }
                    else
                    {
                        for(std::map <int,std::string>::iterator j(plist.begin()); j!=plist.end(); ++j)
                        {
                            std::cout << j->first << ". " << j->second << std::endl;
                        }
                    }
                    if(std::cin.eof())
                        std::cout << "Enter number to be added:";
                    std::cin >> subcmd;
                    ignoreline();
                    if(strtoint(subcmd)>0&&strtoint(subcmd)<=i)
                    {
                        otemp.open(("playlist\\"+plist[strtoint(subcmd)]+".pls").c_str(),std::ios::out|std::ios::binary|std::ios::app);
                        otemp << reader;
                        otemp.close();
                        std::cout << "Added to " << plist[strtoint(subcmd)] << std::endl;
                        getch();
                    }
                    else if(subcmd=="back")
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Invalid input !!!" << std::endl << "Create new playlist ?(Y/N)";
                        do
                        {
                            choice=getch();
                            if(choice=='y'||choice=='Y')
                            {
                                std::cout << "Enter the name of the playlist";
                                getline(std::cin,newname);
                                otemp.open(("playlist\\"+newname+".pls").c_str(),std::ios::out|std::ios::binary);
                                otemp << reader;
                                std::cout << "Added to " << newname << std::endl;
                                getch();
                                break;
                            }
                            else if(choice=='N'||choice=='n')
                                break;
                        }
                        while(true);

                    }
                }
            }
            while(subcmd!="back");
        }
        else if(cmd == "refresh")
        {
            std::thread t2(getall);
            ignoreline();
            P.Stop();
            P.Flush();
            t2.join();
        }
        else if(cmd == "add")
        {
            if(std::cin.get() == '\n')
                std::cout << "Enter nos to be added:";
            std::vector<int> qlist;
            getline(std::cin, cmd);
            std::stringstream input(cmd);
            while(input >> low)
            {
                high = low;
                if(input >> sep)
                {
                    if(sep == '-')
                    {
                        if(!(input >> high))
                            high=low;
                        if(isdigit(input.get()))
                            input.unget();
                    }
                    else if(isdigit(sep))
                        input.unget();
                    else if(sep == ',')
                        /*empty*/;
                    else
                        input.ignore(std::numeric_limits<int>::max(), '\n');
                }
                int _high = std::max(low, high);
                int _low  = std::min(low, high);
                for(int k=_low; k<=_high; ++k)
                    qlist.push_back(k);
                if(input >> sep)
                    if(isdigit(sep))
                        input.unget();
            }

            if(!qlist.empty())
            {
                qlist.erase(std::remove_if(qlist.begin(), qlist.end(), outofrange(0, i)), qlist.end());
                std::sort(qlist.begin(),qlist.end());
                qlist.erase(std::unique(qlist.begin(),qlist.end()),qlist.end());
                std::cout << "Added ";
                for(std::vector<int>::iterator k(qlist.begin()); k!=qlist.end(); ++k)
                {
                    P.Push(*k, "songscache.tmp");
                    std::cout << *k << ' ';
                }
                std::cout << "to play queue";
                getch();
            }


        }
        else if(cmd=="del")
        {
            if(std::cin.get() == '\n')
                std::cout << "Enter nos to be deleted:";
            std::vector<int> qlist;
            getline(std::cin, cmd);
            std::stringstream input(cmd);
            while(input >> low)
            {
                high = low;
                if(input >> sep)
                {
                    if(sep == '-')
                    {
                        if(!(input >> high))
                            high=low;
                        if(isdigit(input.get()))
                            input.unget();
                    }
                    else if(isdigit(sep))
                        input.unget();
                    else if(sep == ',')
                        /*empty*/;
                    else
                        input.ignore(std::numeric_limits<int>::max(), '\n');
                }
                int _high = std::max(low, high);
                int _low  = std::min(low, high);
                for(int k=_low; k<=_high; ++k)
                    qlist.push_back(k);
                if(input >> sep)
                    if(isdigit(sep))
                        input.unget();
            }

            if(!qlist.empty())
            {
                std::sort(qlist.begin(),qlist.end());
                qlist.erase(std::unique(qlist.begin(),qlist.end()),qlist.end());
                auto iter = std::remove_if(qlist.begin(), qlist.end(), outofrange(0, i));
                qlist.erase(iter, qlist.end());
                std::cout << "Selected ";
                std::copy(qlist.begin(), qlist.end(), std::ostream_iterator<int>(std::cout, ", "));
                std::cout << "\b\b " << std::endl  << "Press enter to confirm";
            }
            char ch = getch();
            if(ch == '\r' || ch == '\n')
            {
                std::cout << std::endl << "Removed ";
                for(std::vector<int>::iterator j(qlist.begin()); j!=qlist.end(); ++j)
                {
                    int pos=0;
                    otemp.open("cache.tmp",std::ios::out|std::ios::binary);
                    file.open("songscache.tmp",std::ios::out|std::ios::binary);
                    file.seekg(0);
                    file.clear();
                    while(file >> deleter && !file.eof())
                    {
                        ++pos;
                        if(pos != *j)
                            otemp << deleter;
                        else
                        {
                            remove(deleter.filepath().c_str());
                            P.Pop(deleter.filepath());
                        }
                    }
                    otemp.close();
                    file.close();
                    remove("songscache.tmp");
                    rename("cache.tmp", "songscache.tmp");
                    std::cout << *j << ' ';
                }
                std::cout << "From the disk...";
                getch();
            }
        }
        else if(cmd == "ren")
        {
            songpath_t writer, tmp;
            int no(0);
            size_t endpos(0);
            std::string name;
            if(std::cin.get() == '\n')
                std::cout << "Enter the number to be renamed: ";
            else
                std::cin.unget();
            std::cin >> no;

            if(!(no > 0 && no <= i))
            {
                std::cout << "Invalid Number!";
                getch();
                continue;
            }

            if(std::cin.get()== '\n')
                std::cout << "Enter new name: ";

            std::getline(std::cin, name);
            endpos = name.find_last_of('.');
            if(endpos != std::string::npos)
            {
                if(name.substr(endpos, name.size()) != ".wav")
                    name += ".wav";
            }
            else
                name += ".wav";
            for(int z(0); z < no; ++z)
                file >> reader;
            writer = reader;
            rename(reader.filepath().c_str(), (reader.path + name).c_str());
            for(int z(0); z < no-1; ++z)
                file >> tmp;
            writer.file = name;
            std::ofstream fout("tmp", std::ios::out | std::ios::binary);
            file.seekg(0);
            for(int z(1); file >> tmp && !file.eof(); ++z)
            {
                if(z == no)
                    fout << writer;
                else
                    fout << tmp;
            }
            std::cout << "Renamed " << reader.file << " to " << name;
            fout.close();
            P.Stop();
            P.Flush();
            std::thread task(getall);
            task.join();
            getch();
        }

        else if(cmd=="back")
            mControl = HOME;
    }
    while(mControl == ALLSONGS);
}

void playlists()
{
    system("COLOR 9A");
    std::fstream fin, all("songscache.tmp",std::ios::in|std::ios::binary);
    std::string cmd, filename, dispname, subcmd, subdispname;
    std::fstream fout,temp;
    songpath_t reader;
    std::map <int, std::string> Pmap;
    int i(0), x(0), more(0);
    _finddata_t found;
    do
    {
        i = 0;
        Pmap.clear();
        system("cls");
        if(!(pathexists("playlists\\")))
            system("mkdir playlists");
        x = _findfirst("playlists\\*.pls", &found);
        if(x!=-1)
        {
            dispname = found.name;
            dispname.erase(dispname.find_last_of('.'));
            Pmap[++i] = dispname;
            more = _findnext(x, &found);
            while(more == 0)
            {
                dispname = found.name;
                dispname.erase(dispname.find_last_of('.'));
                Pmap[++i] = dispname;
                more = _findnext(x, &found);
            }
        }
        _findclose(x);
        for(std::map <int, std::string>::iterator displayer(Pmap.begin()); displayer!=Pmap.end(); ++displayer)
        {
            std::cout << displayer->first << ".  " << displayer->second << std::endl;
        }
        if(Pmap.size() == 0)
            std::cout << "No Playlists to show!!!" << std::endl;
        std::cout << std::endl << std::endl;
        std::cout << "Commands" << std::endl
                  << "________" << std::endl << std::endl
                  << "play   " << rightarrow << "play list" << std::endl
                  << "create " << rightarrow << "creates a playlist" << std::endl
                  << "remove " << rightarrow << "removes a playlist" << std::endl
                  << "rename " << rightarrow << "renames list" << std::endl
                  << "back   " << rightarrow << "back to menu" << std::endl;
        std::cin >> cmd;
        if(strtoint(cmd) > 0 && strtoint(cmd) <= i)
        {
            ignoreline();
            bool any(false);
            int num(0);
            do
            {
                system("cls");
                num=0;
                fin.open(("playlists\\" + Pmap[strtoint(cmd)] + ".pls").c_str(), std::ios::in|std::ios::binary);
                while(fin >> reader && !fin.eof())
                {
                    any = true;
                    dispname = reader.file;
                    dispname.erase(dispname.find_last_of('.'));
                    std::cout << ++num << ". " << dispname << std::endl;
                }
                fin.close();
                if(!any)
                {
                    std::cout << "Empty Playlist";
                }
                std::cout << std::endl << std::endl
                          << "Commands" << std::endl
                          << "_________" << std::endl << std::endl
                          << "add    " << rightarrow << " add songs" << std::endl
                          << "remove " << rightarrow << " remove songs" << std::endl
                          << "play   " << rightarrow << " play current playlist" << std::endl
                          << "delete " << rightarrow << " deletes current playlist" << std::endl
                          << "back   " << rightarrow << " back to playlists" << std::endl
                          << std::endl;
                std::cin >> subcmd;
                if(subcmd == "add")
                {
                    system("cls");
                    std::string nos;
                    ignoreline();
                    int low(0),high(0);
                    all.clear();
                    all.seekg(0);
                    std::vector<int>list;
                    char sep('\0');
                    for(i=1; all >> reader && !all.eof(); ++i)
                    {
                        subdispname=reader.file;
                        subdispname.erase(subdispname.find_last_of('.'));
                        std::cout << i << ". " << subdispname << std::endl;
                    }
                    all.seekg(0);
                    std::cout << std::endl << "Enter nos to be added:";
                    std::vector<int> qlist;
                    getline(std::cin, nos);
                    std::stringstream input(nos);
                    while(input >> low)
                    {
                        high = low;
                        if(input >> sep)
                        {
                            if(sep == '-')
                            {
                                if(!(input >> high))
                                    high=low;
                                if(isdigit(input.get()))
                                    input.unget();
                            }
                            else if(isdigit(sep))
                                input.unget();
                            else if(sep == ',')
                                /*empty*/;
                            else
                                input.ignore(std::numeric_limits<int>::max(), '\n');
                        }
                        int _high = std::max(low, high);
                        int _low  = std::min(low, high);
                        for(int k=_low; k<=_high; ++k)
                            qlist.push_back(k);
                        if(input >> sep)
                            if(isdigit(sep))
                                input.unget();
                    }

                    if(!qlist.empty())
                    {
                        fout.open(("playlists\\" + Pmap[strtoint(cmd)] + ".pls").c_str(), std::ios::out|std::ios::binary|std::ios::app);
                        qlist.erase(std::remove_if(qlist.begin(), qlist.end(), outofrange(0, i)), qlist.end());
                        std::sort(qlist.begin(), qlist.end());
                        qlist.erase(std::unique(qlist.begin(), qlist.end()), qlist.end());
                        std::cout << "Added ";
                        for(std::vector<int>::iterator k(qlist.begin()); k != qlist.end(); ++k)
                        {
                            all.clear();
                            all.seekg(0);
                            std::cout << *k << ' ';
                            for(int z(0); z < *k; ++z)
                            {
                                all >> reader;
                            }
                            fout << reader;
                        }
                    }
                    fout.close();
                    std::cout << "to " << Pmap[strtoint(cmd)] << "...";
                    getch();
                }
                else if(subcmd=="play")
                {
                    ignoreline();
                    P.Flush();
                    for(int i(1); i <= num; ++i)
                    {
                        P.Push(i, Pmap[strtoint(cmd)]+".pls");
                    }
                    P.Repeat();
                }
                else if (subcmd == "remove")
                {
                    std::string str;
                    int low(0), high(0);
                    char sep('\0');
                    if(std::cin.get() == '\n')
                        std::cout << "Enter nos to be removed:";
                    std::vector<int> qlist;
                    getline(std::cin, str);
                    std::stringstream input(str);
                    while(input >> low)
                    {
                        high = low;
                        if(input >> sep)
                        {
                            if(sep == '-')
                            {
                                if(!(input >> high))
                                    high=low;
                                if(isdigit(input.get()))
                                    input.unget();
                            }
                            else if(isdigit(sep))
                                input.unget();
                            else if(sep == ',')
                                /*empty*/;
                            else
                                input.ignore(std::numeric_limits<int>::max(), '\n');
                        }
                        int _high = std::max(low, high);
                        int _low  = std::min(low, high);
                        for(int k=_low; k<=_high; ++k)
                            qlist.push_back(k);
                        if(input >> sep)
                            if(isdigit(sep))
                                input.unget();
                    }

                    if(!qlist.empty())
                    {
                        qlist.erase(std::remove_if(qlist.begin(), qlist.end(), outofrange(0, num + 1)), qlist.end());
                        std::sort(qlist.begin(),qlist.end());
                        qlist.erase(std::unique(qlist.begin(),qlist.end()),qlist.end());
                        std::cout << "Removed ";
                        std::ofstream tempout;
                        int iteration(0);
                        for(std::vector<int>::iterator k(qlist.begin()); k!=qlist.end(); ++k)
                        {
                            fin.open(("playlists\\" + Pmap[strtoint(cmd)] + ".pls").c_str(),std::ios::in|std::ios::binary);
                            tempout.open("playlists\\tmp", std::ios::binary|std::ios::out);
                            fin.clear();
                            fin.seekg(0);
                            int pos(0);
                            while(fin >> reader && !fin.eof())
                            {
                                ++pos;
                                if(pos != *k - iteration)
                                    tempout << reader;
                            }
                            std::cout << *k << ' ';
                            ++iteration;
                            tempout.close();
                            fin.close();
                            remove(("playlists\\" + Pmap[strtoint(cmd)] + ".pls").c_str());
                            rename("playlists\\tmp", ("playlists\\" + Pmap[strtoint(cmd)] + ".pls").c_str());
                        }
                        std::cout << "from " << Pmap[strtoint(cmd)];
                        getch();
                    }
                }
                else if(subcmd == "delete")
                {
                    remove(("playlists\\" + Pmap[strtoint(cmd)] + ".pls").c_str());
                }
            }
            while(subcmd!="back");
        }
        else if(cmd == "remove")
        {
            int del;
            if(std::cin.get() == '\n')
                std::cout << "Enter number to be deleted: ";
            else
                std::cin.unget();
            std::cin >> del;
            remove(("playlists\\" + Pmap[del] + ".pls").c_str());
        }
        else if(cmd=="create")
        {
            if(std::cin.get() == '\n')
                std::cout << "Enter name to be created:";
            getline(std::cin,cmd);
            if(!(exists("playlists\\" +cmd+ ".pls")))
            {
                std::fstream creator(("playlists\\"+cmd+".pls").c_str(),std::ios::out|std::ios::binary);
                if(!creator)
                {
                    std::cout << "Error in creation";
                    getch();
                }
                else
                {
                    std::cout << "Created " << cmd;
                    getch();
                    creator.close();
                }
            }
            else
            {
                std::cout << "Already exists!!!";
                getch();
            }
            subcmd.clear();
        }
        else if(cmd == "rename")
        {
            int no(0);
            std::string name;

            if(std::cin.get() == '\n')
                std::cout << "Enter number to be renamed";
            else
                std::cin.unget();
            std::cin >> no;
            if(!(no > 0 && no <= i))
            {
                std::cout << "Invalid number!";
                getch();
                continue;
            }
            if(std::cin.get() == '\n')
                std::cout << "Enter new name: ";
            else
                std::cin.unget();
            std::getline(std::cin, name);

            rename(("playlists\\" + Pmap[no] + ".pls").c_str(),("playlists\\" + name + ".pls").c_str());
            std::cout << "Renamed" << Pmap[no] << " to " << name;
            Pmap[no] = name;
            getch();
        }
        else if (cmd == "back")
            mControl = HOME;
        else
            ignoreline();
    }
    while(mControl == PLAYLISTS);
}

void settings()
{
    system("COLOR 0C");
    std::string path, cmd;
    int i(0), num(0);
    std::fstream fin;
    std::fstream fout,temp;

    do
    {

        i=0;
        fin.open("libraries.cfg", std::ios::in);
        getline(fin, path, ':');
        if(path != "PATH")
            kill(2);
        system("cls");
        std::cout << "Libraries" << std::endl
                  << "_________" << std::endl;
        while(getline(fin, path, ';'))
        {
            std::cout << ++i << ". " << path << std::endl;
        }
        fin.close();

        std::cout << std::endl << std::endl << std::endl << std::endl
                  << "Commands" << std::endl
                  << "________" << std::endl << std::endl
                  << "add    " << rightarrow << " add library" << std::endl
                  << "remove " << rightarrow << " remove library" << std::endl
                  << "back   " << rightarrow << " back to menu" << std::endl;
        std::cin >> cmd;

        if(cmd == "add")
        {
            fout.open("libraries.cfg",std::ios::out|std::ios::app);
            std::fstream check("libraries.cfg",std::ios::in);
            getline(check, path, ':');
            check.close();
            if(path != "PATH")
                kill(2);
            path.clear();
            if(std::cin.get() == '\n')
                std::cout << "Enter Path: ";
            getline(std::cin, path);
            replace(path.begin(), path.end(), '/', '\\');
            if(path.back() != '\\')
                path += "\\";
            if(pathexists(path))
            {
                fout << path << ';';
                std::cout << path << " added to library...";
                getall();
            }
            else
                std::cout << "Invalid Path";
            getch();
            fout.close();
        }

        else if(cmd=="remove")
        {
            fin.open("libraries.cfg",std::ios::in);
            getline(fin, path, ':');
            if(path != "PATH")
                kill(2);
            if(std::cin.get() == '\n')
                std::cout << "Enter Number: ";
            if(!(std::cin >> num))
            {
                std::cout << "Invalid Entry";
                getch();
                ignoreline();
                continue;
            }
            temp.open("temp",std::ios::out);
            temp << "PATH:";
            ignoreline();
            for(int j = 1; getline(fin, path, ';'); ++j)
            {
                if(j != num)
                {
                    temp << path << ';';
                }
            }
            temp.close();
            fin.close();
            remove("libraries.cfg");
            rename("temp", "libraries.cfg");
        }
        else if(cmd != "back")
            ignoreline();
    }
    while(cmd!="back");
}

void about()
{
    system("COLOR 0F");
    system("cls");
    std::cout << "CLAP: Console Lossless Audio Player" << std::endl
              << "Developed by Jyothish and Abhishek" << std::endl << std::endl;
    std::cout << "Press any key to return to menu...";
    getch();
}

#endif
