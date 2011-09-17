#include "wave_sim.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define  debug //printf
#define PI 3.14159265

wave_sim::wave_sim(int x, int y) {
  debug("constructor\n");
  
  w = 1; c = 1; d = 1;
  
  override_emitter =false;
  do_update_emitter=true;  
  capture_intensity=false;
  n_samples = 0;
  g_emitter_t = 0.0;
  g_emitter_a = 0.2;
  g_emitter_w = PI/16.0;
  mesh=NULL;
  w_particles=NULL;
  w_emitter=NULL;
  pool=NULL;
  alloc_pool(x,y);  
  init();
  pause=false;
}

wave_sim::~wave_sim() {
  int x;
  for (x=0; x<width; x++)
    free(pool[x]);
  free(pool);  
  free(mesh);
  free(w_emitter);
  free(w_particles);
}

void wave_sim::alloc_pool(int nx, int ny) {  
  int x,y;
  if (pool) {
    debug("free pool %d %d\n", nx, ny);        
    for (x=0; x<width; x++) {
      for (y=0; y<height; y++) {
        if (pool[x][y].l_amp) {
          free(pool[x][y].l_amp);
          pool[x][y].l_amp=NULL;
        }
      }
      free(pool[x]);
    }
    free(pool);
    pool=NULL;
  }
  width=nx; height=ny;
  debug("alloc pool %dx%d\n", width, height);
  pool = (Particle **)malloc(width * sizeof(Particle*));
  for (x=0; x<width; x++)
    pool[x] = (Particle *)malloc(height * sizeof(Particle));    
    
  for (x=0; x<width; x++)
    for (y=0; y<height; y++)
      pool[x][y] = (Particle) {0,0,0,1,1,0,0,0,NULL,{}};
  
//   fill_pool();
}

void wave_sim::init() {  
  debug("init\n");  
  n_sample    = 0;
  n_w_particles=0;
  n_mesh_nodes= 0;
  n_emitter   = 0;
  
  if (w_emitter) {
    free(w_emitter);
    w_emitter=NULL;
  }
  
  connect_particles();
  create_w_particles();  
  create_mesh();
  set_samples(n_samples);
  
}

void wave_sim::create_mesh() {
  int x, y, n;
  
  float k = (width>height)?((float)width):((float)height);
  
  debug("creating mesh\n");
  if (mesh) {
     debug("free mesh\n");
     free(mesh);
     mesh=NULL;
  }
  debug("alloc mesh\n");
  mesh = (Coord *)malloc( (4 * width * height) * sizeof(Coord));
      
  #define tx(x) ((x-width /2.0)/k)
  #define ty(y) ((y-height/2.0)/k)
  n=0; 
  for (x=0; x<width-1; x++)
    for (y=0; y<height-1; y++) {
      mesh[n  ] = (Coord) { tx(x)  ,ty(y)  , &(pool[x  ][y  ].s), &(pool[x  ][y  ]) };      
      mesh[n+1] = (Coord) { tx(x+1),ty(y)  , &(pool[x+1][y  ].s), &(pool[x+1][y  ]) };
      mesh[n+2] = (Coord) { tx(x+1),ty(y+1), &(pool[x+1][y+1].s), &(pool[x+1][y+1]) };
      mesh[n+3] = (Coord) { tx(x  ),ty(y+1), &(pool[x  ][y+1].s), &(pool[x  ][y+1]) };
      n+=4; 
    }
  n_mesh_nodes=(4*(width-1)*(height-1));
  debug("done\n");
}


void wave_sim::connect_particles() {
  debug("connecting particles\n");
  int x,y;
  for (x=0; x<width; x++)
    for (y=0; y<height; y++) {
//       printf("(%d, %d) (%d, %d) \n", x,y,(x+width-1)%width,(y+width-1)%height);
      pool[x][y].links[0] = &pool[x][(y+1)%height];
      pool[x][y].links[1] = &pool[(x+1)%width][y];
      pool[x][y].links[2] = &pool[x][(y+height-1)%height];
      pool[x][y].links[3] = &pool[(x+width-1)%width][y];
      pool[x][y].n_links=4;
    }
//   for (y=1; y<width-1; y++) {
//     pool[0][y].links[3] = &pool[width][1];
//     pool[width-1][y].links[1] = &pool[1][y];    
//   }
}

void wave_sim::create_w_particles() {
  int x,y,n=0;
  if (w_particles) {
    debug("free w_particles\n");
    free(w_particles);
    w_particles=NULL;
  }
  debug("alloc w_particles\n");
  w_particles = (Particle **)malloc((1+height) * (1+width) * sizeof(Particle*));  
  
  for (x=0; x<width; x++)
    for (y=0; y<height; y++) {  // IF (x,y) E (wasserkarte) dann
      w_particles[n]=&pool[x][y];
      debug("%d %d - %d\n", x,y,n);
      n++;
    }
  n_w_particles = n;
  debug("%d particles\n", n_w_particles);
}

void wave_sim::set_samples(int n) {
  debug("alloc %d samples\n", n);
  int i,x,y;  
  for (x=0; x<width; x++)
    for (y=0; y<height; y++) {
      if (pool[x][y].l_amp)
        pool[x][y].l_amp = (float*)realloc(pool[x][y].l_amp, n*sizeof(float));
      else
        pool[x][y].l_amp = (float*)malloc(n*sizeof(float));
      for (i=0; i<n;i++)
        pool[x][y].l_amp[i]=0.0;
      pool[x][y].n_amp = n;
      pool[x][y].i = 0.0;
    }
  n_samples=n;
  n_sample=0;
}

void wave_sim::set_particle(int x, int y, float k, float d) {
//   debug("set_particle %.2f %.2f\n", k, d);
  pool[x][y].k = k;
  pool[x][y].d = d;
}

void wave_sim::add_emitter(int x, int y, float a, float w, float t, float ttl) {
  if (w_emitter) {
    w_emitter = (Emitter *)realloc(w_emitter, (n_emitter+1) * sizeof(Emitter));
  } else {
    w_emitter = (Emitter *)malloc(sizeof(Emitter));
  }
  w_emitter[n_emitter] = (Emitter) {w,a,t,ttl,x,y};
  pool[x][y].d=0; // ne art emitter-flag
  n_emitter++;
}

void wave_sim::update_emitter() {  
  Emitter *te;
  if (override_emitter) {    
    for (int n=0; n<n_emitter; n++) {
      te = &w_emitter[n];
      if ( (te->ttl>0) && (te->t > te->ttl) ) return;
      pool[te->x][te->y].s = g_emitter_a * sin(g_emitter_t);      
    }
    g_emitter_t += g_emitter_w;
  } else {
    for (int n=0; n<n_emitter; n++) {
      te = &w_emitter[n];
      if ( (te->ttl>0) && (te->t > te->ttl) ) return;
      pool[te->x][te->y].s = te->a * sin(te->t);
      te->t += te->w;
    }
  }
}

void wave_sim::update() {
  int n, n_link, n_sub;
  Particle* tp;
  debug("update\n");
  if (n_samples>0) {
    n_sample = (n_sample+1) % n_samples;
  }
  for (n=0; n<n_w_particles; n++) {
    tp = w_particles[n];
    debug("particle %d\n",n);
    if (0==tp->d)
      continue;
    tp->ds=tp->s;
    for (n_link=0; n_link<tp->n_links; n_link++) {
      tp->ds-=tp->links[n_link]->k*tp->links[n_link]->s/4;
    }
  }  
  for (n=0; n<n_w_particles; n++) {
    tp = w_particles[n];
    tp->v-=tp->ds*tp->k*w;
    tp->s+=tp->v*c;
    tp->v*=tp->d*d;
    tp->s*=tp->d*d;
    if (n_samples>0) {
      tp->l_amp[n_sample] = tp->i = tp->s*tp->s*5;
      for (n_sub=0; n_sub<n_samples; n_sub++)
        if (tp->l_amp[n_sub]>tp->i)
          tp->i=tp->l_amp[n_sub];
    }
  }
  if (do_update_emitter)
    update_emitter();
}

// void wave_sim::clear

void wave_sim::disturb() {
  pool[1+random()%(width-1)][1+random()%(height-1)].s=0.6;
}

void wave_sim::dump_pool() {
  int x, y;
  for (x=0; x<width; x++) {
    for (y=0; y<height; y++) {
      printf("(%d %d) %+.1f", x,y,pool[x][y].s);
    }
    printf("\n");
  }
  printf("\n");
}

void wave_sim::resize(int new_width, int new_height) {  
  debug("resize\n");
  alloc_pool(new_width, new_height);  
  init();
}