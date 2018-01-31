/**
 * \file Convert.cpp
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 25 janvier 2018
 *
 */

#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

vector<double> Mesh::Convert(double x, double y)        // Convertit cartésien en polaire
{
  // Le vecteur coor_cyl comporte les valeurs de r et theta
  vector<double> coor_cyl;
  coor_cyl.resize(2);
  // Calcul de r
  coor_cyl[0] = sqrt(x*x+y*y);
  // Calcul de theta
  if ((x==0)&&(y>0))        // Domaine en x>=0 et y >=0
  {
    coor_cyl[1] = acos(1.0)/2;
  }
  else if ((x>0)&&(y>=0))
  {
    coor_cyl[1] = atan(y/x);
  }
  else if ((y>=0)&&(x<0))
  {
    coor_cyl[1] = acos(-1)*0.5+atan(-x/y);
    cout << "********************   Theta x negatif : " << coor_cyl[1]<< endl;
  }
  else if ((y<0)&&(x>=0))
  {
    coor_cyl[1] = -atan(-y/x);
    cout << "*********************  Theta y negatif : " << coor_cyl[1]<< " x = "<< x << "y= " << y << endl;
  }
  else
  {
    cout << "Coordonnées pas dans le domaine " << x<<" "<<y<<endl;
    abort();
  }
  return coor_cyl;
}
