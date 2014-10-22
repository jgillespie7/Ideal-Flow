double uniform(double x, double y, double vinf, double alpha){
  return x*sin(-alpha)+y*cos(-alpha);
}

double source(double x, double y, double q, double x0, double y0){
  return q/(2*PI)*atan2(y-y0,x-x0);
}

double vortex(double x, double y, double gamma, double x0, double y0){
  double r = sqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) );
  return (-gamma)/(2*PI)*log(r);
}

double doublet(double x, double y, double mu, double beta, double x0, double y0){
  //doesn't use beta yet
  return mu/(2*PI)*y/( (x-x0)*(x-x0) + (y-y0)*(y-y0) );
}

double streamfunction(double x, double y){
  float alpha = .5;
//  return uniform(x, y, 1, .1)+source(x, y, 2, 0,0);
//  return source(x,y,1,0,0);//+doublet(x, y, 1, 0, 0, 0);
//  return source(x, y, 1, -2, 0) + source(x, y, -1, 2, 0) + uniform(x, y, 1, .2);
//  return source(x, y, 1, 0, 3) + vortex(x, y, -PI*1*1*sin(alpha), 0, 0) + uniform(x, y, 1, alpha);
  double r2 = x*x + y*y;
  double theta = atan2(y, x);
  return -1/(2*r2) * sin(-2*theta);
}

void polar2cartesian(PointList& polarPointList){
  int n = polarPointList.length();
  PointList cartesianPointList(n);
  int i;
  float r, theta;
  for (i=0; i<n; i++){
    r = polarPointList.getX(i);
    theta = polarPointList.getY(i);
    cartesianPointList.addPoint(r*cos(theta), r*sin(theta));
  }
//  polarPointList = cartesianPointList;
}
