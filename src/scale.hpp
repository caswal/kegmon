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
#ifndef SRC_SCALE_HPP_
#define SRC_SCALE_HPP_

#include <HX711.h>
#include <Statistic.h>

#include <main.hpp>

#define SAMPLES \
  30  // Normal interval is 2 seconds, so this covers last 60 seconds

class Scale {
 private:
  HX711 *_scale[2] = {0, 0};
  float _lastWeight[2] = {0, 0};
  float _lastPourWeight[2] = {NAN, NAN};
  float _lastStableWeight[2] = {NAN, NAN};
  float _lastAverageWeight[2] = {NAN, NAN};
  statistic::Statistic<float, uint32_t, true> _statistic[2];
  statistic::Statistic<float, uint32_t, true> _stability[2];

  void setScaleFactor(UnitIndex idx);
  void statsAdd(UnitIndex idx, float val) {
    _statistic[idx].add(val);
    _stability[idx].add(val);
  }

#if defined(ENABLE_SCALE_SIMULATION)
  uint32_t _simulatedPos = 0;

  // This set of data is to check if the code rejects the 1.55 and 0.56 values
  // as invalid. float _simulatedData[10] = { 1.0, 1.01, 1.02, 0.99,
  // 0.98, 1.03, 1.55, 0.56, 0.99, 1.02 };

  // Data set that should simulate pouring of a pint.
  float _simulatedData[115] = {
      // Stable level
      9.30683533333333, 9.30827575, 9.309902, 9.31203925, 9.31573966666666,
      9.31561575, 9.31766066666666, 9.3180555, 9.320541, 9.32220175, 9.32270125,
      9.32435033333333, 9.32572075, 9.32636366666666, 9.3283925, 9.330328,
      9.3313305, 9.332643, 9.335377, 9.33557733333333, 9.3371035, 9.3383925,
      9.33901566666666, 9.34129625, 9.34273266666666, 9.3438625,
      9.34550433333333, 9.3470225, 9.34863624999999, 9.348741, 9.351029,
      9.35128066666666, 9.3525855, 9.35372766666666, 9.35459475, 9.355222,
      9.35620533333333, 9.35793966666666, 9.359856, 9.360135, 9.361285, 9.36312,
      9.36306566666666, 9.3638635, 9.36517166666666, 9.3651175, 9.36719675,
      9.36850166666666, 9.368416, 9.37000333333333, 9.37065799999999, 9.370824,
      9.372098, 9.3734915, 9.374169, 9.37439725, 9.373581, 9.37639575,
      9.37692633333333, 9.37719675, 9.37794833333333, 9.3787765,
      9.37946174999999, 9.37931033333333, 9.37962425, 9.38141666666666,
      9.38131975,
      // Pouring
      9.37121133333333, 9.269901, 9.162675, 9.1790975,
      // Level after pour
      9.11246475, 9.11290266666666, 9.114161, 9.11346033333333, 9.11539225,
      9.11459066666666, 9.1151595, 9.11711533333333, 9.11617, 9.11246475,
      9.11290266666666, 9.114161, 9.11346033333333, 9.11539225,
      9.11459066666666, 9.1151595, 9.11711533333333, 9.11617, 9.11246475,
      9.11290266666666, 9.114161, 9.11346033333333, 9.11539225,
      9.11459066666666, 9.1151595, 9.11711533333333, 9.11617, 9.11246475,
      9.11290266666666, 9.114161, 9.11346033333333, 9.11539225,
      9.11459066666666, 9.1151595, 9.11711533333333, 9.11617, 9.11246475,
      9.11290266666666, 9.114161, 9.11346033333333, 9.11539225,
      9.11459066666666, 9.1151595, 9.11711533333333};

  float getNextSimulated() {
    if (_simulatedPos >= (sizeof(_simulatedData) / sizeof(float)))
      _simulatedPos = 0;
    return _simulatedData[_simulatedPos++];
  }
#endif  // ENABLE_SCALE_SIMULATION

 public:
  Scale();

  // Setup and status
  void setup(bool force = false);
  void tare(UnitIndex idx);
#if defined(ENABLE_SCALE_SIMULATION)
  bool isConnected(UnitIndex idx) {
    return _scale[idx] != 0 || idx == 0 ? true : false;
  }
#else
  bool isConnected(UnitIndex idx) { return _scale[idx] != 0 ? true : false; }
#endif

  // Read from scale
  int32_t readRawWeight(UnitIndex idx);
  float readWeight(UnitIndex idx, bool updateStats = false);

  float getLastWeight(UnitIndex idx) { return _lastWeight[idx]; }
  float getAverageWeight(UnitIndex idx) {
    return statsCount(idx) > 0 ? statsAverage(idx) : _lastWeight[idx];
  }

  float getLastStableWeight(UnitIndex idx) { return _lastStableWeight[idx]; }
  bool hasLastStableWeight(UnitIndex idx) {
    return !isnan(_lastStableWeight[idx]);
  }

  float getPourWeight(UnitIndex idx) { return _lastPourWeight[idx]; }
  bool hasPourWeight(UnitIndex idx) { return !isnan(_lastPourWeight[idx]); }

  float getAverageWeightDirectionCoefficient(UnitIndex idx);

  // Calibration
  void findFactor(UnitIndex idx, float weight);

  // Helper methods
  float calculateNoGlasses(UnitIndex idx);

  // Statistics
  void statsClearAll() {
    statsClear(UnitIndex::U1);
    statsClear(UnitIndex::U2);
  }
  void statsClear(UnitIndex idx) { _statistic[idx].clear(); }
  uint32_t statsCount(UnitIndex idx) { return _statistic[idx].count(); }
  float statsSum(UnitIndex idx) { return _statistic[idx].sum(); }
  float statsMin(UnitIndex idx) { return _statistic[idx].minimum(); }
  float statsMax(UnitIndex idx) { return _statistic[idx].maximum(); }
  float statsAverage(UnitIndex idx) { return _statistic[idx].average(); }
  float statsVariance(UnitIndex idx) { return _statistic[idx].variance(); }
  float statsPopStdev(UnitIndex idx) { return _statistic[idx].pop_stdev(); }
  float statsUnbiasedStdev(UnitIndex idx) {
    return _statistic[idx].unbiased_stdev();
  }
  void statsDump(UnitIndex idx);

  // Stability since start
  void stabilityClearAll() {
    stabilityClear(UnitIndex::U1);
    stabilityClear(UnitIndex::U2);
  }
  void stabilityClear(UnitIndex idx) { _stability[idx].clear(); }
  uint32_t stabilityCount(UnitIndex idx) { return _stability[idx].count(); }
  float stabilitySum(UnitIndex idx) { return _stability[idx].sum(); }
  float stabilityMin(UnitIndex idx) { return _stability[idx].minimum(); }
  float stabilityMax(UnitIndex idx) { return _stability[idx].maximum(); }
  float stabilityAverage(UnitIndex idx) { return _stability[idx].average(); }
  float stabilityVariance(UnitIndex idx) { return _stability[idx].variance(); }
  float stabilityPopStdev(UnitIndex idx) { return _stability[idx].pop_stdev(); }
  float stabilityUnbiasedStdev(UnitIndex idx) {
    return _stability[idx].unbiased_stdev();
  }
};

extern Scale myScale;

#endif  // SRC_SCALE_HPP_

// EOF
