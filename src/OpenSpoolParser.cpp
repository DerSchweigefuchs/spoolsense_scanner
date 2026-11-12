#include "OpenSpoolParser.h"
#include <ArduinoJson.h>
#include <cstring>

// Deserialize OpenSpool NDEF JSON payload into typed struct for rapid access
bool parseOpenSpool(const uint8_t* json, size_t len, OpenSpoolData& out) {
    memset(&out, 0, sizeof(out));
    out.valid = false;

    if (!json || len == 0) return false;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, json, len);
    if (err) return false;

    const char* protocol = doc["protocol"] | "";
    if (strcmp(protocol, "openspool") != 0) return false;  // reject non-openspool payloads early

    strncpy(out.protocol, protocol, sizeof(out.protocol) - 1);
    strncpy(out.version, doc["version"] | "1.0", sizeof(out.version) - 1);
    strncpy(out.material, doc["type"] | "", sizeof(out.material) - 1);
    strncpy(out.color_hex, doc["color_hex"] | "", sizeof(out.color_hex) - 1);
    strncpy(out.brand, doc["brand"] | "", sizeof(out.brand) - 1);

    // Writers disagree on types: the OpenSpool spec uses strings for the
    // temperatures, other firmwares write numbers. Accept both everywhere.
    auto num = [&doc](const char* key) -> float {
        JsonVariantConst v = doc[key];
        if (v.is<const char*>()) return (float)atof(v.as<const char*>());
        return v | 0.0f;
    };
    out.min_temp = (int16_t)num("min_temp");
    out.max_temp = (int16_t)num("max_temp");
    out.bed_min_temp = (int16_t)num("bed_min_temp");
    out.bed_max_temp = (int16_t)num("bed_max_temp");
    out.diameter_mm = num("diameter");
    out.density = num("density");
    out.weight_g = num("weight");
    out.spoolman_id = (int32_t)num("spoolman_id");

    out.valid = true;
    return true;
}
