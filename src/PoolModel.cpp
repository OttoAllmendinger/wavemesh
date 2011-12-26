#include <iostream>

#include "PoolModel.h"
#include "Particle.h"

Particle** MakeParticles(int width, int height) {

  Particle** particles = new Particle*[width]();

  for (int i=0; i<width; i++) {
    particles[i] = new Particle[height]();
  }

  return particles;
}


Particle** CopyParticles(Particle** particles, int width, int height) {

  Particle** copy_particles = MakeParticles(width, height);
  
  for (int i=0; i<width; i++) {
    for (int j=0; i<height; i++) {
      copy_particles[i][j] = particles[i][j];
    }
  }

  return copy_particles;
}

PoolModel::PoolModel(int width, int height)
  : width_(width), height_(height),
    particles_(MakeParticles(width_, height_)) { }


PoolModel::PoolModel(const PoolModel& poolModel)
  : width_(poolModel.width_),
    height_(poolModel.height_),
    particles_(CopyParticles(poolModel.particles_, width_, height_)) {
}


PoolModel::~PoolModel() {
  for (int x=0; x<width_; x++) {
    delete particles_[x];
  }
  delete particles_;
}

void PoolModel::update() {

  /* calculate velocities */

  for (int x=1; x<(width_-1); x++) {
    for (int y=1; y<(height_-1); y++) {

      Particle& p = particles_[x][y];

      double h = p.height_;

      p.velocity_ +=
          (particles_[x  ][y-1].height_ - h) * p.transmission_n_ +
          (particles_[x+1][y  ].height_ - h) * p.transmission_e_ +
          (particles_[x  ][y+1].height_ - h) * p.transmission_s_ +
          (particles_[x-1][y  ].height_ - h) * p.transmission_w_;
    }
  }

  /* update positions */

  for (int x=0; x<width_; x++) {
    for (int y=0; y<height_; y++) {
      particles_[x][y].height_ += particles_[x][y].velocity_;
    }
  }
}

double PoolModel::getParticleHeight(int x, int y) const {
  return particles_[x][y].height_;
}

void PoolModel::setParticleHeight(int x, int y, double v) {
  particles_[x][y].height_ = v;
}

int PoolModel::getWidth() const {
  return width_;
}

int PoolModel::getHeight() const {
  return height_;
}
