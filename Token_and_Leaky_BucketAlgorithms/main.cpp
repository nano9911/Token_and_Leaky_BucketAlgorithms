/**
 * @Author Adnan Omar Hleihel (123423)
 * @Author Weaam Akram Alrbeiqi (126928)
 */


#include "TokenBucketAlgorithm-functions.h"
#include "LeakyBucketAlgorithm.h"

void TokenBucketAlgorithm_Run() {
    uint64_t b_sz=0, b_tr=0, q_sz=0, q_par=0, p_mxsz=0;

    std::cout << "Maximum packet arrival rate :\t"; std::cin >> q_par;
    std::cout << "Maximum packet size :\t"; std::cin >> p_mxsz;
    std::cout << "Size of the queue :\t"; std::cin >> q_sz;
    std::cout << "Token rate :\t"; std::cin >> b_tr;
    std::cout << "Bucket capacity :\t"; std::cin >> b_sz;
    std::cout << "\n\n";

    TokenBucketAlgorithm tba(b_sz, b_tr, q_sz, q_par, p_mxsz);
    tba.Run();
    tba.print();
}

void Leaky_Bucket_Algorithm_Run() {
    std::cout << "Maximum packet arrival rate:\t";
    std::cin >> max_packets;
    std::cout << "Maximum packet size:\t";
    std::cin >> max_psize;
    std::cout << "Size of the queue:\t";
    std::cin >> s_queue;
    std::cout << "Departure rate:\t";
    std::cin >> dp_rate;
    std::cout << std::endl;
    Leaky_Bucket_Algorithm();
}


int main(void) {
    int choice = 0;
    std::cout << "[1] Leaky Bucket Algorithm.\n";
    std::cout << "[2] Token Bucket Algoruthm.\n";
    std::cout << "Choose which algorithm you want:\t";

    std::cin >> choice;
    if (choice == 1)
        Leaky_Bucket_Algorithm();
    else if (choice == 2)
        TokenBucketAlgorithm_Run();
}
