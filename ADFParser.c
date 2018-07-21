#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "ADFParser.h"

enum ADFPatternGainUnit stringToGainUnit(const char* str) {
    if(strncmp(str, "DBI", 3)) {
        return ADF_PATTERN_GAIN_UNIT_DBI;
    }
    else if(strncmp(str, "DBD", 3)) {
        return ADF_PATTERN_GAIN_UNIT_DBD;
    }
    else if(strncmp(str, "DBR", 3)) {
        return ADF_PATTERN_GAIN_UNIT_DBR;
    }
    else if(strncmp(str, "LIN", 3)) {
        return ADF_PATTERN_GAIN_UNIT_LIN;
    }
    else {
        printf("Unrecognized gain unit, defaulting to dBi\n");
        return ADF_PATTERN_GAIN_UNIT_DBI;
    }
}

enum ADFPatternCutType stringToCutType(const char* str) {
    if(str[0] == 'H') {
        return ADF_PATTERN_CUT_HORIZONTAL;
    }
    else if(str[0] == 'V') {
        return  ADF_PATTERN_CUT_VERTICAL;
    }
    else if(str[0] == 'A' && str[1] == 'Z') {
        return ADF_PATTERN_CUT_AZIMUTH;
    }
    else if(str[0] == 'E' && str[1] == 'L') {
        return ADF_PATTERN_CUT_ELEVATION;
    }
    else {
        return ADF_PATTERN_CUT_PHI;
    }
}

enum ADFPatternPolarizationType stringToPolarizationType(const char* str) {
    if(strncmp(str, "H/H", 3)) {
        return ADF_PATTERN_POLARIZATION_HORIZONTAL_HORIZONTAL;
    }
    else if(strncmp(str, "H/V", 3)) {
        return ADF_PATTERN_POLARIZATION_HORIZONTAL_VERTICAL;
    }
    else if(strncmp(str, "V/V", 3)) {
        return ADF_PATTERN_POLARIZATION_VERTICAL_VERTICAL;
    }
    else if(strncmp(str, "V/H", 3)) {
        return ADF_PATTERN_POLARIZATION_VERTICAL_HORIZONTAL;
    }
    else if(strncmp(str, "SLR", 3)) {
        return ADF_PATTERN_POLARIZATION_SLANT_RIGHT;
    }
    else if(strncmp(str, "SLL", 3)) {
        return ADF_PATTERN_POLARIZATION_SLANT_LEFT;
    }
    else if(strncmp(str, "RCP", 3)) {
        return ADF_PATTERN_POLARIZATION_RIGHT_HAND;
    }
    else if(strncmp(str, "LCP", 3)) {
        return ADF_PATTERN_POLARIZATION_LEFT_HAND;
    }
    else if(strncmp(str, "ETH", 3)) {
        return ADF_PATTERN_POLARIZATION_E_THETA;
    }
    else if(strncmp(str, "EPH", 3)) {
        return ADF_PATTERN_POLARIZATION_E_PHI;
    }
    else {
        printf("Unknown polarization type \"%s\", defaulting to H/H\n", str);
        return ADF_PATTERN_POLARIZATION_HORIZONTAL_HORIZONTAL;
    }
}

bool startsWith(const char* a, const char* b) {
    return strncmp(a, b, strlen(b) - 1) == 0;
}

bool copyStringProperty(const char* property, const char** data, char* dest, int size) {
    if(startsWith(*data, property)) {
        const char* strStart = strchr(*data, ',') + 1;
        const char* strEnd = strchr(*data, '\r');
        if(strEnd - strStart <= size) {
            printf("The property \"%s\" is too long\n", property);
            strEnd = strStart + size;
        }
        strncpy(dest, strStart, strEnd - strStart);
        *data = strchr(*data, '\n') + 1;
        return true;
    }
    else {
        return false;
    }
}

bool copyMultiPartStringProperty(const char* property, const char** data, char** output) {
    bool hasProperty = false;
    char buffer[800] = {0};
    int bufferLen = 0;

    int propLen = strlen(property);
    char* numberedProperty = (char*)malloc(propLen + 2);
    strcpy(numberedProperty, property);

    char num = '1';
    numberedProperty[propLen + 1] = num;
    
    while(copyStringProperty(numberedProperty, data, &buffer[bufferLen], 800 - bufferLen)) {
        num++;
        numberedProperty[propLen + 1] = num;
        bufferLen = strlen(buffer);
        hasProperty = true;
    }

    if(hasProperty) { 
        *output = (char*)malloc(bufferLen + 1);
        strcpy(*output, buffer);
        return true;
    }
    else {
        *output = NULL;
        return false;
    }
}

bool copyIntProperty(const char* property, const char** data, int* dest) {
    if(startsWith(*data, property)){
        const char* intStart = strchr(*data, ',') + 1;
        char* intEnd;
        double tempInt = strtol(intStart, &intEnd, 10);

        if(intEnd == intStart) {
            //Didn't read a number
            printf("Error in ADF file\n");
            return false;
        }
        *dest = tempInt;
        *data = strchr(*data, '\n') + 1;
        return true;
    }   
    else {
        return false;
    } 
}

bool copyRatioProperty(const char* property, const char** data, int* numerator, int* denominator) {
    if(startsWith(*data, property)){
        const char* numeratorStart = strchr(*data, ',') + 1;
        char* numeratorEnd;
        int tempNumerator = strtol(numeratorStart, &numeratorEnd, 10);

        if(numeratorEnd == numeratorStart) {
            //Didn't read a number
            printf("Error in ADF file\n");
            return false;
        }

        const char* denominatorStart = strchr(*data, '/') + 1;
        char* denominatorEnd;
        int tempDenominator = strtol(denominatorStart, &denominatorEnd, 10);
        if(denominatorStart == denominatorEnd) {
            printf("Error in ADF file\n");
            return false;
        }

        *numerator = tempNumerator;
        *denominator = tempDenominator;
        *data = strchr(*data, '\n') + 1;
        return true;
    }   
    else {
        return false;
    } 
}
bool copyDoubleProperty(const char* property, const char** data, double* dest) {
    if(startsWith(*data, property)){
        const char* doubleStart = strchr(*data, ',') + 1;
        char* doubleEnd;
        double tempDouble = strtod(doubleStart, &doubleEnd);

        if(doubleEnd == doubleStart) {
            //Didn't read a number
            printf("Error in ADF file\n");
            return false;
        }
        *dest = tempDouble;
        *data = strchr(*data, '\n') + 1;
        return true;
    }   
    else {
        return false;
    } 
}

bool copyDoubleWithToleranceProperty(const char* property, const char** data, double* val, double* tolerance) {
    if(startsWith(*data, property)){
        const char* valStart = strchr(*data, ',') + 1;
        char* valEnd;
        double tempVal = strtod(valStart, &valEnd);
        if(valEnd == valStart) {
            //Didn't read a number
            printf("Error in ADF file\n");
            return false;
        }

        const char* endOfLine = strchr(valEnd, '\n');
        const char* toleranceStart = strchr(valEnd, ',') + 1;
        //The tolerance portion is optional
        if(toleranceStart < endOfLine) {
            char* toleranceEnd;
            double tempTolerance = strtod(toleranceStart, &toleranceEnd);
            if(toleranceEnd == toleranceStart) {
                //Didn't read a number
                printf("Error in ADF file\n");
                return false;
            }

            *tolerance = tempTolerance;
        }   

        *val = tempVal;
        *data = endOfLine + 1;
        return true;
    }   
    else {
        return false;
    } 
}

struct ADFPattern parseADFPattern(const char* data, int len) {
    const char* current = data;
    struct ADFPattern pattern;

    copyStringProperty("REVNUM", &current, pattern.revisionNumber, 34);
    copyStringProperty("REVDAT", &current, pattern.revisionDate, 8);
    copyMultiPartStringProperty("COMNT", &current, &pattern.comment);
    copyStringProperty("ANTMAN", &current, pattern.antennaManufacturer, 34);
    copyStringProperty("MODNUM", &current, pattern.modelNumber, 34);
    copyStringProperty("PATNUM", &current, pattern.patternIDNumber, 34);
    copyRatioProperty("FILNUM", &current, &pattern.patternFileNumber, &pattern.patternFileNumberOutOf);
    copyStringProperty("FEDORN", &current, pattern.feedOrientation, 5);
    copyMultiPartStringProperty("DESCR", &current, &pattern.description);
    copyStringProperty("DTDATA", &current, pattern.dateOfData, 8);
    copyDoubleProperty("LOWFRQ", &current, &pattern.lowFrequency);
    copyDoubleProperty("HGHFRQ", &current, &pattern.highFrequency);

    //Parse gain units
    if(startsWith(current, "GUNITS")) {
        const char* maxGainUnitStart = strchr(current, ',') + 1;
        pattern.maxGainUnit = stringToGainUnit(maxGainUnitStart);

        const char* gainDataUnitStart = strchr(current, '/');
        pattern.gainDataUnit = stringToGainUnit(gainDataUnitStart);

        current = strchr(current, '\n') + 1;
    }

    copyDoubleProperty("LWGAIN", &current, &pattern.lowBandGain);
    copyDoubleProperty("MDGAIN", &current, &pattern.midBandGain);
    copyDoubleProperty("HGGAIN", &current, &pattern.highBandGain);
    copyDoubleWithToleranceProperty("AZWIDT", &current, &pattern.azimuthBeamWidth, &pattern.azimuthBeamWidthTolerance);
    copyDoubleWithToleranceProperty("ELWIDT", &current, &pattern.elevationBeamWidth, &pattern.elevationBeamWidthTolerance);
    copyStringProperty("CONTYP", &current, pattern.connectorType, 72);
    copyDoubleProperty("ATVSWR", &current, &pattern.voltageStandingWaveRatio);
    copyDoubleProperty("FRTOBA", &current, &pattern.frontToBackRatio);
    copyDoubleWithToleranceProperty("ELTILT", &current, &pattern.electricalDownTilt, &pattern.electricalDownTiltTolerance);
    copyDoubleProperty("RADCTR", &current, &pattern.radiationCenter);
    copyDoubleProperty("POTOPO", &current, &pattern.portToPortIsolation);
    copyDoubleProperty("MAXPOW", &current, &pattern.maximumInputPower);
    copyDoubleProperty("ANTLEN", &current, &pattern.antennaLength);
    copyDoubleProperty("ANTWID", &current, &pattern.antennaWidth);
    copyDoubleProperty("ANTDEP", &current, &pattern.antennaDepth);
    copyDoubleProperty("ANTWGT", &current, &pattern.antennaWeight);
    copyStringProperty("PATTYP", &current, pattern.patternType, 16);
    copyIntProperty("NOFREQ", &current, &pattern.numberOfFrequencies);

    pattern.frequencies = (struct ADFPatternFrequency*)malloc(sizeof(struct ADFPatternFrequency) * pattern.numberOfFrequencies);

    for(int i = 0; i < pattern.numberOfFrequencies; i++) {
        struct ADFPatternFrequency* freq = &pattern.frequencies[i];
        copyDoubleProperty("PATFRE", &current, &freq->patternFrequency);
        copyIntProperty("NUMCUT", &current, &freq->numberOfPatternCuts);

        freq->patternCuts = (struct ADFPatternCut*)malloc(sizeof(struct ADFPatternCut) * freq->numberOfPatternCuts);
        
        for(int j = 0; j < freq->numberOfPatternCuts; j++) {
            struct ADFPatternCut* cut = &freq->patternCuts[j];
            
            //Pattern cut type
            char cutTypeBuffer[3];
            copyStringProperty("PATCUT", &current, cutTypeBuffer, 3);
            cut->patternCut = stringToCutType(cutTypeBuffer);
            if(cut->patternCut == ADF_PATTERN_CUT_PHI) {
                char* endInt;
                cut->phiCutAngle = strtol(cutTypeBuffer, &endInt, 10);
                if(endInt == cutTypeBuffer) {
                    printf("There should have been a angle, defaulting to 0\n");
                }
            }

            //Polarization
            char polarizationBuff[7];
            copyStringProperty("POLARI", &current, polarizationBuff, 7);
            cut->polarizationOfAntenna = stringToPolarizationType(polarizationBuff);
            cut->polarizationOfSource = stringToPolarizationType(&polarizationBuff[3]);

            copyIntProperty("NUPOIN", &current, &cut->numberOfDataPoints);
            copyDoubleWithToleranceProperty("FSTLST", &current, &cut->firstAngleOfPatternData, &cut->lastAngleOfPatternData);
            copyStringProperty("XORIEN", &current, cut->xAxisOrientation, 45);
            copyStringProperty("YORIEN", &current, cut->yAxisOrientation, 45);
            copyStringProperty("ZORIEN", &current, cut->zAxisOrientation, 45);

            cut->dataPoints = (struct ADFPatternDataPoint*)malloc(sizeof(struct ADFPatternDataPoint) * cut->numberOfDataPoints);
            for(int k = 0; k < cut->numberOfDataPoints; k++) {
                cut->dataPoints[k].angleOfObservation = strtof(current, NULL);
                current = strchr(current, ',') + 1;
                cut->dataPoints[k].magnitudeResponse = strtof(current, NULL);

                //Phase response is not required, so check if it is there
                if(strchr(current, ',') < strchr(current, '\n')) {
                    current = strchr(current, ',') + 1;
                    cut->dataPoints[k].phaseResponse = strtof(current, NULL);
                }
                current = strchr(current, '\n') + 1;
            }

            for(int k = 0; k < cut->numberOfDataPoints; k++) {
                printf("%.2f, %.2f, %.2f\n", cut->dataPoints[k].angleOfObservation, cut->dataPoints[k].magnitudeResponse, cut->dataPoints[k].phaseResponse);
            }
        }

    }
    return pattern;
}

void freeADFPattern(struct ADFPattern pattern) {
    if(pattern.comment != NULL) {
        free(pattern.comment);
    }
    if(pattern.description != NULL) {
        free(pattern.description);
    }
}

int main() {
    return 0;
}