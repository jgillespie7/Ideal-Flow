#include "plot.h"
#include <stdio.h>
#include <math.h>
#include <complex>
#define PI 3.141592653589793
using namespace std;

complex<double> mappingfunction(complex<double> z){
  return pow(z, 1.5);
}

double a(double x, double y){
  return x+y;
}
double b(double x){
  return cos(x);
}

int main(){
  init();
  Plot plot(640, 640, -5, 5, -5, 5);
  float y, x;
  plot.color(255,255,255,255);
  plot.fill();
  plot.color(0,255,0,255);
  PointList list(a, 0, -5, .01, 100);
  plot.polyline(list);
  PointList list2 = list;
  list2.map(mappingfunction);
  plot.color(255,0,0,255);
  plot.polyline(list2);
  plot.update();
  getchar();
  finish();
}
