#include "unity.h"
#include "GPS_Parsing.h"

void test_parsegpsdata_valid_data(void)
{
    const char *nmea = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";
    GPSData gpsData;

    TEST_ASSERT_TRUE(parsegpsdata(nmea, &gpsData));
    TEST_ASSERT_EQUAL_STRING("$GPGGA", gpsData.nmeaIdentifier);
}

void test_parsegpsdata_invalid_data(void)
{
    const char *nmea = "$GPGLL,1234.5678,N,9876.5432,W";
    GPSData gpsData;

    TEST_ASSERT_FALSE(parsegpsdata(nmea, &gpsData));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_parsegpsdata_valid_data);
    RUN_TEST(test_parsegpsdata_invalid_data);
    UNITY_END();

    return 0;
}

void app_main(void)
{
    main();
}


/*#include "GPS_Parsing.h"
#include <stdio.h>

int main()
{
    const char *nmea = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";
    GPSData gpsData;

    bool success = parsegpsdata(nmea, &gpsData);
    if (success) {
        printf("NMEA Sentence Identifier: %s\n", gpsData.nmeaIdentifier);
        printf("Time: %s\n", gpsData.time);
        printf("Latitude: %lf %c\n", gpsData.latitude, gpsData.latitudeHemisphere);
        printf("Longitude: %lf %c\n", gpsData.longitude, gpsData.longitudeHemisphere);
        printf("Fix Quality: %d\n", gpsData.fixQuality);
        printf("Number of Satellites: %d\n", gpsData.numSatellites);
        printf("Horizontal Dilution: %f\n", gpsData.horizontalDilution);
        printf("Altitude above mean sea level: %f %c\n", gpsData.altitudeMsl, gpsData.altitudeUnits);
        printf("Geoid Separation: %f %c\n", gpsData.geoidSeparation, gpsData.geoidSeparationUnits);
        printf("Received Checksum: %s\n", gpsData.receivedChecksum);
    }

    return 0;
}

void app_main(void)
{
    main();
}
*/


