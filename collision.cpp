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
int collision_or_not = rand() % 10;
if (collision_or_not == 0 ) {
int maille = _part[i].Getref();
double temp = _maille[maille].Gettemp();
vector<double> vit_moy(3);
vit_moy = _maille[maille].Getaverage();
vector<double>  Vitesse(3);
Vitesse[0] = vit_moy[0]+sqrt(287*temp)*alea(0,1);
Vitesse[1] = vit_moy[1]+sqrt(287*temp)*alea(0,1);
Vitesse[2] = vit_moy[2]+sqrt(287*temp)*alea(0,1);

_part[i].Modifyvelo(Vitesse);
}
}
}
*/

void Mesh::collision()
{
  int collision_or_not;
  vector<double> esperance(3),variance1(3),variance2(3);
  vector<double> corr_moyenne(3);
  vector<double> corr_dev(3);
  vector<double> vitesse_moyenne(3);
  vector<double> Vitesse(3);
  int N_part;

  //Boucle sur les mailles
  for (size_t i = 0; i < _maille.size(); i++) {

    if(_maille[i].Getindices().size()!=0 && _maille[i].Getindices().size()!=1)
    {
      N_part=_maille[i].Getindices().size();
      for(int j=0;j<3;j++)
      {
        esperance[j]=0.0;
        variance1[j]=0.0;
        variance2[j]=0.0;
      }

      // On obtient la vitesse moyenne dans la maille
      vitesse_moyenne=_maille[i].Getaverage();

      // On calcule la variance initiale de la vitesse dans la maille
      for (size_t j=0;j<N_part;j++)
      {
        variance1[0]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[0]-vitesse_moyenne[0],2);
        variance1[1]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[1]-vitesse_moyenne[1],2);
        variance1[2]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[2]-vitesse_moyenne[2],2);
      }
      variance1[0]/=N_part;
      variance1[1]/=N_part;
      variance1[2]/=N_part;

      // cout<<i<<endl;
      // cout<<"Vitesse moyenne :"<<vitesse_moyenne[0]<<" "<<vitesse_moyenne[1]<<" "<<vitesse_moyenne[2]<<endl;
      // cout<<"Variance: "<<variance1[0]<<" "<<variance1[1]<<" "<<variance1[2]<<endl;

      // On fait les collisions et on obtient la nouvelle vitesse moyenne

      for (size_t j=0;j<N_part;j++)
      {
        collision_or_not = rand() ;
        if (collision_or_not <_C )
        {
          double temp = _maille[i].Gettemp();
          vector<double> vit_moy = _maille[i].Getaverage();

          Vitesse[0] = vit_moy[0]+sqrt(287*temp)*alea(0,1);
          Vitesse[1] = vit_moy[1]+sqrt(287*temp)*alea(0,1);
          Vitesse[2] = vit_moy[2]+sqrt(287*temp)*alea(0,1);

          _part[_maille[i].Getindices()[j]].Modifyvelo(Vitesse);
        }
        esperance[0]+=_part[_maille[i].Getindices()[j]].Getvelo()[0];
        esperance[1]+=_part[_maille[i].Getindices()[j]].Getvelo()[1];
        esperance[2]+=_part[_maille[i].Getindices()[j]].Getvelo()[2];

      }
      esperance[0]/=N_part;
      esperance[1]/=N_part;
      esperance[2]/=N_part;

      // On calcule la nouvelle variance de la vitesse
      for (size_t j=0;j<N_part;j++)
      {
        variance2[0]+=pow((_part[_maille[i].Getindices()[j]].Getvelo()[0]-esperance[0]),2);
        variance2[1]+=pow((_part[_maille[i].Getindices()[j]].Getvelo()[1]-esperance[1]),2);
        variance2[2]+=pow((_part[_maille[i].Getindices()[j]].Getvelo()[2]-esperance[2]),2);
      }
      variance2[0]/=N_part;
      variance2[1]/=N_part;
      variance2[2]/=N_part;

      // On calcule les corrections à apporter à la vitesse
      // SI IL Y A UNE PARTICULE? LA VARIANCE EST NULLE
      corr_dev[0]=variance1[0]/variance2[0];
      corr_dev[1]=variance1[1]/variance2[1];
      corr_dev[2]=variance1[2]/variance2[2];

      corr_moyenne[0]=_maille[i].Getaverage()[0]-sqrt(corr_dev[0])*esperance[0];
      corr_moyenne[1]=_maille[i].Getaverage()[1]-sqrt(corr_dev[1])*esperance[1];
      corr_moyenne[2]=_maille[i].Getaverage()[2]-sqrt(corr_dev[2])*esperance[2];

      // On fait les ajustements nécessaires sur la vitesse
      for (size_t j=0;j<N_part;j++)
      {
        Vitesse[0]=_part[_maille[i].Getindices()[j]].Getvelo()[0]*sqrt(corr_dev[0]);
        Vitesse[1]=_part[_maille[i].Getindices()[j]].Getvelo()[1]*sqrt(corr_dev[1]);
        Vitesse[2]=_part[_maille[i].Getindices()[j]].Getvelo()[2]*sqrt(corr_dev[2]);

        Vitesse[0]+=corr_moyenne[0];
        Vitesse[1]+=corr_moyenne[1];
        Vitesse[2]+=corr_moyenne[2];


        _part[_maille[i].Getindices()[j]].Modifyvelo(Vitesse);

      }
      variance1[0]=0;
      variance1[1]=0;
      variance1[2]=0;

      esperance[0]=0;
      esperance[1]=0;
      esperance[2]=0;

      for (size_t j=0;j<N_part;j++)
      {
        variance1[0]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[0]-vitesse_moyenne[0],2);
        variance1[1]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[1]-vitesse_moyenne[1],2);
        variance1[2]+=pow(_part[_maille[i].Getindices()[j]].Getvelo()[2]-vitesse_moyenne[2],2);

        esperance[0]+=_part[_maille[i].Getindices()[j]].Getvelo()[0];
        esperance[1]+=_part[_maille[i].Getindices()[j]].Getvelo()[1];
        esperance[2]+=_part[_maille[i].Getindices()[j]].Getvelo()[2];
      }
      variance1[0]/=N_part;
      variance1[1]/=N_part;
      variance1[2]/=N_part;

      esperance[0]/=N_part;
      esperance[1]/=N_part;
      esperance[2]/=N_part;

      // cout<<"Vitesse moyenne :"<<esperance[0]<<" "<<esperance[1]<<" "<<esperance[2]<<endl;
      // cout<<"Variance: "<<variance1[0]<<" "<<variance1[1]<<" "<<variance1[2]<<endl;
      // cout<<"-----------------------------------------------"<<endl;
    }
  }
}
