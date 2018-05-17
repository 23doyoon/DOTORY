#ifndef _PACKET_H_
#define _PACKET_H_
struct Packet{
        int flags;
        int fd;
        int channel_num;
        char msg_max[MSG_SIZE];
};

struct State{
        int state;
        int channel_num;
};

#endif
