#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <string.h>

#include "time.h"
#include "global.h"
std::map<std::string, std::string> last_machine_status;

std::string outputDir = "";
bool separateLogs;
int port;

void log_to_file(const std::string& machine, const std::string& line, const std::string mode) {
    if (separateLogs) {
        std::string filename = outputDir + machine + ".csv";
    }

    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[PulseMSG] ERROR: Can't write to " << filename << std::endl;
        return;
    }
    file << line << "\n";
}

void process_log(const std::string& machine, std::stringstream& ss) {
    std::string level, module, msg;

    std::getline(ss, level, DELIMITER);
    std::getline(ss, module, DELIMITER);
    std::getline(ss, msg);

    std::string timestamp = get_current_timestamp();

    std::string log_line = "LOG" + std::string(1, DELIMITER) + machine + std::string(1, DELIMITER) +
                           timestamp + std::string(1, DELIMITER) + level + std::string(1, DELIMITER) +
                           module + std::string(1, DELIMITER) + msg;

    log_to_file(machine, log_line);
    std::cout << "[PulseLog] Received LOG from " << machine << ": " << log_line << std::endl;
}

void process_ping(const std::string& machine, const int client_socket) {
    std::string timestamp = get_current_timestamp();
    const std::string status = "OK";

    std::string log_line = "PING" + std::string(1, DELIMITER) + machine + std::string(1, DELIMITER) +
                           timestamp + std::string(1, DELIMITER) + status;

    log_to_file(machine, log_line);
    std::cout << "[PulsePing] Received PING from " << machine << ". Logged: " << log_line << std::endl;

    const char* response = "PONG";
    send(client_socket, response, strlen(response), 0);
}

void process_status(const std::string& machine, std::stringstream& ss) {
    std::string update_id, status_message;

    std::getline(ss, update_id, DELIMITER);
    std::getline(ss, status_message);

    std::string current_status_key = update_id + DELIMITER + status_message;

    if (last_machine_status.count(machine) && last_machine_status[machine] == current_status_key) {
        std::cout << "[PulseStatus] Status from " << machine << " is unchanged. Ignoring." << std::endl;
        return;
    }

    last_machine_status[machine] = current_status_key;

    std::string timestamp = get_current_timestamp();

    std::string log_line = "STATUS" + std::string(1, DELIMITER) + machine + std::string(1, DELIMITER) +
                           timestamp + std::string(1, DELIMITER) + update_id + std::string(1, DELIMITER) +
                           status_message;

    log_to_file(machine, log_line);
    std::cout << "[PulseStatus] NEW Status received and recorded for " << machine << ": " << log_line << std::endl;
}


int main(int argc, char *argv[]) {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const int PORT = std::stoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    std::cout << "Pulse Server listening on port " << PORT << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        char buffer[2048] = {0};
        int valread = read(new_socket, buffer, 2048);

        if (valread > 0) {
            std::string data(buffer);
            std::stringstream ss(data);
            std::string type, machine;

            std::getline(ss, type, DELIMITER);
            std::getline(ss, machine, DELIMITER);

            if (type == "LOG") {
                process_log(machine, ss);
            } else if (type == "PING") {
                process_ping(machine, new_socket);
            } else if (type == "STATUS") {
                process_status(machine, ss);
            } else {
                std::cerr << "[PulseMSG] WARNING: Unknown message type: " << type << std::endl;
            }
        }
        close(new_socket);
    }
}
