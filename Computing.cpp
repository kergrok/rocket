#include <vector>
#include <string>
#include "Mesh.h"
#include "Dense"
#include <fstream>
#include <iostream>

using namespace std;
using namespace Eigen;

void Mesh::initialize()
{
  ReadParameter();
  cout << "-------------------Initialisation---------------------" << endl;
  _omega=_rho*_surf_tot/_N;
  _Mp=4.65*pow(10,-23);
  cout << "Mach = " << _Ma
  << "; rho = " << _rho
  << "; température = " << _T << endl;
  cout << "temps final: " << _Temps_final << endl;
  Buildvoisins();
  CFL();
  cout << "pas de temps: " << _dt << endl;
  Create_in_Flow();
}

void Mesh::compute()
{
  double t;
  t=0.;
  cout << "--------------------Computing-----------------------" << endl;
  int k = 0;

  while (t<_Temps_final)
  {
    k++;
    cout << "t= " << t << endl;
    Displacement();
    MajMailleParticule();
    for (int i=0; i<_maille.size();i++)
    {
      Calc_prop(i);
    }
    write("Resultats/solDens"+to_string(k)+".inp");
    Create_in_Flow();
    t+=_dt;
  }
}
