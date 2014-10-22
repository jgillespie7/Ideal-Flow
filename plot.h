#include <SDL2/SDL.h>
#include "points.h"

int init();
int finish();
int delay(int);

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
