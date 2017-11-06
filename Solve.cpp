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

void Mesh::Find_Maille(int i) // i numéro de la particule,
{
  bool is_found(false);

  //On cherche d'abord dans la maille où était la particule
  int maille_initiale=_part[i].Getref();
  //On récupère la position de la particule
  Vector2d Position;
  Position=_part[i].Getcoor();

  Vector4i mailles_a_tester;
  mailles_a_tester=_maille[maille_initiale].Getvoisins();

  if(not(is_in(maille_initiale,Position)))
  {
    for(int j=1; j<5; j++)
    {
      if(is_in(mailles_a_tester[j],Position))
      {
        _part[i].Modifyref(mailles_a_tester[j]);
        is_found=true;
      }
      if(is_found)
      {
        break;
      }
    }
  }
  else
  {
    is_found=true;
  }



  Vector2i mailles_a_tester2;
  if(not(is_found))
  {
    for(int k=1;j=5;j++)
    {
      mailles_a_tester2=_maille[maille_a_tester[k]].Getvoisins();
      for(int j=1; j<5; j++)
      {
        if(is_in(mailles_a_tester2[j],Position))
        {
          _part[i].Modifyref(mailles_a_tester2[j]);
          is_found=true;
        }
        if(is_found)
        {
          break;
        }
      }
      if(is_found)
      {
        break;
      }
    }
  }

}

bool Mesh::is_in(int maille, Vector2d Position)
{
  Vector2d Normale;
  Vector2i Sommets;
  Vector2d Middle_Edge;
  Vector2d MiEdge_Middle;
  Vector2d MiEdge_Position;

  Vector4i Aretes;
  Aretes=_mquad[maille].Getquadv();

  //On récupère les numéros des sommets du quad testé
  Vector4i Sommets;
  Sommets=_mquad[maille].Getquadp();

  Vector2d Middle;
  Middle.setZero();

  for(int j=1; j<5;j++)
  {
    Middle[0] += _mpoint[Sommets[j]].Getcoor()[0]/4;
    Middle[1] += _mpoint[Sommets[j]].Getcoor()[1]/4;
  }

  for(int i=1;i<5;i++)
  {

    Sommets[0]=_medge[Aretes[i]].Getedge()[0];
    Sommets[1]=_medge[Aretes[i]].Getedge()[1];


    Normale[0]=Sommets[0].Getcoor()[1]-Sommets[1].Getcoor()[1];
    Normale[1]=-Sommets[0].Getcoor()[1]+Sommets[1].Getcoor()[1];

    Middle_Edge[0]=(Sommets[0].Getcoor()[0]+Sommets[1].Getcoor()[0])/2;
    Middle_Edge[1]=(Sommets[0].Getcoor()[1]+Sommets[1].Getcoor()[1])/2;

    MiEdge_Middle[0]=Middle[0]-Middle_Edge[0];
    MiEdge_Middle[1]=Middle[1]-Middle_Edge[1];

    MiEdge_Position[0]=Position[0]-Middle_Edge[0];
    MiEdge_Position[1]=Position[1]-Middle_Edge[1];

    if(Normale[0]*MiEdge_Middle[0]+Normale[1]*MiEdge_Middle[1]>0)
    {
      Normale=-Normale;
    }

    if(MiEdge_Position[0]*Normale[0]+MiEdge_Position[0]*Normale[0]>0)
    {
      return false
    }


  }

  return true

}
