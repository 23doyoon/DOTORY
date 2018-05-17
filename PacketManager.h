/*
 * PacketManager.h
 *
 *  Created on: 2018. 5. 18.
 *      Author: doyoon
 */
#ifndef PACKETMANAGER_H_
#define PACKETMANAGER_H_
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "ChannelManager.h"
#include "Packet.h"

#define MAX_PACKET_SIZE 1024
#define MAX_CHAT_LEN 1024

#define packet_add 0
#define packet_del 1
#define packet_chat 2
#define MSG_SIZE 1012

class PacketManager {
private:
	struct Packet packet;
public:

	PacketManager();
	void recvPacket(struct Packet packet);
	State get_packet_state();
	virtual ~PacketManager();
};

#endif /* PACKETMANAGER_H_ */
