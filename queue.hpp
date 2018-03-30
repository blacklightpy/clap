/**************************************
** WAVPLAYER Queue Class Header File **
***************************************/

#ifndef CBSE_WAVPLAYER_QUEUE_HPP
#define CBSE_WAVPLAYER_QUEUE_HPP

class Queue
{
    int total;
    song_t * start, * end;
public:
    Queue();
    ~Queue();
    void Pushnext(int const&, std::string);
    void Push(int const&, std::string);
    void Pop(int const&);
    void Pop(std::string const&);
    void Display();
    void Flush();
    void Move(int const&, int const&);
    songpath_t Get(int const&);
    int size()
    {
        return total;
    }
};



#endif
