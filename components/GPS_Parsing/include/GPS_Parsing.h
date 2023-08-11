#ifndef GPS_PARSER_H
#define GPS_PARSER_H
#define MAX_NMEA_LENGTH 82

#include <stdbool.h>

typedef struct {
    char nmeaIdentifier[7];
    char time[11];
    double latitude;
    char latitudeHemisphere;
    double longitude;
    char longitudeHemisphere;
    int fixQuality;
    int numSatellites;
    float horizontalDilution;
    float altitudeMsl;
    char altitudeUnits;
    float geoidSeparation;
    char geoidSeparationUnits;
    char receivedChecksum[3];
} GPSData;

// Function to validate NMEA checksum
bool validateChecksum(const char *nmea);

// Function to parse GPS data
bool parsegpsdata(const char *nmea, GPSData *gpsData);

#endif // GPS_PARSER_H
