#ifndef POOLMODEL_H
#define POOLMODEL_H

struct Particle;

class PoolModel {
  public:
    PoolModel(const PoolModel&);
    PoolModel(int width, int height);
    ~PoolModel();

    double* getHeightMap() const;

    /* low performance */
    double getParticleHeight(int x, int y) const;
    void setParticleHeight(int x, int y, double v);

    int getWidth() const;
    int getHeight() const;

    PoolModel& operator=(const PoolModel&);

    void update();

  private:
    const int width_;
    const int height_;
    Particle** particles_;
};

#endif
