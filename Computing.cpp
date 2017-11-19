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
  Buildvoisins();
  CFL();
  Create_in_Flow();
}

void Mesh::compute()
{
  double t;
  t=0.;

  cout << _Temps_final << endl;
  cout << _dt << endl;
  int k = 0;

  while (t<_Temps_final)
  {
    k++;
    Displacement();
    MajMailleParticule();
    for (int i=0; i<_maille.size();i++)
    {
      Calc_prop(i);
    }
    write("solDens"+to_string(k)+".inp");
    Create_in_Flow();
    t+=_dt;
  }
}
