#include "Firmware.hpp"

int main() {
    std::string filePath = "firmware.bin";
    int transferLimit = 150'000;

    WorDfuUtils::Firmware f;
    const auto uploadRes = f.Upload(filePath, transferLimit);
    if(!uploadRes) {
        return -50;
    }
    return 0;
}
