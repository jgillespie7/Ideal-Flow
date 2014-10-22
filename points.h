#include <complex>
using namespace std;

typedef double (*double2double)(double);
typedef double (*pair2double)(double, double);
typedef complex<double> cplx;
typedef cplx (*cplx2cplx)(cplx);

double root(double2double f, double y, double t);

double contour(pair2double f, double y, double z, double yinit, double t);
double fdy(pair2double, double, double);
double fdx(pair2double, double, double);

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

