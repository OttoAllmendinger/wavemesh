#ifndef PARTICLE_H
#define PARTICLE_H

const double kTransmissionDefault = 0.25 * 0.9;

struct Particle {
  double height_;
  double velocity_;
  double transmission_n_; /* north */
  double transmission_e_; /* east  */
  double transmission_s_; /* south */
  double transmission_w_; /* west  */

  Particle() : 
    height_(0.0),
    velocity_(0.0),
    transmission_n_(kTransmissionDefault),
    transmission_e_(kTransmissionDefault),
    transmission_s_(kTransmissionDefault),
    transmission_w_(kTransmissionDefault) { }
};

#endif
