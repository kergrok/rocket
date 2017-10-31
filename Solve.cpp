#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;

/* A terminer */
void Calc_density(int num_maille)
{
  double surface = _maille[i].Getsurf();
  Eigen::VectorXi part = _maille[i].Getindices();
  int nb_part = part.size();

  double density = nb_part * _omega / surface;

  Eigen::Vector3d velocity = _mpoint[part[0]].Getvelo();
  double velocity_moy = 0;
  for (int i = 0; i < nb_part; i++) {
    velocity = _mpoint[part[i]].Getvelo();

  }

}
