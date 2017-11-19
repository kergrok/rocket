#include <vector>
#include <string>
#include <fstream>

#include "Mesh.h"
#include "Dense"

using namespace std;


// Ecrit dans le fichier "Nom_Fichier"



void Mesh::write(string Nom_Fichier)
{
  ofstream fichier(Nom_Fichier, ios::out | ios::trunc);

  if(fichier)
        {
          fichier<< _mpoint.size()<<" "<< _maille.size()<<" 0 1 0"<< endl;
          for(int i=0;i<_mpoint.size();i++)
          {
            fichier<<i+1<<" "<<_mpoint[i].Getcoor()[0]<<" "<< _mpoint[i].Getcoor()[1]<<" 0"<<endl;
          }

          for(int i=0;i<_maille.size();i++)
          {
            fichier<< i+1<< " 0 quad "<< _mquad[i].Getquadp()[0]+1<<" "<< _mquad[i].Getquadp()[1]+1<<" "<<_mquad[i].Getquadp()[2]+1<<" "<< _mquad[i].Getquadp()[3]+1<<endl;
          }
          fichier<<"1 1"<<endl;
          fichier<<"sol, NA"<<endl;
          for(int i=0;i<_maille.size();i++)
          {
            fichier<<i+1<<" "<<_maille[i].Getdensity()<<endl;
          }

        }

        fichier.close();
}
