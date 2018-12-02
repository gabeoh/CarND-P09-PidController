/**
 * ParamOptimizer.h
 *
 * Parameter optimizer using twiddle algorithm
 *
 * Created on: Nov 19th, 2018
 * Author: Gabriel Jungmoo Oh
 */

#ifndef PID_PARAMOPTIMIZER_H
#define PID_PARAMOPTIMIZER_H


#include <vector>
#include <iostream>

using namespace std;
class ParamOptimizer {
  // Number of parameters
  int num_params;

  double best_error;

  // Tuner tolerance
  double tuner_tolerance;

  // Vector of parameters
  vector<double> params;

  // Vector of parameter tuning values
  vector<double> tuners;

  enum State {
    INITIALIZE_BEST_ERROR,
    CHECK_TUNER_TOLERANCE,
    ITERATE_NEXT_PARAM,
    INCREMENT_PARAM,
    CHECK_ERROR_INCREMENT,
    DECREMENT_PARAM,
    CHECK_ERROR_DECREMENT,
    DONE
  };

  State state;
  int param_index;

public:
  explicit ParamOptimizer(int num_params) : num_params(num_params), best_error(0.0), tuner_tolerance(0.001),
                                   params(num_params, 0.0), tuners(num_params, 1.0),
                                   state(INITIALIZE_BEST_ERROR), param_index(0) {}
  ~ParamOptimizer() = default;

  void setTunerTolerance(double tolerance) {
    tuner_tolerance = tolerance;
  }

  void setParams(const vector<double> &params)  {
    // Make sure the number of the given parameters matches with optimizer setting
    assert(params.size() == num_params);
    this->params.assign(params.begin(), params.end());
  }

  void setTuners(const vector<double> &tuners)  {
    // Make sure the number of the given parameter tuners matches with optimizer setting
    assert(tuners.size() == num_params);
    this->tuners.assign(tuners.begin(), tuners.end());
  }

  double getBest_error() const {
    return best_error;
  }

  vector<double> getParams() {
    return params;
  }

  vector<double> getTuners() {
    return tuners;
  }

  void tuneParameters(double error);
};


#endif //PID_PARAMOPTIMIZER_H
