#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;


/*
void Mesh::collision()
{
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
*/

void Mesh::collision(){





  int collision_or_not;
  Vector3d esperance,variance1,variance2;
  Vector3d corr_moyenne;
  Vector3d corr_dev;
  Vector3d vitesse_moyenne;
  Vector3d Vitesse;


  //Boucle sur les mailles
  for (size_t i = 0; i < _maille.size(); i++) {

    esperance.setZero();
    variance1.setZero();
    variance2.setZero();

    // On obtient la vitesse moyenne dans la maille
    vitesse_moyenne=_maille[i].Getaverage();


    // On calcule la variance initiale de la vitesse dans la maille
    for (size_t j=0;j<_maille[i].Getindices().size();j++)
    {
      variance1[0]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[0]-vitesse_moyenne[0],2);
      variance1[1]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[1]-vitesse_moyenne[1],2);
      variance1[2]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[2]-vitesse_moyenne[2],2);
    }
    variance1/=_maille[i].Getindices().size();



    // On fait les collisions et on obtient la nouvelle vitesse moyenne
    for (size_t j=0;j<_maille[i].Getindices().size();j++)
    {
      collision_or_not = rand() % 10;
      if (collision_or_not == 0 ) {

        double temp = _maille[i].Gettemp();
        Vector3d vit_moy = _maille[i].Getaverage();

        Vitesse[0] = vit_moy[0]+sqrt(287*temp)*alea(0,1);
        Vitesse[1] = vit_moy[1]+sqrt(287*temp)*alea(0,1);
        Vitesse[2] = vit_moy[2]+sqrt(287*temp)*alea(0,1);

        _part[i].Modifyvelo(Vitesse);

        esperance+=Vitesse;

      }

    }
    esperance/=_maille[i].Getindices().size();

    // On calcule la nouvelle variance de la vitesse
    for (size_t j=0;j<_maille[i].Getindices().size();j++)
    {
      variance2[0]+=pow((_part[_maille[i].Getindices()[j]].Getvelo()[0]-esperance[0]),2);
      variance2[1]+=pow((_part[_maille[i].Getindices()[j]].Getvelo()[1]-esperance[1]),2);
      variance2[2]+=pow((_part[_maille[i].Getindices()[j]].Getvelo()[2]-esperance[2]),2);
    }
    variance2/=_maille[i].Getindices().size();


    // On calcule les corrections à apporter à la vitesse

    corr_dev[0]=variance1[0]/variance2[0];
    corr_dev[1]=variance1[1]/variance2[1];
    corr_dev[2]=variance1[2]/variance2[2];

    corr_moyenne[0]=_maille[i].Getaverage()[0]-sqrt(corr_dev[0])*esperance[0];
    corr_moyenne[1]=_maille[i].Getaverage()[1]-sqrt(corr_dev[1])*esperance[1];
    corr_moyenne[2]=_maille[i].Getaverage()[2]-sqrt(corr_dev[2])*esperance[2];



    // On fait les ajustements nécessaires sur la vitesse
    for (size_t j=0;j<_maille[i].Getindices().size();j++)
    {
      Vitesse[0]=_part[_maille[i].Getindices()[j]].Getvelo()[0]*sqrt(corr_dev[0]);
      Vitesse[1]=_part[_maille[i].Getindices()[j]].Getvelo()[1]*sqrt(corr_dev[1]);
      Vitesse[2]=_part[_maille[i].Getindices()[j]].Getvelo()[2]*sqrt(corr_dev[2]);

      _part[_maille[i].Getindices()[j]].Modifyvelo(Vitesse);
      _part[_maille[i].Getindices()[j]].Modifyvelo(_part[_maille[i].Getindices()[j]].Getvelo()+corr_moyenne);
    }
  }
}
