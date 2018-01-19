#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;



void Mesh::Calc_prop(int i)
{
  double surface = _maille[i].Getsurf();
  vector<int> part = _maille[i].Getindices();
  int nb_part = part.size();

  double density = nb_part * _omega / surface;
  _maille[i].Modifydensity(density);

  vector<double> velocity_moy;
  velocity_moy.resize(3);
  velocity_moy[0]=0.;
  velocity_moy[1]=0.;
  velocity_moy[2]=0.;
  for (int j = 0; j < nb_part; j++)
  {
    for(int k=0; k<3;k++)
    {
      velocity_moy[k] += _part[part[j]].Getvelo()[k];
    }
  }

  for(int k=0; k<3;k++)
  {
    if(nb_part!=0)
      velocity_moy[k] = velocity_moy[k]/nb_part;
    else
      velocity_moy[k]=0;
  }

  _maille[i].Modifyaverage(velocity_moy);
  double temp = 0;
  double R = 287;
  for (int j = 0; j < nb_part; j++)
  {
    temp += pow(_part[part[j]].Getvelo()[0]-velocity_moy[0],2);
    temp += pow(_part[part[j]].Getvelo()[1]-velocity_moy[1],2);
    temp += pow(_part[part[j]].Getvelo()[2]-velocity_moy[2],2);
  }
  if(nb_part!=0)
    temp = (1./3)*temp/(nb_part*R);
  else
    temp = 0;
  _maille[i].Modifytemp(temp);

  //cout << "maille : " << i << "; vitesse moyenne = " << sqrt(pow(velocity_moy[0],2)+pow(velocity_moy[1],2)+pow(velocity_moy[2],2)) << endl;
}



bool Mesh::Find_Maille(int i) // i numéro de la particule, true si il a trouvé la nouvelle maille
{

  bool is_found(false);

  //On cherche d'abord dans la maille où était la particule
  int maille_initiale=_part[i].Getref();

  //On récupère la position de la particule
  vector<double> Position;
  Position.resize(2);
  Position=_part[i].Getcoor();

  //On récupère les mailles voisines
  vector<int> mailles_a_tester(4);
  mailles_a_tester=_maille[maille_initiale].Getvoisins();

  // On commence à chercher dans les mailles ayant une arete commune
  if(not(is_in(maille_initiale,Position)))
  {
    for(int j=0; j<4; j++)
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

  // Si on a pas trouvé, on cherche dans les mailles ayant une arete commune avec les voisins
  vector<int> mailles_a_tester2(4);
  if(not(is_found))
  {
    // Pour chaque voisin...
    for(int k=0;k<4;k++)
    {
      // cout << mailles_a_tester[k] << endl;
      //... on cherche le voisin...
      if (mailles_a_tester[k]!=-1)
      {
        mailles_a_tester2=_maille[mailles_a_tester[k]].Getvoisins();
        // ... et on teste
        for(int j=0; j<4; j++)
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
  return is_found;
}


bool Mesh::is_in(int maille, vector<double> Position) // True si la particule est dans la maille
{

  vector<vector<double>> Normale;
  vector<vector<double>> Middle_Edge;
  vector<vector<double>> MiEdge_Middle;
  Normale.resize(2,vector<double>(2));
  Middle_Edge.resize(2,vector<double>(2));
  MiEdge_Middle.resize(2,vector<double>(2));
  vector<double> MiEdge_Position;
  MiEdge_Position.resize(2);

  vector<int> Aretes(4);



  if(maille==(-1))
  {
    return false;
  }

  // Aretes=_mquad[maille].Getquadv();
  //
  // //On récupère les numéros des sommets du quad testé
  // Vector4i Sommets;
  // Sommets=_mquad[maille].Getquadp();
  //
  // Coordonées du milieu du quad
  // Vector2d Middle;
  // Middle.setZero();
  //
  // for(int j=0; j<4;j++)
  // {
  //   Middle[0] += _mpoint[Sommets[j]].Getcoor()[0]/4;
  //   Middle[1] += _mpoint[Sommets[j]].Getcoor()[1]/4;
  // }

  // Pour chaque arete, on vérifie si le produit scalaire
  // Vecteur allant du centre de l'arete à la position de la particule, normale de l'arete
  // A le bon signe ie <0
  Normale=_mquad[maille].Getnormale();
  MiEdge_Middle=_mquad[maille].Getmidnorm();
  Middle_Edge=_mquad[maille].Getmiddle();
  for(int i=0;i<4;i++)
  {

    // Sommets[0]=_medge[Aretes[i]].Getedge()[0];
    // Sommets[1]=_medge[Aretes[i]].Getedge()[1];
    // Normale[0]=_mpoint[Sommets[0]].Getcoor()[1]-_mpoint[Sommets[1]].Getcoor()[1];
    // Normale[1]=-_mpoint[Sommets[0]].Getcoor()[0]+_mpoint[Sommets[1]].Getcoor()[0];
    // Middle_Edge[0]=(_mpoint[Sommets[0]].Getcoor()[0]+_mpoint[Sommets[1]].Getcoor()[0])/2;
    // Middle_Edge[1]=(_mpoint[Sommets[0]].Getcoor()[1]+_mpoint[Sommets[1]].Getcoor()[1])/2;
    // MiEdge_Middle[0]=Middle[0]-Middle_Edge[0];
    // MiEdge_Middle[1]=Middle[1]-Middle_Edge[1];

    MiEdge_Position[0]=Position[0]-Middle_Edge[i][0];
    MiEdge_Position[1]=Position[1]-Middle_Edge[i][1];

    // On vérifie le sens de la normale pour qu'elle soit vers l'extérieur
    // if(Normale[i][0]*MiEdge_Middle[i][0]+Normale[i][1]*MiEdge_Middle[i][1]>0)
    // {
    //   Normale[i][0]=-Normale[i][0];
    //   Normale[i][1]=-Normale[i][1];
    // }

    // On vérifie si la position est du bon coté de l'arete
    if(MiEdge_Position[0]*Normale[i][0]+MiEdge_Position[1]*Normale[i][1]>0)
    {
      return false;
    }

  }
  // Si on a pas trouvé d'arete fausse, c'est bon
  return true;
}


void Mesh::MajMailleParticule(int me, int i1, int iN)
{
  int ref_part;
  // Remet à 0 les particules présentes dans chaque maille
  for (size_t i = 0; i < _maille.size(); i++)
  {
    if(me==0)
      _maille[i].Majindices();
  }


  // Ajout des particules dans chaque maille
  for (size_t i = i1; i < iN; i++) {
    if (_TF[i] == true) {
      ref_part = _part[i].Getref();
      _maille[ref_part].Addindice(i);
    }
  }
}
