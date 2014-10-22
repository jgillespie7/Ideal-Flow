#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include "points.h"

int init(){
  SDL_Init( SDL_INIT_VIDEO );
}
int finish(){
  SDL_Quit();
}
int delay(int ms){
  SDL_Delay(ms);
}

class Plot{
    SDL_Window* window;
    SDL_Renderer* renderer;
    float xmin, xmax, ymin, ymax;
    int width, height;
  public:
    Plot (int, int, float, float, float, float);
    ~Plot();
    int point (float, float);
    int line (float, float, float, float);
    int polyline(PointList);
    int pointlist(PointList);
    int color(int, int, int, int);
    int update ();
    int fill ();
    int x2col (float);
    int y2row (float);
};
Plot::Plot(int w, int h, float a, float b, float c, float d){
  width = w;
  height = h;
  xmin = a;
  xmax = b;
  ymin = c;
  ymax = d;
  window = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderClear( renderer );
}
Plot::~Plot(){
    SDL_DestroyRenderer(renderer);
    if (window != NULL){
      SDL_DestroyWindow(window);
    }
}
int Plot::x2col(float x){
  return width * (x-xmin)/(xmax-xmin);
}
int Plot::y2row(float y){
  return height * (ymax-y)/(ymax-ymin);
}
int Plot::point(float x, float y){
  int col = x2col(x);
  int row = y2row(y);
  SDL_RenderDrawPoint( renderer, col, row);
}
int Plot::line(float x1, float y1, float x2, float y2){
  int col1 = x2col(x1);
  int row1 = y2row(y1);
  int col2 = x2col(x2);
  int row2 = y2row(y2);
  SDL_RenderDrawLine( renderer, col1, row1, col2, row2);
}
int Plot::update(){
  SDL_RenderPresent( renderer );
}
int Plot::fill(){
  SDL_RenderClear( renderer );
}
int Plot::color(int r, int g, int b, int a){
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
int Plot::pointlist(PointList points){
  int i;
  int n = points.length();
  for (i=0; i<n; i++){
    point(points.getX(i), points.getY(i));
  }
}
int Plot::polyline(PointList points){
  int i;
  int n = points.length();
  for (i=0; i<n-1; i++){
    if (isfinite(points.getX(i)), isfinite(points.getY(i)), isfinite(points.getX(i+1)), isfinite(points.getY(i+1))){
      line(points.getX(i), points.getY(i), points.getX(i+1), points.getY(i+1));
    }
  }
}
