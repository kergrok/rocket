#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"
#include <fstream>
#include <iostream>
#include "omp.h"


using namespace std;
using namespace Eigen;

void Mesh::initialize()
{
  system("rm -r Resultats/"); 
  system("mkdir Resultats");
  ReadParameter();
  cout << "-------------------Initialisation---------------------" << endl;
  _omega=_rho*_surf_tot/_N;
  _Mp=4.65*pow(10,-23);
  cout << "Mach = " << _Ma
  << "; rho = " << _rho
  << "; température = " << _T << endl;
  cout << "temps final: " << _Temps_final << endl;
  Buildvoisins();
  Build_Center_Norm();
  CFL();
  cout << "pas de temps: " << _dt << endl;
  Create_in_Flow();
  Create_tau();
  cout << "tau : " << _tau << endl;
  for (size_t i=0; i<_maille.size();i++)
  {
    Calc_prop(i);
  }
  write("Resultats/solDens0.inp","Resultats/solTemp0.inp","Resultats/solVelo0.inp");
}

void Mesh::compute()
{
  int me;
  int Np;
  vector<int> i1_iN(2);

  double t;
  t=0.;
  cout << "--------------------Computing-----------------------" << endl;
  int k = 0;

  double dt_inj = 0.1*_tau;

  while (t<_Temps_final)
  {
    k++;
    cout << "-----------------------------"<< endl;
    cout << "t= " << t << endl;
    cout << "it = " << k << endl;
    # pragma omp parallel num_threads(3) private(me, Np, i1_iN)

    {

      me=omp_get_thread_num();
      Np=omp_get_num_threads();
      i1_iN = charge (me, Np);

      Displacement(i1_iN[0], i1_iN[1]);
      MajMailleParticule(i1_iN[0], i1_iN[1]);
    }
    for (size_t i=0; i<_maille.size();i++)
    {
      Calc_prop(i);
    }
    write("Resultats/solDens"+to_string(k)+".inp","Resultats/solTemp"+to_string(k)+".inp","Resultats/solVelo"+to_string(k)+".inp");

    Create_in_Flow();
    cout << "inflow" << endl;
    t+=_dt;
  }
}
