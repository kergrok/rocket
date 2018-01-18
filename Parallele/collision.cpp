#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;


void Mesh::collision(){
  for (int i = 0; i < _part.size(); i++) {
    collision_or_not = rand() % 10;
    if (collision_or_not == 0 ) {
      int maille = _part[i].Getref();
      double temp = _maille[maille].Gettemp();
      Vector3d vit_moy = _maille[maille].Getaverage();

      Vector3d Vitesse;
      Vitesse[0] = vit_moy[0]+sqrt(287*temp)*alea(0,1);
      Vitesse[1] = vit_moy[1]+sqrt(287*temp)*alea(0,1);
      Vitesse[2] = vit_moy[2]+sqrt(287*temp)*alea(0,1); 

      _part[i].Modifyvelo(Vitesse);
    }
  }
}
