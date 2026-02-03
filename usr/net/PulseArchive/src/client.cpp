#include <iostream>
#include <regex>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "global.h"

void send_pulse(const std::string serverIP, const std::string& payload, const int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIP.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        close(sock);
        return;
    }

    send(sock, payload.c_str(), payload.size(), 0);

    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    if (valread > 0) {
        std::cout << "Server Response: " << std::string(buffer, valread) << std::endl;
    }

    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage (LOG): ./client SERVER:PORT LOG MACHINEID LEVEL MODULE MSG" << std::endl;
        std::cout << "Usage (PING): ./client SERVER:PORT PING MACHINEID" << std::endl;
        std::cout << "Usage (STATUS): ./client SERVER:PORT STATUS MACHINEID UPDATEID STATUSMSG" << std::endl;
        return 1;
    }
    std::string serverAddr = argv[1];
    std::string type = argv[2];
    std::string machine = argv[3];
    std::string payload;
    std::string d = std::string(1, DELIMITER);

    /**
     * To separate the ip into addr and port
     */
    std::string serverIP = serverAddr.substr(0, serverAddr.find(":"));
    std::string serverPort = serverAddr.substr(serverIP.length()+1, serverAddr.find(":"));
    std::cout << serverIP;

    if (type == "LOG") {
        if (argc < 6) {
             std::cout << "Usage (LOG): ./client LOG MACHINEID LEVEL MODULE MSG" << std::endl;
             return 1;
        }
        std::string level = argv[4];
        std::string module = argv[5];
        std::string msg = argv[6];

        payload = type + d + machine + d + level + d + module + d + msg;
        std::cout << "Sending PulseLog: " << payload << std::endl;

    } else if (type == "PING") {

        payload = type + d + machine;
        std::cout << "Sending PulsePing: " << payload << std::endl;

    } else if (type == "STATUS") {
        if (argc < 5) {
             std::cout << "Usage (STATUS): ./client STATUS MACHINEID UPDATEID STATUSMSG" << std::endl;
             return 1;
        }
        std::string update_id = argv[4];
        std::string status_msg = argv[5];


        payload = type + d + machine + d + update_id + d + status_msg;
        std::cout << "Sending PulseStatus: " << payload << std::endl;

    } else {
        std::cerr << "Unknown pulse type: " << type << std::endl;
        return 1;
    }

    send_pulse(serverIP, payload, std::stoi(serverPort));

    return 0;
}
