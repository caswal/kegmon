/*
MIT License

Copyright (c) 2021-22 Magnus

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
#ifndef SRC_LEVELS_HPP_
#define SRC_LEVELS_HPP_

#include <Arduino.h>

#include <kegconfig.hpp>
#include <levelraw.hpp>
#include <levelstatistic.hpp>
#include <stability.hpp>
#include <weightvolume.hpp>

constexpr auto LEVELS_FILENAME = "/levels.log";
constexpr auto LEVELS_FILENAME2 = "/levels2.log";
constexpr auto LEVELS_FILEMAXSIZE = 2000;

class LevelDetection {
 private:
 #if CFG_SCALECOUNT == 2
  Stability _stability[2];
  RawLevelDetection* _rawLevel[2] = {0, 0};
  StatsLevelDetection* _statsLevel[2] = {0, 0};
#elif CFG_SCALECOUNT == 3
  Stability _stability[3];
  RawLevelDetection* _rawLevel[3] = {0, 0, 0};
  StatsLevelDetection* _statsLevel[3] = {0, 0, 0};
#else
  Stability _stability[4];
  RawLevelDetection* _rawLevel[4] = {0, 0, 0, 0};
  StatsLevelDetection* _statsLevel[4] = {0, 0, 0, 0};
#endif

  LevelDetection(const LevelDetection&) = delete;
  void operator=(const LevelDetection&) = delete;

#if CFG_SCALECOUNT == 2
  void logLevels(float kegVolume1, float kegVolume2, float pourVolume1,
                 float pourVolume2);
#elif CFG_SCALECOUNT == 3
void logLevels(float kegVolume1, float kegVolume2, float kegVolume3, float pourVolume1,
                 float pourVolume2,float pourVolume3);
#elif CFG_SCALECOUNT == 4     
void logLevels(float kegVolume1, float kegVolume2, float kegVolume3, float kegVolume4, float pourVolume1,
                 float pourVolume2,float pourVolume3, float pourVolume4);            
                 #endif
  void pushKegUpdate(UnitIndex idx, float stableVol, float pourVol,
                     float glasses);
  void pushPourUpdate(UnitIndex idx, float stableVol, float pourVol);

 public:
  LevelDetection();
  void update(UnitIndex idx, float raw, float temp);

  Stability* getStability(UnitIndex idx) { return &_stability[idx]; }
  RawLevelDetection* getRawDetection(UnitIndex idx) { return _rawLevel[idx]; }
  StatsLevelDetection* getStatsDetection(UnitIndex idx) {
    return _statsLevel[idx];
  }

  // Return values based on the chosen algoritm
  bool hasStableWeight(UnitIndex idx,
                       LevelDetectionType type = myConfig.getLevelDetection());
  bool hasPourWeight(UnitIndex idx,
                     LevelDetectionType type = myConfig.getLevelDetection());

  float getBeerWeight(UnitIndex idx,
                      LevelDetectionType type = myConfig.getLevelDetection());
  float getBeerStableWeight(
      UnitIndex idx, LevelDetectionType type = myConfig.getLevelDetection());
  float getPourWeight(UnitIndex idx,
                      LevelDetectionType type = myConfig.getLevelDetection());

  float getTotalWeight(UnitIndex idx,
                       LevelDetectionType type = myConfig.getLevelDetection());
  float getTotalStableWeight(
      UnitIndex idx, LevelDetectionType type = myConfig.getLevelDetection());

  // Return the raw data (last value from scale)
  float getTotalRawWeight(UnitIndex idx);

  // Returns weights in liters
  float getBeerVolume(UnitIndex idx,
                      LevelDetectionType type = myConfig.getLevelDetection()) {
    WeightVolumeConverter conv(idx);
    return conv.weightToVolume(getBeerWeight(idx, type));
  }

  float getBeerStableVolume(
      UnitIndex idx, LevelDetectionType type = myConfig.getLevelDetection()) {
    WeightVolumeConverter conv(idx);
    return conv.weightToVolume(getBeerStableWeight(idx, type));
  }

  float getPourVolume(UnitIndex idx,
                      LevelDetectionType type = myConfig.getLevelDetection()) {
    WeightVolumeConverter conv(idx);
    return conv.weightToVolume(getPourWeight(idx, type));
  }

  float getNoGlasses(UnitIndex idx,
                     LevelDetectionType type = myConfig.getLevelDetection()) {
    WeightVolumeConverter conv(idx);
    return conv.weightToGlasses(getBeerWeight(idx, type));
  }

  float getNoStableGlasses(
      UnitIndex idx, LevelDetectionType type = myConfig.getLevelDetection()) {
    WeightVolumeConverter conv(idx);
    return conv.weightToGlasses(getBeerStableWeight(idx, type));
  }
};

extern LevelDetection myLevelDetection;

#endif  // SRC_LEVELS_HPP_

// EOF
