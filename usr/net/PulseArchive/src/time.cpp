#include <chrono>
#include <ctime>
#include <string>

/**
 * Time for the logging
 */
std::string get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);

    std::tm* ptm = std::localtime(&tt);

    char buffer[20];
    std::strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", ptm);
    return std::string(buffer);
}
