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
  _maille[i].Modifyaverage(velocity_moy);
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



bool Mesh::Find_Maille(int i) // i numéro de la particule, true si il a trouvé la nouvelle maille
{

  bool is_found(false);

  //On cherche d'abord dans la maille où était la particule
  int maille_initiale=_part[i].Getref();

  //On récupère la position de la particule
  Vector2d Position;
  Position=_part[i].Getcoor();

  //On récupère les mailles voisines
  Vector4i mailles_a_tester;
  mailles_a_tester=_maille[maille_initiale].Getvoisins();

  // On commence à chercher dans les mailles ayant une arete commune
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
    is_found=false;
  }

  // Si on a pas trouvé, on cherche dans les mailles ayant une arete commune avec les voisins
  Vector4i mailles_a_tester2;
  if(not(is_found))
  {
    // Pour chaque voisin...
    for(int k=1;k=5;k++)
    {
      //... on cherche le voisin...
      mailles_a_tester2=_maille[mailles_a_tester[k]].Getvoisins();
      // ... et on teste
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
  return is_found;
}


bool Mesh::is_in(int maille, Vector2d Position) // True si la particule est dans la maille
{

  Vector2d Normale;
  Vector2d Middle_Edge;
  Vector2d MiEdge_Middle;
  Vector2d MiEdge_Position;

  Vector4i Aretes;
  Aretes=_mquad[maille].Getquadv();

  //On récupère les numéros des sommets du quad testé
  Vector4i Sommets;
  Sommets=_mquad[maille].Getquadp();

  // Coordonées du milieu du quad
  Vector2d Middle;
  Middle.setZero();

  for(int j=1; j<5;j++)
  {
    Middle[0] += _mpoint[Sommets[j]].Getcoor()[0]/4;
    Middle[1] += _mpoint[Sommets[j]].Getcoor()[1]/4;
  }


  // Pour chaque arete, on vérifie si le produit scalaire
  // Vecteur allant du centre de l'arete à la position de la particule, normale de l'arete
  // A le bon signe ie <0

  for(int i=1;i<5;i++)
  {

    Sommets[0]=_medge[Aretes[i]].Getedge()[0];
    Sommets[1]=_medge[Aretes[i]].Getedge()[1];

    Normale[0]=_mpoint[Sommets[0]].Getcoor()[1]-_mpoint[Sommets[1]].Getcoor()[1];
    Normale[1]=-_mpoint[Sommets[0]].Getcoor()[0]+_mpoint[Sommets[1]].Getcoor()[0];

    Middle_Edge[0]=(_mpoint[Sommets[0]].Getcoor()[0]+_mpoint[Sommets[1]].Getcoor()[0])/2;
    Middle_Edge[1]=(_mpoint[Sommets[0]].Getcoor()[1]+_mpoint[Sommets[1]].Getcoor()[1])/2;

    MiEdge_Middle[0]=Middle[0]-Middle_Edge[0];
    MiEdge_Middle[1]=Middle[1]-Middle_Edge[1];

    MiEdge_Position[0]=Position[0]-Middle_Edge[0];
    MiEdge_Position[1]=Position[1]-Middle_Edge[1];

    // On vérifie le sens de la normale pour qu'elle soit vers l'extérieur
    if(Normale[0]*MiEdge_Middle[0]+Normale[1]*MiEdge_Middle[1]>0)
    {
      Normale=-Normale;
    }

    // On vérifie si la position est du bon coté de l'arete
    if(MiEdge_Position[0]*Normale[0]+MiEdge_Position[0]*Normale[0]>0)
    {
      return false;
    }

  }
  // Si on a pas trouvé d'arete fausse, c'est bon
  return true;

}
