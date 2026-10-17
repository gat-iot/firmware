#include <cassert>
#include <cstdint>
#include <iostream>

constexpr int BME_ADDR = 0x76;
constexpr int BME_ADDR_ALTERNATE = 0x77;
enum DeviceType { NONE, BMP_280, BME_280, BME_680, BMP_085, DPS310, SPA06, BMP_3XX };
struct Address { uint8_t address; };
struct ScanI2CTwoWire {
    struct RegisterLocation {
        Address address;
        int reg;
        RegisterLocation(Address a, int r) : address(a), reg(r) {}
    };
};
static int chipId, secondaryId, secondaryReads;
int getRegisterValue(ScanI2CTwoWire::RegisterLocation location, int)
{
    if (location.reg == 0xd0)
        return chipId;
    ++secondaryReads;
    return secondaryId;
}
void logFoundDevice(const char *, uint8_t) {}
DeviceType detect(uint8_t address, int id, int otherId = 0)
{
    chipId = id;
    secondaryId = otherId;
    secondaryReads = 0;
    Address addr{address};
    DeviceType type = NONE;
    int registerValue = 0;
    switch (address) {
#include "sensor_probe.inc"
    }
    return type;
}
int main()
{
    for (uint8_t address : {0x76, 0x77}) {
        for (int otherId : {0, 0x50, 0x60, 0xff}) {
            assert(detect(address, 0x58, otherId) == BMP_280);
            assert(secondaryReads == 0);
            assert(detect(address, 0x60, otherId) == BME_280);
            assert(secondaryReads == 0);
        }
    }
#ifdef GAT562_BME280_ADDRESS
    assert(detect(0x77, 0) == BME_280);
    assert(detect(0x77, 0xff) == BME_280);
#else
    assert(detect(0x77, 0x61) == BME_680);
    assert(detect(0x77, 0x55) == BMP_085);
    assert(detect(0x77, 0, 0x10) == DPS310);
    assert(detect(0x77, 0, 0x11) == SPA06);
#endif
    assert(detect(0x76, 0x61) == BME_680);
    assert(detect(0x76, 0x55) == BMP_085);
    assert(detect(0x76, 0, 0x10) == DPS310);
    assert(detect(0x76, 0, 0x11) == SPA06);
    assert(detect(0x76, 0xff, 0x50) == BMP_3XX);
    assert(detect(0x76, 0xff, 0x60) == BMP_3XX);
    std::cout << "PASS: BMP280/BME280 IDs at both addresses, fallback and other sensor routes\n";
}
