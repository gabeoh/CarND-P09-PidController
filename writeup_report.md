# PID Controller

## Objective

The goal of this project is to implement a PID controller that controls
the vehicle in the simulator using the provided Cross Tracking Error (CTE). 

---

## PID Controller Implementation

The PID Controller determines the steering wheel turn value based on the
Cross Tracking Error (CTE).  The CTE contributes to the resulting steering
value in three ways; proportional to the CTE (P), differentials of
subsequent CTEs (D), the cumulative sum of CTEs (I, integral).

## Parameter Optimization

### Implementation
`ParamOptimizer` class implements PID controller parameter optimizer utilizing
*Twiddle* algorithm.  The main executable works as a server interacting
with the simulator as a client.  The server receives the CTE and the speed of
the vehicle from the simulator, and it responds with the steering angle and
the throttle values.

Due to the reactive nature of the overall system architecture, the parameter
optimizer cannot control the overall execution flow.  As a result, the
optimizer is implemented as a state machine.  It tracks the optimizer's
state and the parameter being tuned.  This way, the Twiddle parameter
optimizer algorithm is successfully implemented under the reactive
overall architecture.

Another challenge with the parameter optimization was the limitation in the
parameter variation range.  When the parameters were far off, the vehicle
failed to stay on the road.  When such events occurred, the simulator had
to be restarted and the manual intervention was required for the optimization.

Also, in order to reflect different course features in the optimization,
the error computation is performed for the full lap of the vehicle drive.
Since the Twiddle algorithm achieves the optimal state through iterative
trials, it took long simulation time to achieve the final parameters. 

### Results
The P component contributes the steering value proportion to the CTE.
The bigger the P coefficient, the faster the error gets corrected.
However, the bigger value results in the overshoot and unsteady drive.

The D component mitigates the overshoot by taking the rate of CTE value
changes into an account.  The large D coefficient slows down the error
correction, and this can cause the vehicle deviate from the desired course.
When the value is too small, on the other hand, the overshoot problem
does not get addressed.

The I component corrects the vehicle controller's systematic bias.  In this
project, the bias was insignificant, and a small I coefficient helped
reduce the overall error.  

| Coefficient | Value    |
| ----------- |---------:|
| P           | 0.468120 |
| I           | 0.000050 |
| D           | 8.903274 |

### Throttle
The project also manipulates throttle value in the attempt to achieve higher
speed.  First, it determines whether to accelerate or to brake based on the
steering turn value.  Then, the speed is taken into a consideration to
compute the final throttle value.

### Sample Video
A sample video where the vehicle travels under the implemented PID controller
is attached. While it wobbles a bit, the vehicle successfully stays
close to the middle of the road throughout the entire track.
It achieves over 65 MPH in the straight stretch of the road. 

* [pid_control_sample_video.mp4](results/pid_control_sample_video.mp4)

