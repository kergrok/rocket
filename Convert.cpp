#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Eigen;

double Mesh::Convert(double x, double y)        // Convertit cartésien en polaire
{
  double r,theta;
  r= sqrt(x*x+y*y);
  if ((x==0)&&(y>0))        // Domaine en x>=0 et y >=0
  {
    theta= acos(1.0)/2;
  }
  else if ((x>0)&&(y<=0))
  {
    theta= atan(y/x);
  }
  else
  {
    cout << "Coordonnées pas dans le domaine" << endl;
    abort;
  }
  return r,theta;
}

double Mesh::Convertinv(double r, double theta)    // Convertit polaire en cartésien
{
  double x,y;
  x= r*cos(theta);
  y= r*sin(theta);
  return x,y;
}

double Mesh::Angle(int i)     // Calcul angle incident et prends en entrée l'indice de la particule i
{
  double r,theta,phi;
  r,theta=Convert(_part[i].Getcoor()[0],_part[i].Getcoor()[1]);
  phi=-theta+atan((_part[i].Getvelo()[1])/(_part[i].Getvelo()[0]));
  return phi;
}
