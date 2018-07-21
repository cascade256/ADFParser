#pragma once

enum ADFPatternCutType {
    ADF_PATTERN_CUT_HORIZONTAL,
    ADF_PATTERN_CUT_VERTICAL,
    ADF_PATTERN_CUT_AZIMUTH,
    ADF_PATTERN_CUT_ELEVATION,
    ADF_PATTERN_CUT_PHI
};

enum ADFPatternPolarizationType {
    ADF_PATTERN_POLARIZATION_HORIZONTAL_HORIZONTAL,
    ADF_PATTERN_POLARIZATION_HORIZONTAL_VERTICAL,
    ADF_PATTERN_POLARIZATION_VERTICAL_HORIZONTAL,
    ADF_PATTERN_POLARIZATION_VERTICAL_VERTICAL,
    ADF_PATTERN_POLARIZATION_SLANT_RIGHT,
    ADF_PATTERN_POLARIZATION_SLANT_LEFT,
    ADF_PATTERN_POLARIZATION_RIGHT_HAND,
    ADF_PATTERN_POLARIZATION_LEFT_HAND,
    ADF_PATTERN_POLARIZATION_E_THETA,
    ADF_PATTERN_POLARIZATION_E_PHI
};

enum ADFPatternGainUnit {
    ADF_PATTERN_GAIN_UNIT_DBI,
    ADF_PATTERN_GAIN_UNIT_DBD,
    ADF_PATTERN_GAIN_UNIT_DBR,
    ADF_PATTERN_GAIN_UNIT_LIN
};

struct ADFPatternDataPoint {
    float angleOfObservation;
    float magnitudeResponse;
    float phaseResponse;
};

struct ADFPatternCut {
    enum ADFPatternCutType patternCut;                        //Required
    int phiCutAngle;                                            //Required if patternCut is ADF_PATTERN_CUT_PHI
    enum ADFPatternPolarizationType polarizationOfSource;     //Required
    enum ADFPatternPolarizationType polarizationOfAntenna;    //Required
    int numberOfDataPoints;                                     //Required
    double firstAngleOfPatternData;                             //Required
    double lastAngleOfPatternData;                              //Required
    char xAxisOrientation[45];                  
    char yAxisOrientation[45]; 
    char zAxisOrientation[45];
    struct ADFPatternDataPoint* dataPoints;                     //Required
};

struct ADFPatternFrequency {
    double patternFrequency;//in MHz            //Required
    int numberOfPatternCuts;                    //Required
    struct ADFPatternCut* patternCuts;                 //Required
};

struct ADFPattern {
    char revisionNumber[34];                    //Required
    char revisionDate[8];//YYYYMMDD             //Required
    char* comment;              
    char antennaManufacturer[34];               //Required
    char modelNumber[34];                       //Required
    char patternIDNumber[34];
    int patternFileNumber;
    int patternFileNumberOutOf;
    char feedOrientation[5];
    char* description;
    char dateOfData[8];//YYYYMMDD
    double lowFrequency;//in MHz                //Required
    double highFrequency;//in MHz               //Required
    enum ADFPatternGainUnit maxGainUnit;      //Required
    enum ADFPatternGainUnit gainDataUnit;     //Required
    double lowBandGain;      
    double midBandGain;                         //Required
    double highBandGain;
    double azimuthBeamWidth;
    double azimuthBeamWidthTolerance;
    double elevationBeamWidth;
    double elevationBeamWidthTolerance;
    char connectorType[72];
    double voltageStandingWaveRatio;
    double frontToBackRatio;
    double electricalDownTilt;                   //Required
    double electricalDownTiltTolerance;
    double radiationCenter;//in meters
    double portToPortIsolation;
    double maximumInputPower;//in watts
    double antennaLength;//in meters
    double antennaWidth;//in meters
    double antennaDepth;//in meters
    double antennaWeight;//in kilograms
    char patternType[16];                       //Required
    int numberOfFrequencies;                    //Required
    struct ADFPatternFrequency* frequencies;    //Required
};

struct ADFPattern parseADFPattern(const char* data, int len);
void freeADFPattern(struct ADFPattern pattern);