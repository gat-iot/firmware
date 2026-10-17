# Barometer Identification Regression

Run from the repository root with a host C++17 compiler (or Zig):

```sh
python test/host_gat562_barometer/run.py /path/to/zig
```

The runner extracts the actual BME_ADDR dispatch block from ScanI2CTwoWire.cpp
and compiles it with deterministic register responses. It tests with and without
the GAT562 board override. No copied implementation of the dispatch is used.

Checks cover BMP280 (0x58) and BME280 (0x60) at both 0x76 and 0x77, avoiding
secondary ID reads for those known IDs, the existing GAT562 BME280 startup
fallback, and the generic BME680, BMP085, DPS310, SPA06 and BMP3xx routes.
This does not exercise a physical I2C bus or sensor measurements.
