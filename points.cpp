#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <complex>
#define PI 3.141592653589793
using namespace std;

typedef double (*double2double)(double);
typedef double (*pair2double)(double, double);
typedef complex<double> cplx;
typedef cplx (*cplx2cplx)(cplx);

double root(double2double f, double y, double t){
  //Solves equation f(x) = y for x with tolerance t
  double x1 = 0, x2 = 1, x = 0;
  while ( (f(x)-y) > t || (f(x)-y) < -t ){
    x = x1 - (f(x)-y) * (x1-x2) / (f(x1)-f(x2));
    x2 = x1;
    x1 = x;
  }
  return x;
}

double contour(pair2double f, double x, double z, double yinit, double t){
  //Newton's method
  double y = yinit;
  double h, d;
  double sqreps = 1.5e-8;
  int i=0;
  while ( fabs(f(x, y) - z) > t ){
    h = sqreps * y;
    d = (-f(x, y+2*h) + 8*f(x, y+h) - 8*f(x, y-h) + f(x, y-2*h))/ (2*h); //5-point stencil for df/dy
    y = y - (f(x, y)-z) / d;
    i++;
    if (i>500){ return NAN; }
  }
  return y;
}

double fdy(pair2double f, double x, double y){
  double h, dy;
  double sqreps = 1.5e-8;
  h = sqreps * y;
  return (-f(x, y+2*h) + 8*f(x, y+h) - 8*f(x, y-h) + f(x, y-2*h))/ (12*h); //5-point stencil for df/dy
}

double fdx(pair2double f, double x, double y){
  double h, dx;
  double sqreps = 1.5e-8;
  h = sqreps * x;
  return (-f(x+2*h, y) + 8*f(x+h, y) - 8*f(x-h, y) + f(x-2*h, y))/ (12*h); //5-point stencil for df/dx
}

class PointList{
    int n; //Number of points added
    int m; //Allocated size
    cplx* zlist;
  public:
    PointList ();
    PointList (int);
    PointList (double2double, float, float, int);
    PointList(double2double xfunction, double2double yfunction, float tmin, float tmax, int npoints);
    PointList(double2double rfunction, int npoints);
    PointList(pair2double f, double x0, double y0, double xmax, int npoints);
    PointList(PointList&);
    ~PointList ();
    int length();
    int addPoint(cplx);
    int addPoint(double, double);
    double getX(int);
    double getY(int);
    int map(cplx2cplx);
};
//Create an empty Pointlist with space for 1 ordered pair
PointList::PointList(){
  m = 1;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  n = 0;
}
//Create an empty Pointlist with space for "maxsize" ordered pairs
PointList::PointList(int maxsize){
  m = maxsize;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  n = 0;
}
//Create a PointList from a Cartesian function y(x)
PointList::PointList(double2double f, float xmin, float xmax, int npoints){
  m = npoints;
  n = 0;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  int i;
  double x = xmin;
  double xinc = (xmax-xmin)/(npoints-1);
  for (i=0; i<npoints; i++){
    addPoint(x, f(x));
    x += xinc;
  }
}
//Create a PointList from a parametric function x(t), y(t)
PointList::PointList(double2double xfunction, double2double yfunction, float tmin, float tmax, int npoints){
  m = npoints;
  n = 0;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  int i;
  float t = tmin;
  float tinc = (tmax-tmin)/(npoints-1);
  for (i=0; i<npoints; i++){
    addPoint(xfunction(t), yfunction(t));
    t += tinc;
  }
}
//Create a PointList from a polar function r(theta), 0 < theta < 2*pi
PointList::PointList(double2double rfunction, int npoints){
  m = npoints;
  n = 0;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  int i;
  float theta = 0;
  float thetainc = (2*PI)/(npoints-1);
  float r;
  for (i=0; i<npoints; i++){
    r = rfunction(theta);
    addPoint(r*cos(theta), r*sin(theta));
    theta += thetainc;
  }
}
//Create a Pointlist from a potential function f(x, y) = z
//The Pointlist will be a contour of the potential function, starting at (x0, y0)
//z = f(x0, y0)
PointList::PointList(pair2double f, double x0, double y0, double xinc, int npoints){
  m = npoints;
  n = 0;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  int i;
  double z = f(x0, y0);
  double x = x0, y = y0;
  for (i=0; i<npoints; i++){
    y = contour(f, x, z, y, .00001);
    addPoint(x, y);
    x += xinc;
//    x += xinc*fdx(f, x, y);
//    y += xinc*fdy(f, x, y);
  }
}
PointList::PointList(PointList &other){
  n = other.n;
  m = other.m;
  zlist = (cplx*) malloc(sizeof(cplx) * m);
  int i;
  for (i=0; i<n; i++){
    zlist[i] = other.zlist[i];
  }
}
PointList::~PointList(){
  free (zlist);
  zlist = NULL;
}
int PointList::length(){
  return n;
}
int PointList::addPoint(cplx z){
  if (n < m){
    zlist[n] = z;
    n++;
  }
  else{
    m = m*2;
    cplx* zlistnew = (cplx*) malloc(sizeof(cplx) * m); int i;
    for (i=0; i<n; i++){
      zlistnew[i] = zlist[i];
    }
    zlist = zlistnew;
    zlist[n] = z;
    n++;
  }
}
int PointList::addPoint(double x, double y){
  cplx z(x, y);
  if (n < m){
    zlist[n] = z;
    n++;
  }
  else{
    m = m*2;
    cplx* zlistnew = (cplx*) malloc(sizeof(cplx) * m); int i;
    for (i=0; i<n; i++){
      zlistnew[i] = zlist[i];
    }
    zlist = zlistnew;
    zlist[n] = z;
    n++;
  }
}
double PointList::getX(int i){
  if (i < n){
    return zlist[i].real();
  }
  else {
    fprintf(stderr, "Error: Index %d is out of range. The max index is %d\n", i, n-1);
    exit(-1);
  }
}
double PointList::getY(int i){
  if (i < n){
    return zlist[i].imag();
  }
  else {
    fprintf(stderr, "Error: Index %d is out of range. The max index is %d\n", i, n-1);
    exit(-1);
}
}
int PointList::map(cplx2cplx f){
  int i;
  for (i=0; i<n; i++){
    zlist[i] = f(zlist[i]);
  }
}
