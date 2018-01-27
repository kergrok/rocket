/**
 * \file Computing.cpp
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 25 janvier 2018
 *
 */

#include <vector>
#include <string>
#include "Mesh.h"
#include <fstream>
#include <iostream>

using namespace std;

// Initialisation du maillage
void Mesh::initialize()
{
  // Création du fichier résultats
  system("rm -r Resultats/");
  system("mkdir Resultats");

  ReadParameter();

  cout << "-------------------Initialisation---------------------" << endl;
  int i=0;
  while(_maille[i].Getref()!=4)
  {
    i+=1;
  }
  _omega=_rho*_maille[i].Getsurf()/_N;
  cout<<_omega<<endl;
  _Mp=4.65*pow(10,-23);
  cout << "Mach = " << _Ma
  << "; rho = " << _rho
  << "; température = " << _T << endl;
  cout << "temps final: " << _Temps_final << endl;
  // Construction des voisins pour chaque maille
  Buildvoisins();
  // Pour chaque arêtes, calcul sa norme, son milieu et sa normale
  Build_Center_Norm();
  // Calcul de dt grâce à la "CFL"
  CFL();
  cout << "pas de temps: " << _dt << endl;
  // Entrée des premières particules dans le domaine
  Create_in_Flow();
  // Calcul temps caractéristique pour les injections
  Create_tau();
  cout << "tau : " << _tau << endl;
  // Calcul des propriétés de chaque maille
  for (size_t i=0; i<_maille.size();i++)
  {
    Calc_prop(i);
  }
  write("Resultats/solDens0.inp","Resultats/solTemp0.inp","Resultats/solVelo0.inp","Resultats/solVx0.inp","Resultats/solVy0.inp");
}

void Mesh::compute()
{
  double t;
  t=0.;
  cout << "--------------------Computing-----------------------" << endl;
  int k = 0;

  double dt_inj = 0.1*_tau;
  cout<< "dt injection : " << dt_inj << endl;
  if(dt_inj<_dt)
  {
   _dt=dt_inj;
 }
  else
  {
    _C=_dt/_tau;
  }
  int mesh_size=_maille.size();
  vector<double> rho_moy(mesh_size,0);
  vector<double> temp_moy(mesh_size,0);
  vector<double> Vx_moy(mesh_size,0);
  vector<double> Vy_moy(mesh_size,0);
  vector<double> Vz_moy(mesh_size,0);
  // boucle en temps
  while (t<_Temps_final)
  {
    k++;
    cout << "-----------------------------"<< endl;
    cout << "t= " << t << endl;
    cout << "it = " << k << endl;
    // Déplacement des particules
    Displacement();


    // Mise à jour des positions des particules
    MajMailleParticule();
    // Mise à jour des propriétés physiques dans chaque maille
    for (size_t i=0; i<_maille.size();i++)
    {
      Calc_prop(i);
    }
    collision();
    // Ecriture des propriétés physiques dans les fichiers résultats
    if(k%20==0)
    {
      cout<<"jécris"<<endl;
      write("Resultats/solDens"+to_string(k)+".inp","Resultats/solTemp"+to_string(k)+".inp","Resultats/solVelo"+to_string(k)+".inp",
      "Resultats/solVx"+to_string(k)+".inp","Resultats/solVy"+to_string(k)+".inp");
    }
    /********************************Pour la moyenne des résultats********************************/
    // vector<double> vitesse(3);
    // if(k>=3500&&k<3600)
    // {
    //   for(int i =0;i<mesh_size;i++)
    //   {
    //     rho_moy[i]+=_maille[i].Getdensity()/100;
    //     temp_moy[i]+=_maille[i].Gettemp()/100;
    //     vitesse=_maille[i].Getaverage();
    //     Vx_moy[i]+=vitesse[0]/100;
    //     Vy_moy[i]+=vitesse[1]/100;
    //     Vz_moy[i]+=vitesse[2]/100;
    //   }
    // }
    // //Ecriture des propriétés physiques dans les fichiers résultats moyenné
    // if(k==3599)
    // {
    //   cout<<"jécris"<<endl;
    //   for(int i =0;i<mesh_size;i++)
    //   {
    //     _maille[i].Modifydensity(rho_moy[i]);
    //     vitesse=_maille[i].Getaverage();
    //     vitesse[0]=Vx_moy[i];
    //     vitesse[1]=Vy_moy[i];
    //     vitesse[2]=Vz_moy[i];
    //     _maille[i].Modifyaverage(vitesse);
    //     _maille[i].Modifytemp(temp_moy[i]);
    //   }
    //   write("Resultats/solDens"+to_string(k)+".inp","Resultats/solTemp"+to_string(k)+".inp","Resultats/solVelo"+to_string(k)+".inp",
    //   "Resultats/solVx"+to_string(k)+".inp","Resultats/solVy"+to_string(k)+".inp");
    //   abort();
    // }


    // Insertion de nouvelles particules dans le domaine
    Create_in_Flow();

    t+=_dt;
  }
}
