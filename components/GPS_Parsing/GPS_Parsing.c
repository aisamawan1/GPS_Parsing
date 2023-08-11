#include <stdio.h>
#include "GPS_Parsing.h"
#include "esp_log.h"
#include "string.h"

static const char *TAG = "GPS_PARSER";

bool validateChecksum(const char *nmea)
{
    // Extracting checksum from the NMEA sentence
    char receivedChecksum[3];
    int len = strlen(nmea);
    if (len < 7) {
        ESP_LOGE(TAG, "Invalid NMEA sentence: %s", nmea);
        return false;
    }
    strncpy(receivedChecksum, nmea + len - 2, 2);
    receivedChecksum[2] = '\0';

    // Calculated the checksum
    char calculatedChecksum[3] = {0};
    for (int i = 1; i < len - 3; i++) {
        calculatedChecksum[0] ^= nmea[i];
    }

    char checksumHex[3];
    sprintf(checksumHex, "%02X", calculatedChecksum[0]);

    // Compared the received checksum with the calculated checksum
    return (strcmp(receivedChecksum, checksumHex) == 0);
}


bool parsegpsdata(const char *nmea, GPSData *gpsData)
{
    char *token;
    char nmeaCopy[MAX_NMEA_LENGTH];
    strncpy(nmeaCopy, nmea, MAX_NMEA_LENGTH);

    strncpy(gpsData->nmeaIdentifier, nmeaCopy, 6);
    gpsData->nmeaIdentifier[6] = '\0';

    // Checking sentence identifier and validating checksum
    if (strncmp(nmeaCopy, "$GPGGA,", 7) != 0 || !validateChecksum(nmeaCopy)) {
        ESP_LOGE(TAG, "Invalid NMEA sentence: %s", nmea);
        return false;
    }

    // Extracting and storing the received checksum
    strncpy(gpsData->receivedChecksum, nmeaCopy + strlen(nmeaCopy) - 2, 2);
    gpsData->receivedChecksum[2] = '\0';

    // Parse time (hhmmss.sss format)
    token = strtok(nmeaCopy, ",");
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) < 10) {
        ESP_LOGE(TAG, "Invalid time data: %s", nmea);
        return false;
    }
    strncpy(gpsData->time, token, 10);
    gpsData->time[10] = '\0';

    // Parse latitude
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid latitude data: %s", nmea);
        return false;
    }
    double latDegrees = atof(token) / 100.0;
    int latMinutes = (int)latDegrees;
    double latSeconds = (latDegrees - latMinutes) * 60.0;
    gpsData->latitude = latMinutes + (latSeconds / 60.0);

    // Parse latitude hemisphere (N/S)
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid latitude hemisphere: %s", nmea);
        return false;
    }
    gpsData->latitudeHemisphere = token[0];

    // Parse longitude
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid longitude data: %s", nmea);
        return false;
    }
    double lonDegrees = atof(token) / 100.0;
    int lonMinutes = (int)lonDegrees;
    double lonSeconds = (lonDegrees - lonMinutes) * 60.0;
    gpsData->longitude = lonMinutes + (lonSeconds / 60.0);

    // Parse longitude hemisphere (E/W)
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid longitude hemisphere: %s", nmea);
        return false;
    }
    gpsData->longitudeHemisphere = token[0];

    // Parse fix quality
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid fix quality data: %s", nmea);
        return false;
    }
    gpsData->fixQuality = atoi(token);

    // Parse number of satellites
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid number of satellites data: %s", nmea);
        return false;
    }
    gpsData->numSatellites = atoi(token);

    // Parse horizontal dilution
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid horizontal dilution data: %s", nmea);
        return false;
    }
    gpsData->horizontalDilution = atof(token);

    // Parse altitude above mean sea level
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid altitude above mean sea level data: %s", nmea);
        return false;
    }
    gpsData->altitudeMsl = atof(token);

    // Parse altitude units
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid altitude units data: %s", nmea);
        return false;
    }
    gpsData->altitudeUnits = token[0];

    // Parse geoid separation
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid geoid separation data: %s", nmea);
        return false;
    }
    gpsData->geoidSeparation = atof(token);

    // Parse geoid separation units
    token = strtok(NULL, ",");
    if (token == NULL || strlen(token) == 0) {
        ESP_LOGE(TAG, "Invalid geoid separation units data: %s", nmea);
        return false;
    }
    gpsData->geoidSeparationUnits = token[0];

    return true;
}



