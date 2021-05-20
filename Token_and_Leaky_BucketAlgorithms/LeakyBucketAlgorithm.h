/**
 *	@Author Weaam Akram Alrbeiqi (126928)
 */

#pragma once
#include <iostream>
#include <string>
#include <queue>

std::queue<unsigned int> q_arr_packet; //queue to collect all the arrival packets
unsigned int packed_queue = 0;
unsigned int max_packets, max_psize, s_queue, dp_rate;
std::string arrived_packet;

void sendpacket(unsigned int* departed, unsigned int* n) {
    if (!q_arr_packet.empty() && q_arr_packet.front() <= (*n))
    {
        (*departed)++;
        (*n) -= q_arr_packet.front();
        packed_queue -= q_arr_packet.front();
        q_arr_packet.pop();
    }
}

bool addpacket()
{
    //Taking random value for packet size
    unsigned int packet_size = rand() % max_psize + 1;
    arrived_packet += std::to_string(packet_size) + ",";

    if (packet_size <= (s_queue - packed_queue))
    {
        q_arr_packet.push(packet_size);
        packed_queue += packet_size;
        return true;
    }

    else
        return false;
}

void Leaky_Bucket_Algorithm() {
    unsigned int num_packets;
    for (int s = 0; s < 15; s++) {
        unsigned int num_dropped = 0, departed = 0, n = dp_rate;
        arrived_packet.clear();

        // number of packets for each tick
        num_packets = rand() % max_packets + 1;

        for (unsigned int j = 0; j < num_packets; j++) {
            sendpacket(&departed, &n);
            if (!addpacket())
                num_dropped++;
            sendpacket(&departed, &n);
        }

        std::cout << "Tick #" << s + 1 << std::endl;
        std::cout << "Arrived packets(size):\t" << num_packets << "(" << arrived_packet << ")" << std::endl;
        std::cout << "Departed packets:\t" << departed << std::endl;
        std::cout << "Dropped packets:\t" << num_dropped << std::endl;
        std::cout << "Packets in queue:\t" << q_arr_packet.size() << "\n\n";
    }
}
