/**
 * @Author Adnan Omar Hleihel (123423)
 */

#pragma once
#include "TokenBucketAlgorithm.h"

/**
 * @brief Construct a new token bucket algo::token bucket algo object
 * Initialize variables and structures.
 *
 * @param b_sz bucket maximum size
 * @param b_tr bucket tocken rate
 * @param q_sz queue size
 * @param q_par queue maximum packet arrival eate
 * @param p_mxsz packets maximum size
 */
TokenBucketAlgorithm::TokenBucketAlgorithm(uint64_t b_sz, uint64_t b_tr,
    uint64_t q_sz, uint64_t q_par,
    uint64_t p_mxsz) :
    Bucket{ b_sz, b_sz, b_tr },
    Queue{ q_sz, q_sz, 0, q_par },
    ul64PacketMaxSize(p_mxsz)   {

    for (int i = 0; i < TICKS; i++)
        Tick[i] = { 0,0,"",0,0,0 };
}

uint64_t TokenBucketAlgorithm::GetRandomPacketArrivalRate() {
    std::random_device Rate;
    return (((Rate() % this->Queue.ul64MaxPacketArrivalRate) +
        this->Queue.ul64MaxPacketArrivalRate) %
        this->Queue.ul64MaxPacketArrivalRate + 1);
}

void TokenBucketAlgorithm::DropPacket() {
    delete this->ArrivedPacket;
    this->iDroppedPackets++;
}

/**
 * @brief generate (allocate) new packet which represents an arrived
 * packet, with random size, and initializing the arrival time variable.
 */
void TokenBucketAlgorithm::GenRandomArrivedPacket(int i) {
    std::random_device R;
    this->ArrivedPacket = NewPacket();

    this->ArrivedPacket->ul64Size = ((((R() % this->ul64PacketMaxSize) + this->ul64PacketMaxSize) % this->ul64PacketMaxSize) + 1);
    this->iArrivedPackets++;

    this->Tick[i].iArrivedPackets++;
    this->Tick[i].sArrivedPackets += std::to_string(this->ArrivedPacket->ul64Size);
}

/**
 * @brief check if the queue is full or have a space to add the arrived packet,
 * if it's then add it to the queue by assigning the last_packet_in_queue to
 * the arrived packet. If it's not then drop the packet by freeing the new
 * allocated space for it.
 */
void TokenBucketAlgorithm::AddPacketToQueue() {
    if (this->Queue.ul64FreeSpace < this->ArrivedPacket->ul64Size) {
        DropPacket();
    }

    else {
        if (this->Queue.ul64CurrentPackets == 0) {
            this->LastPacketInQueue = this->ArrivedPacket;
            this->FirstPacketInQueue = this->ArrivedPacket;
        }

        else {
            this->ArrivedPacket->next = this->LastPacketInQueue;
            this->LastPacketInQueue->before = this->ArrivedPacket;
            this->LastPacketInQueue = this->ArrivedPacket;

        }

        this->Queue.ul64CurrentPackets++;
        this->Queue.ul64FreeSpace -= this->LastPacketInQueue->ul64Size;
    }
}

/**
 * @brief send the first packet in the queue (FIFO queue) if there's enouph tockens
 * in the bucket equal to the packet size, then return TRUE.
 * But, if there's not enouph tokens, return false to stop trying and wait for the
 * next tick to try again.
 *
 * @return true --> packet sent
 * @return false --> not enouph token to send packet
 */
bool TokenBucketAlgorithm::SendPacket() {
    if (this->FirstPacketInQueue != nullptr &&
        this->Queue.ul64CurrentPackets > 0 &&
        this->Bucket.ul64CurrentTokens >= this->FirstPacketInQueue->ul64Size) {

        struct Packet* temp = this->FirstPacketInQueue;
        this->FirstPacketInQueue = this->FirstPacketInQueue->before;

        /* modify queue available space and bucket tockens count */
        this->Queue.ul64FreeSpace += temp->ul64Size;
        this->Bucket.ul64CurrentTokens -= temp->ul64Size;

        delete temp;
        this->iDepartedPackets++;
        this->Queue.ul64CurrentPackets--;
        return true;
    }

    return false;
}

void TokenBucketAlgorithm::OneTick(int e) {
    uint64_t rate = GetRandomPacketArrivalRate();
    int x = e - 1;

    for (uint64_t i = 0; i < rate; i++) {
        if (i > 0)
            this->Tick[e].sArrivedPackets.append(",");

        SendPacket();

        GenRandomArrivedPacket(e);
        if (this->Queue.ul64FreeSpace > 0) {
            AddPacketToQueue();
        }

        SendPacket();
    }

    this->Tick[e].ul64BucketTockens = this->Bucket.ul64CurrentTokens;
    this->Tick[e].ul64QueuePackets = this->Queue.ul64CurrentPackets;
    this->Tick[e].iDepartedPackets = this->iDepartedPackets - this->Tick[x].iDepartedPackets;
    this->Tick[e].iDroppedPackets = this->iDroppedPackets - this->Tick[x].iDroppedPackets;

    this->Tick[e].sArrivedPackets = "(" + this->Tick[e].sArrivedPackets + ")";

    this->Bucket.ul64CurrentTokens += this->Bucket.ul64TokenRate;
}

/**
 * @brief Run the simulation of the algorithm, initialize the startup time
 * and the first tick. Then start a loop of the number of TICKS, in each tick
 * call OneTick() and add the new info to the appropriate tick row of the table.
 */
void TokenBucketAlgorithm::Run() {
    for (int i = 1; i < TICKS + 1; i++)
        OneTick(i);
}


void TokenBucketAlgorithm::print() {
    for (int i = 1; i < TICKS + 1; i++) {
        std::cout << "Tick #\t" << i <<
            "\nArrived packets:\t" << this->Tick[i].iArrivedPackets << this->Tick[i].sArrivedPackets <<
            "\nDeparted packets:\t" << this->Tick[i].iDepartedPackets <<
            "\nDropped packets:\t" << this->Tick[i].iDroppedPackets <<
            "\nPackets in Queue:\t" << this->Tick[i].ul64QueuePackets <<
            "\nNo. of tokens:\t\t" << this->Tick[i].ul64BucketTockens << std::endl << std::endl;
    }
}
