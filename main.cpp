#include "mbed.h"
#include "nRF24L01P.h"
#include "io-tea/node.h"

#include <string>

Serial wifi(PA_9, PA_10);
DigitalOut wifi_io1(PC_10);

void relayWifi(const std::string& data) {
    wifi_io1 = 0;
    wait(0.001);

    std::string buffer = '\0' + data;
    for (const char c: buffer)
        wifi.putc(c);

    wait(0.001);
    wifi_io1 = 1;
    wait(0.001);
}

int handlePeer(int pipe) {
    static char rxData[32];

    int count = 0;
    for(;radio.readable(pipe) && count < 8; ++count) {
        int rxBytes = radio.read(pipe, rxData, 32);
        if (rxBytes < 0)
            continue;
        relayWifi(std::string(rxData, rxData + rxBytes));
    }
    return count;
}

int main() {
    wifi.baud(115200);

    setupSerial("GATEWAY");
    setupGatewayRadio();

    time_t lastStatsTime = time(nullptr) - 1;
    int relayed[3] = {0};
    while (true) {
        if (lastStatsTime != time(nullptr)) {
            lastStatsTime = time(nullptr);

            pc.printf("====\r\n");
            for (int i = 0; i < 3; ++i)
                pc.printf("%d: %d packets relayed\r\n", i, relayed[i]);
        }

        for (int i = 0; i < 3; ++i) {
            relayed[i] += handlePeer(NRF24L01P_PIPE_P0 + i);
        }
    }
}
