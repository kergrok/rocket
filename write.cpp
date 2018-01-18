#include <vector>
#include <string>
#include <fstream>

#include "Mesh.h"
#include "Dense"

using namespace std;


// Ecrit dans le fichier "Nom_Fichier"



void Mesh::write(string Nom_Fichier, string Nom_Fichier1, string Nom_Fichier2)
{
  ofstream fichier(Nom_Fichier, ios::out | ios::trunc);

  if(fichier)
        {
          fichier<< _mpoint.size()<<" "<< _maille.size()<<" 0 1 0"<< endl;
          for(size_t i=0;i<_mpoint.size();i++)
          {
            fichier<<i+1<<" "<<_mpoint[i].Getcoor()[0]<<" "<< _mpoint[i].Getcoor()[1]<<" 0"<<endl;
          }

          for(size_t i=0;i<_maille.size();i++)
          {
            fichier<< i+1<< " 0 quad "<< _mquad[i].Getquadp()[0]+1<<" "<< _mquad[i].Getquadp()[1]+1<<" "<<_mquad[i].Getquadp()[2]+1<<" "<< _mquad[i].Getquadp()[3]+1<<endl;
          }
          fichier<<"1 1"<<endl;
          fichier<<"dens, NA"<<endl;
          for(size_t i=0;i<_maille.size();i++)
          {
            fichier<<i+1<<" "<<_maille[i].Getdensity()<<endl;
          }
        }

  fichier.close();

  ofstream fichier1(Nom_Fichier1, ios::out | ios::trunc);

  if(fichier1)
        {
          fichier1<< _mpoint.size()<<" "<< _maille.size()<<" 0 1 0"<< endl;
          for(size_t i=0;i<_mpoint.size();i++)
          {
            fichier1<<i+1<<" "<<_mpoint[i].Getcoor()[0]<<" "<< _mpoint[i].Getcoor()[1]<<" 0"<<endl;
          }

          for(size_t i=0;i<_maille.size();i++)
          {
            fichier1<< i+1<< " 0 quad "<< _mquad[i].Getquadp()[0]+1<<" "<< _mquad[i].Getquadp()[1]+1<<" "<<_mquad[i].Getquadp()[2]+1<<" "<< _mquad[i].Getquadp()[3]+1<<endl;
          }
          fichier1<<"1 1"<<endl;
          fichier1<<"temp, NA"<<endl;
          for(size_t i=0;i<_maille.size();i++)
          {
            fichier1<<i+1<<" "<<_maille[i].Gettemp()<<endl;
          }

        }

  fichier1.close();

  ofstream fichier2(Nom_Fichier2, ios::out | ios::trunc);

  if(fichier2)
        {
          
          fichier2<< _mpoint.size()<<" "<< _maille.size()<<" 0 1 0"<< endl;
          for(size_t i=0;i<_mpoint.size();i++)
          {
            fichier2<<i+1<<" "<<_mpoint[i].Getcoor()[0]<<" "<< _mpoint[i].Getcoor()[1]<<" 0"<<endl;
          }

          for(size_t i=0;i<_maille.size();i++)
          {
            fichier2<< i+1<< " 0 quad "<< _mquad[i].Getquadp()[0]+1<<" "<< _mquad[i].Getquadp()[1]+1<<" "<<_mquad[i].Getquadp()[2]+1<<" "<< _mquad[i].Getquadp()[3]+1<<endl;
          }
          fichier2<<"1 1"<<endl;
          fichier2<<"velo, NA"<<endl;
          for(size_t i=0;i<_maille.size();i++)
          {
            fichier2<<i+1<<" "<<sqrt(pow(_maille[i].Getaverage()[0],2)+pow(_maille[i].Getaverage()[1],2)+pow(_maille[i].Getaverage()[2],2))<<endl;
          }

        }

  fichier2.close();
}
