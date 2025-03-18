#include <iostream>
#include <fstream>
#include <ctime>
#include <boost/tokenizer.hpp>
#include <set>

int randIP() {
    return rand() % 256;
}

void fillTxt(int num) {
    std::ofstream outputFile("IPs.txt");
    for (int i = 0; i < num; ++i) {
        outputFile << randIP() << "."
            << randIP() << "." << randIP()
            << "." << randIP() << std::endl;
    }
    outputFile.close();
}

int IPtoInt(const std::string& ip) {
    boost::tokenizer<boost::char_separator<char>> tokenizer(ip, boost::char_separator<char>("."));
    int ipInt = 0;
    int shift = 24;
    for (const auto& token : tokenizer) {
        ipInt |= std::stoi(token) << shift;
        shift -= 8;
    }
    return ipInt;
}

int main() {
    srand(time(0));
    fillTxt(1000);

    std::ifstream inputFile("IPs.txt");
    std::set<int> uniqueIPs;

    std::string ip;
    while (std::getline(inputFile, ip)) {
        int ipInt = IPtoInt(ip);
        uniqueIPs.insert(ipInt);
    }

    std::cout << "Total unique IPs: " << uniqueIPs.size() << std::endl;

    return 0;
}
