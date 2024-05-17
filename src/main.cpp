#include "Firmware.hpp"

using namespace WorDfuUtil;

int main() {
    std::string filePath = "firmware.bin";
    int transferLimit = 150'000;

    const auto uploadRes = Firmware::Upload(filePath, transferLimit);
    if(!uploadRes) {
        return -50;
    }
    return 0;
}
