#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;
using namespace Eigen;

Eigen::Vector2d Mesh::Convert(double x, double y)        // Convertit cartésien en polaire
{
  // Le vecteur coor_cyl comporte les valeurs de r et theta
  Eigen::Vector2d coor_cyl;
  // Calcul de r
  coor_cyl[0] = sqrt(x*x+y*y);
  // Calcul de theta
  if ((x==0)&&(y>0))        // Domaine en x>=0 et y >=0
  {
    coor_cyl[1] = acos(1.0)/2;
  }
  else if ((x>0)&&(y<=0))
  {
    coor_cyl[1] = atan(y/x);
  }
  else
  {
    cout << "Coordonnées pas dans le domaine" << endl;
    abort;
  }
  return coor_cyl;
}

Eigen::Vector2d Mesh::Convertinv(double r, double theta)    // Convertit polaire en cartésien
{
  // Le vecteur coor_pol comporte les valeurs de x et y
  Eigen::Vector2d coor_pol;
  coor_pol[0] = r*cos(theta);
  coor_pol[1] = r*sin(theta);
  return coor_pol;
}

double Mesh::Angle(int i)     // Calcul angle incident et prends en entrée l'indice de la particule i
{
  double r,theta,phi;
  r,theta=Convert(_part[i].Getcoor()[0],_part[i].Getcoor()[1]);
  phi=-theta+atan((_part[i].Getvelo()[1])/(_part[i].Getvelo()[0]));
  return phi;
}
