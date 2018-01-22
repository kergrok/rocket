#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"
#include <fstream>
#include <iostream>

using namespace std;
using namespace Eigen;

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
  write("Resultats/solDens0.inp","Resultats/solTemp0.inp","Resultats/solVelo0.inp");
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
   _dt=dt_inj;
   double test_inj=_dt;
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
    if(k%10==0)
    {
      cout<<"jécris"<<endl;
      write("Resultats/solDens"+to_string(k)+".inp","Resultats/solTemp"+to_string(k)+".inp","Resultats/solVelo"+to_string(k)+".inp");
    }
    // Insertion de nouvelles particules dans le domaine
    if(test_inj>dt_inj)
    {
      Create_in_Flow();
      test_inj=_dt;
    }
    else
    {
      test_inj+=_dt;
    }

    t+=_dt;
  }
}
