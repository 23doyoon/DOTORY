/*
 * PacketManager.cpp
 *
 *  Created on: 2018. 5. 18.
 *      Author: doyoon
 */

#include "PacketManager.h"

PacketManager::PacketManager(){

}
void PacketManager::recvPacket(struct Packet packet){
	this->packet = packet;
}

State PacketManager::get_packet_state(){
	State ret = {packet.flags, packet.channel_num};
	return ret;
}


PacketManager::~PacketManager() {
	// TODO Auto-generated destructor stub
}

