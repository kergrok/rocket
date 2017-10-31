#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;

/* A terminer */
void Mesh::Calc_prop(int i)
{
  double surface = _maille[i].Getsurf();
  VectorXi part = _maille[i].Getindices();
  int nb_part = part.size();

  double density = nb_part * _omega / surface;
  _maille[i].Modifydensity(density);

  Vector3d velocity_moy;
  velocity_moy.setZero();
  for (int j = 0; j < nb_part; j++)
  {
    velocity_moy += _part[j].Getvelo();
    velocity_moy = _omega*velocity_moy/surface;
  }
  _maille[i].Modifyvelo(velocity_moy);
  double temp = 0;
  double R = 8.314;
  for (int j = 0; j < nb_part; j++)
  {
    temp += pow(_part[j].Getvelo()[0]-_maille[i].Getaverage()[0],2);
    temp += pow(_part[j].Getvelo()[1]-_maille[i].Getaverage()[1],2);
    temp += pow(_part[j].Getvelo()[2]-_maille[i].Getaverage()[2],2);
  }
  temp = (_omega/3)*temp/(surface*R);
  _maille[i].Modifytemp(temp);


}
