//
// Created by Gabriel Oh on 2018-11-19.
//

#include <cassert>
#include "ParamOptimizer.h"

void ParamOptimizer::tuneParameters(double error) {

  while (true) {
    switch (state) {
      case INITIALIZE_BEST_ERROR: {
        best_error = error;
        state = CHECK_TUNER_TOLERANCE;
        break;
      }
      case CHECK_TUNER_TOLERANCE: {
        double tuner_sum = 0.0;
        for (double tuner: tuners)
          tuner_sum += tuner;
        if (tuner_sum > tuner_tolerance) {
          state = ITERATE_NEXT_PARAM;
          param_index = 0;
        } else {
          state = DONE;
        }
        break;
      }
      case ITERATE_NEXT_PARAM: {
        if (param_index >= num_params) {
          state = CHECK_TUNER_TOLERANCE;
        } else {
          state = INCREMENT_PARAM;
        }
        break;
      }
      case INCREMENT_PARAM: {
        params[param_index] += tuners[param_index];
        state = CHECK_ERROR_INCREMENT;
        return;
      }
      case CHECK_ERROR_INCREMENT: {
        if (error < best_error) {
          best_error = error;
          tuners[param_index] *= 1.1;
          ++param_index;
          state = ITERATE_NEXT_PARAM;
        } else {
          state = DECREMENT_PARAM;
        }
        break;
      }
      case DECREMENT_PARAM: {
        params[param_index] -= 2 * tuners[param_index];
        state = CHECK_ERROR_DECREMENT;
        return;
      }
      case CHECK_ERROR_DECREMENT: {
        if (error < best_error) {
          best_error = error;
          tuners[param_index] *= 1.1;
        } else {
          params[param_index] += tuners[param_index];
          tuners[param_index] *= 0.9;
        }
        ++param_index;
        state = ITERATE_NEXT_PARAM;
        break;
      }
      case DONE:
        return;
      default:
        throw "Unknown ParamOptimizer state.";
    }
  }
}

