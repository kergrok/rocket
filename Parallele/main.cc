#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"
#include <fstream>
#include <iostream>
//#include "omp.h"

using namespace std;

int main()
{
  Mesh* mesh = new Mesh("test.mesh");
  mesh->readmesh();

  mesh->initialize();
  mesh->compute();

  /*# pragma omp parallel num_threads(3)
  {
    me=omp_get_thread_num();
    cout << "coucou" << endl;
    Np=omp_get_num_threads();
    i1_iN = mesh->charge (me, Np);
  }*/

  /*system("rm -r Resultats/");
  system("mkdir Resultats");
  mesh->ReadParameter();
  cout << "-------------------Initialisation---------------------" << endl;
  double omega=_rho*_surf_tot/_N;
  double Mp=4.65*pow(10,-23);
  cout << "Mach = " << _Ma
  << "; rho = " << _rho
  << "; température = " << _T << endl;
  cout << "temps final: " << _Temps_final << endl;
  mesh->Buildvoisins();
  mesh->Build_Center_Norm();
  double dt = mesh->CFL();
  cout << "pas de temps: " << dt << endl;
  mesh->Create_in_Flow();
  double tau = mesh->Create_tau();
  cout << "tau : " << tau << endl;
  for (size_t i=0; i<_maille.size();i++)
  {
    mesh->Calc_prop(i);
  }
  write("Resultats/solDens0.inp","Resultats/solTemp0.inp","Resultats/solVelo0.inp");*/



  /*int me;
  int Np = 3;
  vector<int> i1_iN(2);

  double t = 0.;
  cout << "--------------------Computing-----------------------" << endl;
  int k = 0;

  double tau = mesh->Get_tau();
  double dt_inj = 0.1*tau;
  double dt = mesh->Get_dt();
  double Temps_final = mesh->Get_Temps_final();
  int nb_mailles = mesh->Get_nb_mailles();

  while (t<Temps_final)
  {
    k++;
    cout << "-----------------------------"<< endl;
    cout << "t= " << t << endl;
    cout << "it = " << k << endl;
    # pragma omp parallel num_threads(Np) private(me, Np, i1_iN)
    {
      me=omp_get_thread_num();
      Np=omp_get_num_threads();
      cout << "me : " << me << endl;
      cout << "Np : " << Np << endl;
      i1_iN = mesh->charge (me, Np);

      mesh->Displacement(i1_iN[0], i1_iN[1]);
      mesh->MajMailleParticule(i1_iN[0], i1_iN[1]);
    }
    for (size_t i=0; i<nb_mailles;i++)
    {
      mesh->Calc_prop(i);
    }
    mesh->write("Resultats/solDens"+to_string(k)+".inp","Resultats/solTemp"+to_string(k)+".inp","Resultats/solVelo"+to_string(k)+".inp");

    mesh->Create_in_Flow();
    cout << "inflow" << endl;
    t+=dt;
  }*/
  return 0;
}
