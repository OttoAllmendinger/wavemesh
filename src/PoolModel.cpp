#include "PoolModel.h"

#include "Particle.h"

PoolModel::PoolModel(const int width, const int height) {

  particles_ = new Particle*[width];
  for (int i=0; i<width; i++) {
    particles_[i] = new Particle[height];
  }

}

PoolModel::~PoolModel() {

}

void PoolModel::update() {
}
