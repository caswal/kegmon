/*
MIT License

Copyright (c) 2021-23 Magnus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include <kegconfig.hpp>
#include <log.hpp>
#include <main.hpp>
#include <utils.hpp>

constexpr auto PARAM_PLATFORM = "platform";

KegConfig::KegConfig(String baseMDNS, String fileName)
    : BaseConfig(baseMDNS, fileName, JSON_BUFFER) {}

void KegConfig::createJson(DynamicJsonDocument& doc, bool skipSecrets) {
  // Call base class functions
  createJsonBase(doc, skipSecrets);
  createJsonWifi(doc, skipSecrets);
  createJsonOta(doc, skipSecrets);
  createJsonPush(doc, skipSecrets);

  // Handle project specific config
  doc[PARAM_WEIGHT_UNIT] = getWeightUnit();
  doc[PARAM_VOLUME_UNIT] = getVolumeUnit();

  doc[PARAM_DISPLAY_LAYOUT] = getDisplayLayoutTypeAsInt();
  doc[PARAM_TEMP_SENSOR] = getTempSensorTypeAsInt();
  doc[PARAM_SCALE_SENSOR] = getScaleSensorTypeAsInt();
  doc[PARAM_DISPLAY_DRIVER] = getDisplayDriverTypeAsInt();
  // doc[PARAM_LEVEL_DETECTION] = getLevelDetectionAsInt();

  doc[PARAM_BREWFATHER_APIKEY] = getBrewfatherApiKey();
  doc[PARAM_BREWFATHER_USERKEY] = getBrewfatherUserKey();

  doc[PARAM_BREWSPY_TOKEN1] = getBrewspyToken(UnitIndex::U1);
  doc[PARAM_BREWSPY_TOKEN2] = getBrewspyToken(UnitIndex::U2);

  Log.noticeln(F("%s:%d"), __FILE__, __LINE__);

  #if CFG_SCALECOUNT > 2
     doc[PARAM_BREWSPY_TOKEN3] = getBrewspyToken(UnitIndex::U3);
  #endif  
  #if CFG_SCALECOUNT > 3
     doc[PARAM_BREWSPY_TOKEN4] = getBrewspyToken(UnitIndex::U4);
  #endif    

  doc[PARAM_SCALE_TEMP_FORMULA1] =
      getScaleTempCompensationFormula(UnitIndex::U1);
  doc[PARAM_SCALE_FACTOR1] =
      serialized(String(getScaleFactor(UnitIndex::U1), 5));
  doc[PARAM_SCALE_OFFSET1] = getScaleOffset(UnitIndex::U1);
  
  doc[PARAM_KEG_WEIGHT1] =
      serialized(String(convertOutgoingWeight(getKegWeight(UnitIndex::U1)),
                        getWeightPrecision()));
  doc[PARAM_KEG_VOLUME1] = serialized(String(
      getKegVolume(UnitIndex::U1),
      getWeightPrecision()));  // Dont convert this part (drop down in UI)
  doc[PARAM_GLASS_VOLUME1] = serialized(String(
      getGlassVolume(UnitIndex::U1),
      3));  // Dont convert this part (drop down in UI)
  doc[PARAM_BEER_NAME1] = getBeerName(UnitIndex::U1);
  doc[PARAM_BEER_ABV1] = serialized(String(getBeerABV(UnitIndex::U1), 2));
  doc[PARAM_BEER_FG1] = serialized(String(getBeerFG(UnitIndex::U1), 2));
  doc[PARAM_BEER_EBC1] = getBeerEBC(UnitIndex::U1);
  doc[PARAM_BEER_IBU1] = getBeerIBU(UnitIndex::U1);

  doc[PARAM_SCALE_TEMP_FORMULA2] =
      getScaleTempCompensationFormula(UnitIndex::U2);
  doc[PARAM_SCALE_FACTOR2] =
      serialized(String(getScaleFactor(UnitIndex::U2), 5));
  doc[PARAM_SCALE_OFFSET2] = getScaleOffset(UnitIndex::U2);
  doc[PARAM_KEG_WEIGHT2] =
      serialized(String(convertOutgoingWeight(getKegWeight(UnitIndex::U2)),
                        getWeightPrecision()));
  doc[PARAM_KEG_VOLUME2] = serialized(String(
      getKegVolume(UnitIndex::U2),
      getWeightPrecision()));  // Dont convert this part (drop down in UI)
  doc[PARAM_GLASS_VOLUME2] =
      serialized(String(getGlassVolume(UnitIndex::U2),
                       3));  // Dont convert this part (drop down in UI)
  doc[PARAM_BEER_NAME2] = getBeerName(UnitIndex::U2);
  doc[PARAM_BEER_ABV2] = serialized(String(getBeerABV(UnitIndex::U2), 2));
  doc[PARAM_BEER_FG2] = serialized(String(getBeerFG(UnitIndex::U2), 2));
  doc[PARAM_BEER_EBC2] = getBeerEBC(UnitIndex::U2);
  doc[PARAM_BEER_IBU2] = getBeerIBU(UnitIndex::U2);

  Log.noticeln(F("%s:%d"), __FILE__, __LINE__);
  #if CFG_SCALECOUNT > 2
       doc[PARAM_SCALE_TEMP_FORMULA3] =
      getScaleTempCompensationFormula(UnitIndex::U3);
  doc[PARAM_SCALE_FACTOR3] =
      serialized(String(getScaleFactor(UnitIndex::U3), 5));
  doc[PARAM_SCALE_OFFSET3] = getScaleOffset(UnitIndex::U3);
  doc[PARAM_KEG_WEIGHT3] =
      serialized(String(convertOutgoingWeight(getKegWeight(UnitIndex::U3)),
                        getWeightPrecision()));
  doc[PARAM_KEG_VOLUME3] = serialized(String(
      getKegVolume(UnitIndex::U3),
      getWeightPrecision()));  // Dont convert this part (drop down in UI)
  doc[PARAM_GLASS_VOLUME3] = serialized(String(
      getGlassVolume(UnitIndex::U3),
      3));  // Dont convert this part (drop down in UI)
  doc[PARAM_BEER_NAME3] = getBeerName(UnitIndex::U3);
  doc[PARAM_BEER_ABV3] = serialized(String(getBeerABV(UnitIndex::U3), 2));
  doc[PARAM_BEER_FG3] = serialized(String(getBeerFG(UnitIndex::U3), 2));
  doc[PARAM_BEER_EBC3] = getBeerEBC(UnitIndex::U3);
  doc[PARAM_BEER_IBU3] = getBeerIBU(UnitIndex::U3);
  #endif  
Log.noticeln(F("%s:%d"), __FILE__, __LINE__);
  #if CFG_SCALECOUNT > 3
       doc[PARAM_SCALE_TEMP_FORMULA4] =
      getScaleTempCompensationFormula(UnitIndex::U4);
  doc[PARAM_SCALE_FACTOR4] =
      serialized(String(getScaleFactor(UnitIndex::U4), 5));
  doc[PARAM_SCALE_OFFSET4] = getScaleOffset(UnitIndex::U4);
  doc[PARAM_KEG_WEIGHT4] =
      serialized(String(convertOutgoingWeight(getKegWeight(UnitIndex::U4)),
                        getWeightPrecision()));
  doc[PARAM_KEG_VOLUME4] = serialized(String(
      getKegVolume(UnitIndex::U4),
      getWeightPrecision()));  // Dont convert this part (drop down in UI)
  doc[PARAM_GLASS_VOLUME4] = serialized(String(
      getGlassVolume(UnitIndex::U4),
      getWeightPrecision()));  // Dont convert this part (drop down in UI)
  doc[PARAM_BEER_NAME4] = getBeerName(UnitIndex::U4);
  doc[PARAM_BEER_ABV4] = serialized(String(getBeerABV(UnitIndex::U4), 2));
  doc[PARAM_BEER_FG4] = serialized(String(getBeerFG(UnitIndex::U4), 2));
  doc[PARAM_BEER_EBC4] = getBeerEBC(UnitIndex::U4);
  doc[PARAM_BEER_IBU4] = getBeerIBU(UnitIndex::U4);
  #endif    

  doc[PARAM_SCALE_DEVIATION_INCREASE] =
      serialized(String(getScaleDeviationIncreaseValue(), 2));
  doc[PARAM_SCALE_DEVIATION_DECREASE] =
      serialized(String(getScaleDeviationDecreaseValue(), 2));
  doc[PARAM_SCALE_DEVIATION_KALMAN] =
      serialized(String(getScaleKalmanDeviationValue(), 2));
  doc[PARAM_SCALE_READ_COUNT] = getScaleReadCount();
  doc[PARAM_SCALE_READ_COUNT_CALIBRATION] = getScaleReadCountCalibration();
  doc[PARAM_SCALE_STABLE_COUNT] = getScaleStableCount();

  doc[PARAM_PIN_DISPLAY_DATA] = getPinDisplayData();
  doc[PARAM_PIN_DISPLAY_CLOCK] = getPinDisplayClock();
  doc[PARAM_PIN_SCALE1_DATA] = getPinScale1Data();
  doc[PARAM_PIN_SCALE1_CLOCK] = getPinScale1Clock();
  doc[PARAM_PIN_SCALE2_DATA] = getPinScale2Data();
  doc[PARAM_PIN_SCALE2_CLOCK] = getPinScale2Clock();
  doc[PARAM_PIN_TEMP_DATA] = getPinTempData();
  doc[PARAM_PIN_TEMP_POWER] = getPinTempPower();
Log.noticeln(F("%s:%d"), __FILE__, __LINE__);
    #if CFG_SCALECOUNT > 2
  doc[PARAM_PIN_SCALE3_DATA] = getPinScale3Data();
  doc[PARAM_PIN_SCALE3_CLOCK] = getPinScale3Clock();
  #endif  

    #if CFG_SCALECOUNT > 3
  doc[PARAM_PIN_SCALE4_DATA] = getPinScale4Data();
  doc[PARAM_PIN_SCALE4_CLOCK] = getPinScale4Clock();
  #endif    
Log.noticeln(F("%s:%d"), __FILE__, __LINE__);
#if defined(ESP8266)
  doc[PARAM_PLATFORM] = "esp8266";
#elif defined(ESP32S2)
  doc[PARAM_PLATFORM] = "esp32s2";
#endif

  /*
  doc[PARAM_KALMAN_ACTIVE] = isKalmanActive();
  doc[PARAM_KALMAN_MEASUREMENT] = getKalmanMeasurement();
  doc[PARAM_KALMAN_ESTIMATION] = getKalmanEstimation();
  doc[PARAM_KALMAN_NOISE] = getKalmanNoise();
  */
}

void KegConfig::parseJson(DynamicJsonDocument& doc) {
  // Call base class functions
  parseJsonBase(doc);
  parseJsonWifi(doc);
  parseJsonOta(doc);
  parseJsonPush(doc);

  // Handle project specific config
  if (!doc[PARAM_WEIGHT_UNIT].isNull()) setWeightUnit(doc[PARAM_WEIGHT_UNIT]);
  if (!doc[PARAM_VOLUME_UNIT].isNull()) setVolumeUnit(doc[PARAM_VOLUME_UNIT]);

  if (!doc[PARAM_BREWFATHER_APIKEY].isNull())
    setBrewfatherApiKey(doc[PARAM_BREWFATHER_APIKEY]);
  if (!doc[PARAM_BREWFATHER_USERKEY].isNull())
    setBrewfatherUserKey(doc[PARAM_BREWFATHER_USERKEY]);

  if (!doc[PARAM_BREWSPY_TOKEN1].isNull())
    setBrewspyToken(UnitIndex::U1, doc[PARAM_BREWSPY_TOKEN1]);
  if (!doc[PARAM_BREWSPY_TOKEN2].isNull())
    setBrewspyToken(UnitIndex::U2, doc[PARAM_BREWSPY_TOKEN2]);

#if CFG_SCALECOUNT > 2
  if (!doc[PARAM_BREWSPY_TOKEN3].isNull())
    setBrewspyToken(UnitIndex::U3, doc[PARAM_BREWSPY_TOKEN3]);
#endif


#if CFG_SCALECOUNT > 3
  if (!doc[PARAM_BREWSPY_TOKEN4].isNull())
    setBrewspyToken(UnitIndex::U4, doc[PARAM_BREWSPY_TOKEN3]);
#endif

  if (!doc[PARAM_DISPLAY_LAYOUT].isNull())
    setDisplayLayoutType(doc[PARAM_DISPLAY_LAYOUT].as<int>());
  if (!doc[PARAM_TEMP_SENSOR].isNull())
    setTempSensorType(doc[PARAM_TEMP_SENSOR].as<int>());
  if (!doc[PARAM_SCALE_SENSOR].isNull())
    setScaleSensorType(doc[PARAM_SCALE_SENSOR].as<int>());
  if (!doc[PARAM_DISPLAY_DRIVER].isNull())
    setDisplayDriverType(doc[PARAM_DISPLAY_DRIVER].as<int>());

  /*if (!doc[PARAM_LEVEL_DETECTION].isNull())
    setLevelDetection(doc[PARAM_LEVEL_DETECTION].as<int>());*/

  if (!doc[PARAM_SCALE_TEMP_FORMULA1].isNull())
    setScaleTempCompensationFormula(UnitIndex::U1,
                                    doc[PARAM_SCALE_TEMP_FORMULA1]);
  if (!doc[PARAM_SCALE_FACTOR1].isNull())
    setScaleFactor(UnitIndex::U1, doc[PARAM_SCALE_FACTOR1].as<float>());
  if (!doc[PARAM_SCALE_OFFSET1].isNull())
    setScaleOffset(UnitIndex::U1, doc[PARAM_SCALE_OFFSET1].as<float>());
  if (!doc[PARAM_KEG_WEIGHT1].isNull())
    setKegWeight(UnitIndex::U1,
                 convertIncomingWeight(doc[PARAM_KEG_WEIGHT1].as<float>()));
  if (!doc[PARAM_KEG_VOLUME1].isNull())
    setKegVolume(
        UnitIndex::U1,
        doc[PARAM_KEG_VOLUME1]
            .as<float>());  // No need to convert this, always in Liters
  if (!doc[PARAM_GLASS_VOLUME1].isNull())
    setGlassVolume(
        UnitIndex::U1,
        doc[PARAM_GLASS_VOLUME1]
            .as<float>());  // No need to convert this, always in Liters
  if (!doc[PARAM_BEER_NAME1].isNull())
    setBeerName(UnitIndex::U1, doc[PARAM_BEER_NAME1]);
  if (!doc[PARAM_BEER_EBC1].isNull())
    setBeerEBC(UnitIndex::U1, doc[PARAM_BEER_EBC1].as<int>());
  if (!doc[PARAM_BEER_ABV1].isNull())
    setBeerABV(UnitIndex::U1, doc[PARAM_BEER_ABV1].as<float>());
  if (!doc[PARAM_BEER_IBU1].isNull())
    setBeerIBU(UnitIndex::U1, doc[PARAM_BEER_IBU1].as<int>());
  if (!doc[PARAM_BEER_FG1].isNull())
    setBeerFG(UnitIndex::U1, doc[PARAM_BEER_FG1].as<float>());

  if (!doc[PARAM_SCALE_TEMP_FORMULA2].isNull())
    setScaleTempCompensationFormula(UnitIndex::U2,
                                    doc[PARAM_SCALE_TEMP_FORMULA2]);
  if (!doc[PARAM_SCALE_FACTOR2].isNull())
    setScaleFactor(UnitIndex::U2, doc[PARAM_SCALE_FACTOR2].as<float>());
  if (!doc[PARAM_SCALE_OFFSET2].isNull())
    setScaleOffset(UnitIndex::U2, doc[PARAM_SCALE_OFFSET2].as<float>());
  if (!doc[PARAM_KEG_WEIGHT2].isNull())
    setKegWeight(UnitIndex::U2,
                 convertIncomingWeight(doc[PARAM_KEG_WEIGHT2].as<float>()));
  if (!doc[PARAM_KEG_VOLUME2].isNull())
    setKegVolume(
        UnitIndex::U2,
        doc[PARAM_KEG_VOLUME2]
            .as<float>());  // No need to convert this, always in Liters
  if (!doc[PARAM_GLASS_VOLUME2].isNull())
    setGlassVolume(
        UnitIndex::U2,
        doc[PARAM_GLASS_VOLUME2]
            .as<float>());  // No need to convert this, always in Liters
  if (!doc[PARAM_BEER_NAME2].isNull())
    setBeerName(UnitIndex::U2, doc[PARAM_BEER_NAME2]);
  if (!doc[PARAM_BEER_EBC2].isNull())
    setBeerEBC(UnitIndex::U2, doc[PARAM_BEER_EBC2].as<int>());
  if (!doc[PARAM_BEER_ABV2].isNull())
    setBeerABV(UnitIndex::U2, doc[PARAM_BEER_ABV2].as<float>());
  if (!doc[PARAM_BEER_IBU2].isNull())
    setBeerIBU(UnitIndex::U2, doc[PARAM_BEER_IBU2].as<int>());
  if (!doc[PARAM_BEER_FG2].isNull())
    setBeerFG(UnitIndex::U2, doc[PARAM_BEER_FG2].as<float>());

#if CFG_SCALECOUNT > 2

  if (!doc[PARAM_SCALE_TEMP_FORMULA3].isNull())
    setScaleTempCompensationFormula(UnitIndex::U3,
                                    doc[PARAM_SCALE_TEMP_FORMULA3]);
  if (!doc[PARAM_SCALE_FACTOR3].isNull())
    setScaleFactor(UnitIndex::U3, doc[PARAM_SCALE_FACTOR3].as<float>());
  if (!doc[PARAM_SCALE_OFFSET3].isNull())
    setScaleOffset(UnitIndex::U3, doc[PARAM_SCALE_OFFSET3].as<float>());
  if (!doc[PARAM_KEG_WEIGHT3].isNull())
    setKegWeight(UnitIndex::U3,
                 convertIncomingWeight(doc[PARAM_KEG_WEIGHT3].as<float>()));
  if (!doc[PARAM_KEG_VOLUME3].isNull())
    setKegVolume(
        UnitIndex::U3,
        doc[PARAM_KEG_VOLUME3]
            .as<float>()); // No need to convert this, always in Liters
  if (!doc[PARAM_GLASS_VOLUME3].isNull())
    setGlassVolume(
        UnitIndex::U3,
        doc[PARAM_GLASS_VOLUME3]
            .as<float>()); // No need to convert this, always in Liters
  if (!doc[PARAM_BEER_NAME3].isNull())
    setBeerName(UnitIndex::U3, doc[PARAM_BEER_NAME3]);
  if (!doc[PARAM_BEER_EBC3].isNull())
    setBeerEBC(UnitIndex::U3, doc[PARAM_BEER_EBC3].as<int>());
  if (!doc[PARAM_BEER_ABV3].isNull())
    setBeerABV(UnitIndex::U3, doc[PARAM_BEER_ABV3].as<float>());
  if (!doc[PARAM_BEER_IBU3].isNull())
    setBeerIBU(UnitIndex::U3, doc[PARAM_BEER_IBU3].as<int>());
  if (!doc[PARAM_BEER_FG3].isNull())
    setBeerFG(UnitIndex::U3, doc[PARAM_BEER_FG3].as<float>());
#endif

#if CFG_SCALECOUNT > 3

  if (!doc[PARAM_SCALE_TEMP_FORMULA4].isNull())
    setScaleTempCompensationFormula(UnitIndex::U4,
                                    doc[PARAM_SCALE_TEMP_FORMULA4]);
  if (!doc[PARAM_SCALE_FACTOR4].isNull())
    setScaleFactor(UnitIndex::U4, doc[PARAM_SCALE_FACTOR4].as<float>());
  if (!doc[PARAM_SCALE_OFFSET4].isNull())
    setScaleOffset(UnitIndex::U4, doc[PARAM_SCALE_OFFSET4].as<float>());
  if (!doc[PARAM_KEG_WEIGHT4].isNull())
    setKegWeight(UnitIndex::U4,
                 convertIncomingWeight(doc[PARAM_KEG_WEIGHT4].as<float>()));
  if (!doc[PARAM_KEG_VOLUME4].isNull())
    setKegVolume(
        UnitIndex::U4,
        doc[PARAM_KEG_VOLUME4]
            .as<float>());  // No need to convert this, always in Liters
  if (!doc[PARAM_GLASS_VOLUME4].isNull())
    setGlassVolume(
        UnitIndex::U4,
        doc[PARAM_GLASS_VOLUME4]
            .as<float>());  // No need to convert this, always in Liters
  if (!doc[PARAM_BEER_NAME4].isNull())
    setBeerName(UnitIndex::U4, doc[PARAM_BEER_NAME4]);
  if (!doc[PARAM_BEER_EBC4].isNull())
    setBeerEBC(UnitIndex::U4, doc[PARAM_BEER_EBC4].as<int>());
  if (!doc[PARAM_BEER_ABV4].isNull())
    setBeerABV(UnitIndex::U4, doc[PARAM_BEER_ABV4].as<float>());
  if (!doc[PARAM_BEER_IBU4].isNull())
    setBeerIBU(UnitIndex::U4, doc[PARAM_BEER_IBU4].as<int>());
  if (!doc[PARAM_BEER_FG4].isNull())
    setBeerFG(UnitIndex::U4, doc[PARAM_BEER_FG4].as<float>());
#endif

  if (!doc[PARAM_SCALE_DEVIATION_DECREASE].isNull())
    setScaleDeviationDecreaseValue(doc[PARAM_SCALE_DEVIATION_DECREASE]);
  if (!doc[PARAM_SCALE_DEVIATION_INCREASE].isNull())
    setScaleDeviationIncreaseValue(doc[PARAM_SCALE_DEVIATION_INCREASE]);
  if (!doc[PARAM_SCALE_DEVIATION_KALMAN].isNull())
    setScaleKalmanDeviationValue(doc[PARAM_SCALE_DEVIATION_KALMAN]);
  if (!doc[PARAM_SCALE_READ_COUNT].isNull())
    setScaleReadCount(doc[PARAM_SCALE_READ_COUNT]);
  if (!doc[PARAM_SCALE_READ_COUNT_CALIBRATION].isNull())
    setScaleReadCountCalibration(doc[PARAM_SCALE_READ_COUNT_CALIBRATION]);
  if (!doc[PARAM_SCALE_STABLE_COUNT].isNull())
    setScaleStableCount(doc[PARAM_SCALE_STABLE_COUNT]);

  if (!doc[PARAM_PIN_DISPLAY_DATA].isNull())
    setPinDisplayData(doc[PARAM_PIN_DISPLAY_DATA]);
  if (!doc[PARAM_PIN_DISPLAY_CLOCK].isNull())
    setPinDisplayClock(doc[PARAM_PIN_DISPLAY_CLOCK]);
  if (!doc[PARAM_PIN_SCALE1_DATA].isNull())
    setPinScale1Data(doc[PARAM_PIN_SCALE1_DATA]);
  if (!doc[PARAM_PIN_SCALE1_CLOCK].isNull())
    setPinScale1Clock(doc[PARAM_PIN_SCALE1_CLOCK]);
  if (!doc[PARAM_PIN_SCALE2_DATA].isNull())
    setPinScale2Data(doc[PARAM_PIN_SCALE2_DATA]);
  if (!doc[PARAM_PIN_SCALE2_CLOCK].isNull())
    setPinScale2Clock(doc[PARAM_PIN_SCALE2_CLOCK]);
  if (!doc[PARAM_PIN_TEMP_DATA].isNull())
    setPinTempData(doc[PARAM_PIN_TEMP_DATA]);
  if (!doc[PARAM_PIN_TEMP_POWER].isNull())
    setPinTempPower(doc[PARAM_PIN_TEMP_POWER]);

#if CFG_SCALECOUNT > 2
  if (!doc[PARAM_PIN_SCALE3_DATA].isNull())
    setPinScale3Data(doc[PARAM_PIN_SCALE3_DATA]);
  if (!doc[PARAM_PIN_SCALE3_CLOCK].isNull())
    setPinScale3Clock(doc[PARAM_PIN_SCALE3_CLOCK]);
#endif

#if CFG_SCALECOUNT > 3
  if (!doc[PARAM_PIN_SCALE4_DATA].isNull())
    setPinScale4Data(doc[PARAM_PIN_SCALE4_DATA]);
  if (!doc[PARAM_PIN_SCALE4_CLOCK].isNull())
    setPinScale4Clock(doc[PARAM_PIN_SCALE4_CLOCK]);
#endif
  /*
  if (!doc[PARAM_KALMAN_ESTIMATION].isNull())
    setKalmanEstimation(doc[PARAM_KALMAN_ESTIMATION].as<float>());
  if (!doc[PARAM_KALMAN_MEASUREMENT].isNull())
    setKalmanMeasurement(doc[PARAM_KALMAN_MEASUREMENT].as<float>());
  if (!doc[PARAM_KALMAN_NOISE].isNull())
    setKalmanNoise(doc[PARAM_KALMAN_NOISE].as<float>());
  if (!doc[PARAM_KALMAN_ACTIVE].isNull()) {
    String s = doc[PARAM_KALMAN_ACTIVE];
    setKalmanActive(s.equals("yes") ? true : false);
  }*/
}

float convertIncomingWeight(float w) {
  float r;

  if (myConfig.isWeightUnitKG())
    r = w;
  else
    r = convertLBStoKG(w);

#if LOG_LEVEL == 6
    // Log.verbose(F("CFG : Convering %F to %F (%s)." CR), w, r,
    // myConfig.getWeightUnit());
#endif
  return r;
}

float convertIncomingVolume(float v) {
  float r;

  if (myConfig.isVolumeUnitCL())
    r = v * 100.0;
  else if (myConfig.isVolumeUnitML())
    r = v * 1000.0;
  else if (myConfig.isVolumeUnitUSOZ())
    r = convertUSOZtoCL(v);

  else if (myConfig.isVolumeUnitUKOZ())
    r = convertUKOZtoCL(v);    
  else
    r = convertUKPinttoCL(v);

#if LOG_LEVEL == 6
    // Log.verbose(F("CFG : Converting incoming volume %F to %F (%s)." CR), v,
    // r, myConfig.getVolumeUnit());
#endif
  return r;
}

float convertOutgoingWeight(float w) {
  float r;

  if (myConfig.isWeightUnitKG())
    r = w;
  else
    r = convertKGtoLBS(w);

#if LOG_LEVEL == 6
    // Log.verbose(F("CFG : Converting outgoing weight %F to %F (%s)." CR), w,
    // r, myConfig.getWeightUnit());
#endif
  if (isnan(r))
    return 0.0f;

  return r;
}

float convertOutgoingVolume(float v) {
  float r;

  if (myConfig.isVolumeUnitCL())
    r = (v == 0.0 ? 0.0 : v * 100.0);
  else if (myConfig.isVolumeUnitML())
    r = (v == 0.0 ? 0.0 : v * 1000.0);
  else if (myConfig.isVolumeUnitUSOZ())
    r = convertCLtoUSOZ(v == 0.0 ? 0.0 : v * 100.0);
  else if (myConfig.isVolumeUnitUKOZ())
    r = convertCLtoUKOZ(v == 0.0 ? 0.0 : v * 100.0);    
  else
    r = convertCLtoUKPint(v == 0.0 ? 0.0 : v * 100.0);

#if LOG_LEVEL == 6
    // Log.verbose(F("CFG : Converting outgoing volume %F to %F (%s)." CR), v,
    // r, myConfig.getVolumeUnit());
#endif
  if (isnan(r))
    return 0.0f;

  return r;
}

float convertOutgoingTemperature(float t) {
  if (myConfig.isTempFormatC()) return t;
  return convertCtoF(t);
}

// EOF
