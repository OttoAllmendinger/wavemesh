#ifndef  _wavesim_h_
#define  _wavesim_h_

struct s_particle {
  float  s, v, ds, k, d, i;  
  int    n_links;
  int    n_amp;
  float  *l_amp;
  s_particle *links[4];    
};
struct s_coord {
  float x, y, *z;
  s_particle *p;
};

struct s_emitter {
  float w,a,t,ttl;
  int x,y;
};

typedef struct s_coord    Coord;
typedef struct s_particle Particle;
typedef struct s_emitter  Emitter;
typedef float s_particle::* p_value;
// typedef struct s_coords   Coords;

class wave_sim {
  public:
    wave_sim(int x, int y);
    ~wave_sim();
    void draw();
    void dump_pool();
    void update();
//     void setOmega(float k);
//     void setC    (float k);
//     void setD    (float k);
    void disturb();
    Coord*     mesh;
    Particle** w_particles;
    int        n_w_particles;
    int        n_mesh_nodes;
    int        width, height; 
    
    void set_particle(int x, int y, float k, float d);
    void add_emitter(int, int, float, float, float, float);
    void set_samples(int);
    void init();
    void resize(int, int);
    float w, c, d;

    float g_emitter_a;
    float g_emitter_w;
    float g_emitter_t;
    
    bool override_emitter;
    bool do_update_emitter;
    bool capture_intensity;
    int  n_samples;
    int  n_sample;

    Particle** pool;
  
  private:
    
    bool  pause;
    
    Emitter*   w_emitter;
    int        n_emitter;
    void alloc_pool(int, int);    
    void create_mesh();
    void create_pool();
    void connect_particles();
    void create_w_particles();
    void update_emitter();
    void barrier_particle(int, int);    
    void block_particle(int, int);    
};

#endif
