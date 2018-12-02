#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include "ParamOptimizer.h"
#include <cmath>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(const std::string &s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of('[');
  auto b2 = s.find_last_of(']');
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  // Define parameter optimizer
  const bool run_param_tune = false;
  const bool debug = true;
  const int param_tune_interval = 9000;
  // const vector<double> initial_params = {0.3, 0.0, 6.0};
  // const vector<double> initial_param_tuner = {0.01, 0.0001, 1.0};
  const vector<double> initial_params = {0.468120, 0.00005, 8.903274};
  const vector<double> initial_param_tuner = {0.0007, 0.00004, 0.003};
  ParamOptimizer p_opt(3);
  p_opt.setParams(initial_params);
  p_opt.setTuners(initial_param_tuner);

  // Initializer PID tuner with initial parameter values
  PID pid;
  pid.Init(initial_params[0], initial_params[1], initial_params[2]);

  h.onMessage([&pid, &p_opt](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (!s.empty()) {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;

          // Calculate steering value here using PID controller
          pid.UpdateError(cte);
          steer_value = - pid.Kp * pid.p_error - pid.Kd * pid.d_error - pid.Ki * pid.i_error;

          // Tune parameter every 'param_tune_interval' run
          if (run_param_tune && pid.update_count % param_tune_interval == 0) {

            std::cout << " <== ERROR:" << std::scientific << pid.total_error;
            std::cout << " (Best Error: " << p_opt.getBest_error() << ")" << std::endl;

            p_opt.tuneParameters(pid.total_error);
            vector<double> params = p_opt.getParams();
            vector<double> tuners = p_opt.getTuners();
            double tuner_sum = tuners[0] + tuners[1] + tuners[2];
            pid.Init(params[0], params[1], params[2]);

            std::cout << "## PARAMS: " << std::fixed << "(" << params[0] << "," << params[1] << "," << params[2] << ")";
            std::cout << ", TUNERS: (SUM:" << tuner_sum << ", " << tuners[0] << "," << tuners[1] << "," << tuners[2] << ")" << std::flush;
          }

          // Compute throttle value based on the steering value
          double steer_abs = abs(steer_value);
          double throttle = 0.2;
          if (steer_abs < 1.2) {
            throttle = (0.5 - steer_abs);
            // Adjust the throttle value based on the current vehicle speed
            if (throttle < 0.0) {
              throttle *= (speed / 6.0);
            } else {
              throttle *= (180.0 / speed);
            }
          }

          // DEBUG
          if (debug && !run_param_tune)
            std::cout << "CTE: " << cte << ", Steering Value: " << steer_value << ", Throttle: " << throttle << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          if (debug)
            std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
