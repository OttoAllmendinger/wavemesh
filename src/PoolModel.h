#ifndef POOLMODEL_H
#define POOLMODEL_H

class Particle;

class PoolModel {
  public:
    PoolModel(int width, int height);
    ~PoolModel();

    double* getHeightMap() const;

    int getWidth() const;
    int getHeight() const;

    void update();

  private:
    Particle** particles_;
};

#endif
