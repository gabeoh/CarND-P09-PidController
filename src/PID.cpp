#include "PID.h"

/*
* TODO: Complete the PID class.
*/

PID::PID() = default;

PID::~PID() = default;

void PID::Init(double Kp, double Ki, double Kd) {
  // Set PID controller initial errors
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  total_error = 0.0;
  update_count = 0;

  // Set PID controller coefficient
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
}

void PID::UpdateError(double cte) {
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;

  total_error += cte * cte;
  ++update_count;
}

double PID::TotalError() {
  return total_error / update_count;
}

