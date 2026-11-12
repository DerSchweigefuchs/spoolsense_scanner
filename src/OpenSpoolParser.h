#pragma once

#include <cstdint>
#include <cstddef>

struct OpenSpoolData {
    char protocol[16];
    char version[8];
    char material[32];
    char color_hex[8];    // "FFAABB" (no #)
    char brand[64];
    int16_t min_temp;
    int16_t max_temp;
    // Optional extension fields (0 = absent). Key names follow the Snapmaker U1
    // extended firmware (bed_min_temp, bed_max_temp, diameter, weight); density
    // and spoolman_id are SpoolSense additions. Readers that only know the
    // seven standard keys skip them.
    int16_t bed_min_temp;
    int16_t bed_max_temp;
    float diameter_mm;
    float density;
    float weight_g;        // full (nominal) filament weight
    int32_t spoolman_id;   // 0 = absent
    bool valid;
};

bool parseOpenSpool(const uint8_t* json, size_t len, OpenSpoolData& out);
