/**
 * \file Run.cpp
 * \author L.Bouet - S.Bouchex - A.Antimes - C.Gouin
 * \version 1.0
 * \date 25 janvier 2018
 *
 */

#include <vector>
#include <string>
#include "Mesh.h"
#include <iostream>

using namespace std;

// ---------------- Calcul dt grâce à la "CFL" ------------------
// CFL définie par u * dt << dx
void Mesh::CFL()
{
  // Recherche de la plus petite arête pour définir le dx
  double dx_min = _lenghts[0];
  for (int i = 1; i < _lenghts.size() ; i++) {
    if (_lenghts[i] < dx_min) {
      dx_min = _lenghts[i];
    }
  }

  // Pour que la CFL soit vérifiée, on définit _dt comme suit :
  _dt = dx_min/(5. * sqrt(_gamma*287*_T)*_Ma);
  _vitesse_max=dx_min/_dt;
}

void Mesh::Create_in_Flow()
{
  vector<int> Edges;
  Edges.resize(4);
  cout << "inflow" << endl;
  for(size_t i=0;i<_maille.size();i++)
  {
    if(_maille[i].Getref()== 4 || _maille[i].Getref()== 34 || _maille[i].Getref()== 14) //Positionnée sur le bord
    {
      vector<int> indices;
      indices=_maille[i].Getindices();
      for(int k=0;k<indices.size();k++)
      {
        _TF[indices[k]]=false;
      }
      Edges=_mquad[i].Getquadv();
      for(int j=0;j<4;j++)
      {
        if(_medge[Edges[j]].Getref()==4)
        {
          Create_particules(i,Edges[j],Edges);
        }
      }
    }
  }
}


void Mesh::Create_particules(int maille, int arete, vector<int> Edges)
{
  vector<double> Vitesse;
  Vitesse.resize(3);
  vector<double> Position, Position1, Position2;
  Position.resize(2);
  Position1.resize(2);
  Position2.resize(2);
  size_t j=0;


  for(int i=0; i<_N;i++)
  {

    while((j<_part.size()) && (_TF[j]) )
    {
      j=j+1;
    }

    if(j == _part.size())
    {
      Part part(Position,Vitesse,1);
      _part.push_back(part);
      _TF.push_back(true);
    }
    else
    {
      _TF[j]=true;
    }
    double t = (double)rand()/RAND_MAX;
    Position1[0] = (_mpoint[_medge[arete].Getedge()[0]].Getcoor()[0]*t+_mpoint[_medge[arete].Getedge()[1]].Getcoor()[0]*(1-t));
    Position1[1] = (_mpoint[_medge[arete].Getedge()[0]].Getcoor()[1]*t+_mpoint[_medge[arete].Getedge()[1]].Getcoor()[1]*(1-t));

    vector<double> Position3;
    Position3.resize(4);
    Position3[0]=0.;
    vector<double> test1,test2;
    test1.resize(2);
    test2.resize(2);
    for(int k=0;k<4;k++)
    {
      test1=_mpoint[_medge[Edges[k]].Getedge()[0]].Getcoor();
      test2=_mpoint[_medge[Edges[k]].Getedge()[1]].Getcoor();
      if((_mpoint[_medge[arete].Getedge()[0]].Getcoor()[0]!=test1[0])||(_mpoint[_medge[arete].Getedge()[0]].Getcoor()[1]!=test1[1]))
      {
        if((_mpoint[_medge[arete].Getedge()[1]].Getcoor()[0]!=test1[0])||(_mpoint[_medge[arete].Getedge()[1]].Getcoor()[1]!=test1[1]))
        {
          if ((Position3.size()<5)&&((Position3[0]!=test1[0])&&(Position3[1]!=test1[1])||(test1[1]==0.)))
          {
            if(Position3[0]==0.)
            {
              Position3[0]=test1[0];
              Position3[1]=test1[1];
            }
            else
            {
              Position3[2]=test1[0];
              Position3[3]=test1[1];
            }
          }
        }
      }
      if((_mpoint[_medge[arete].Getedge()[0]].Getcoor()[0]!=test2[0])||(_mpoint[_medge[arete].Getedge()[0]].Getcoor()[1]!=test2[1]))
      {
        if((_mpoint[_medge[arete].Getedge()[1]].Getcoor()[0]!=test2[0])||(_mpoint[_medge[arete].Getedge()[1]].Getcoor()[1]!=test2[1]))
        {
          if ((Position3.size()<5)&&((Position3[0]!=test2[0])&&(Position3[1]!=test2[1])||(test2[1]==0.)))
          {
            if(Position3[0]==0.)
            {
              Position3[0]=test2[0];
              Position3[1]=test2[1];
            }
            else
            {
              Position3[2]=test2[0];
              Position3[3]=test2[1];
            }
          }
        }
      }
    }
    if(Position3.size()!=4)
    cout << "pas trouvé les deux points ..." << endl;

    Position2[0]=Position3[0]*t+Position3[2]*(1-t);
    Position2[1]=Position3[1]*t+Position3[3]*(1-t);

    t = (double)rand()/RAND_MAX;
    Position[0]=Position1[0]*t+Position2[0]*(1-t);
    Position[1]=Position1[1]*t+Position2[1]*(1-t);

    Vitesse[0] = _Ma*sqrt(_gamma*287*_T)+sqrt(287*_T)*alea(0,1);
    Vitesse[1] = sqrt(287*_T)*alea(0,1);
    Vitesse[2] = sqrt(287*_T)*alea(0,1);

    _part[j].Modifyvelo(Vitesse);
    _part[j].Modifycoor(Position);
    _part[j].Modifyref(maille);

  }


}



// Déplace toutes les particules pour un pas de temps
// Ne met à jour que la position, la mise à jour de la référence de la maille se fait dans une autre fonction
// En commentaire la fonction pour éffectuer la mise à jour sur plusieurs pas de temps

void Mesh::Displacement()
{
  vector<double> new_coor,coor, vitesse, coor_ini;
  new_coor.resize(2);
  coor.resize(2);
  coor_ini.resize(2);
  vitesse.resize(3);
  bool in_domain;

  for (size_t i = 0; i < _part.size() ; i++) {

    if (_TF[i] == true) {
      int compt=0;
      coor_ini = _part[i].Getcoor();
      if(not(is_CFL_respected(_part[i].Getvelo())))
      {
        while(not(is_CFL_respected(_part[i].Getvelo()))&&compt<10)
        {
          cout<<"je ne respecte pas la CFL "<<compt<<endl;
          compt+=1;
          _part[i].Modifycoor(coor_ini);
          for(int k=0;k<(compt+1)*10;k++)
          {
            new_coor = _part[i].Getcoor();
            coor = _part[i].Getcoor();
            vitesse = _part[i].Getvelo();
            new_coor[0]-=vitesse[0]*_dt/((compt+1)*10);
            new_coor[1]-=vitesse[1]*_dt/((compt+1)*10);
            _part[i].Modifycoor(new_coor);
            in_domain = Find_Maille(i);

            if(not(in_domain))
            {
              find_impact(i,coor,new_coor);
            }
          }
        }

        if (compt==10)
        {
          _TF[i]=false;
        }
      }
      else
      {
        new_coor = _part[i].Getcoor();
        coor = _part[i].Getcoor();
        vitesse = _part[i].Getvelo();
        new_coor[0]-=vitesse[0]*_dt;
        new_coor[1]-=vitesse[1]*_dt;
        _part[i].Modifycoor(new_coor);
        in_domain = Find_Maille(i);



        if(not(in_domain))
        {
          find_impact(i,coor,new_coor);
        }
      }
    }
  }
}

bool Mesh::is_CFL_respected(vector<double> Vitesse)
{
  if(sqrt(pow(Vitesse[0],2)+pow(Vitesse[1],2))>_vitesse_max)
  return false;
  else
  return true;
}


void Mesh::find_impact(int i, vector<double> coor, vector<double> new_coor)
{

  double a1,c1,a2,c2;
  int ref_maille;
  vector<int> ref_edges;
  ref_edges.resize(4);
  int impact_edge;
  double distance_a_parcourir,distance_parcourue;
  vector<double> coorS1,coorS2,coorImpact,vector_edge,vector_deplacement;
  vector<int> sommets;
  coorS1.resize(2);     coorS2.resize(2);
  coorImpact.resize(2);    vector_edge.resize(2);
  vector_deplacement.resize(2);     sommets.resize(2);

  // Coefficients de la droite trajet de la particule
  a1=(coor[1]-new_coor[1])/(coor[0]-new_coor[0]);
  c1=coor[1]-coor[0]*a1;

  //Maille dans laquelle est la particule
  ref_maille = _part[i].Getref();
  // Numéro des aretes de la maille
  ref_edges = _mquad[ref_maille].Getquadv();

  if((_maille[_part[i].Getref()].Getref()!=12) && (_maille[_part[i].Getref()].Getref()!=23) && (_maille[_part[i].Getref()].Getref()!=14))
  {
    for(int j=0;j<4;j++)
    {
      if(_medge[ref_edges[j]].Getref() == 1)
      {
        impact_edge = ref_edges[j];
        // Coordonnées des sommets de l'arete par laquelle sort la particule
        sommets = _medge[impact_edge].Getedge();
        coorS1 = _mpoint[sommets[0]].Getcoor();
        coorS2 = _mpoint[sommets[1]].Getcoor();

        //Coefficient de la droite passant par l'arete
        if(coorS1[0]!=coorS2[0])
        {
          a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
          c2 = coorS1[1]-coorS1[0]*a2;

          // Coordonnées de l'impact
          coorImpact[0] = (c2-c1)/(a1-a2);
          coorImpact[1] = a1*coorImpact[0]+c1;
        }
        else
        {
          coorImpact[0]=coorS1[0];
          coorImpact[1]=a1*coorImpact[0]+c1;
        }

        // Calcul de distances
        distance_a_parcourir=Norme_entre(new_coor,coor);
        distance_parcourue=Norme_entre(coor,coorImpact);

        double dt_rest;
        vector<double> newvelo;
        newvelo.resize(3);
        // Nouvelle vitesse Vx=-Vx
        newvelo[0]=_part[i].Getvelo()[0];
        newvelo[1]=-_part[i].Getvelo()[1];
        newvelo[2]=_part[i].Getvelo()[2];
        // Calcul des nouvelles coor
        dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
        _part[i].Modifyvelo(newvelo);
        coorImpact[0]-=newvelo[0]*dt_rest;
        coorImpact[1]-=newvelo[1]*dt_rest;
        _part[i].Modifycoor(coorImpact);
      }
      if(_medge[ref_edges[j]].Getref() == 2)
      {
        impact_edge = ref_edges[j];
        // Coordonnées des sommets de l'arete par laquelle sort la particule
        sommets = _medge[impact_edge].Getedge();
        coorS1 = _mpoint[sommets[0]].Getcoor();
        coorS2 = _mpoint[sommets[1]].Getcoor();

        //Coefficient de la droite passant par l'arete
        if(coorS1[0]!=coorS2[0])
        {
          a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
          c2 = coorS1[1]-coorS1[0]*a2;

          // Coordonnées de l'impact
          coorImpact[0] = (c2-c1)/(a1-a2);
          coorImpact[1] = a1*coorImpact[0]+c1;
        }
        else
        {
          coorImpact[0]=coorS1[0];
          coorImpact[1]=a1*coorImpact[0]+c1;
        }

        // Calcul de distances
        distance_a_parcourir=Norme_entre(new_coor,coor);
        distance_parcourue=Norme_entre(coor,coorImpact);

        vector<double> cyl, velocyl;
        vector<double> newvelo;
        cyl.resize(2);     velocyl.resize(2);
        newvelo.resize(3);
        cyl=Convert(coorImpact[0],coorImpact[1]);       //coordonnées cylindrique du point d'impact
        // vitesse incidente en cylindrique
        velocyl[0]=_part[i].Getvelo()[0]*cos(cyl[1])+_part[i].Getvelo()[1]*sin(cyl[1]);
        velocyl[1]=_part[i].Getvelo()[1]*cos(cyl[1])-_part[i].Getvelo()[0]*sin(cyl[1]);
        // vitesse selon r = -vitesse selon r (rebond)  --> Nouvelle vitesse
        velocyl[0]=-velocyl[0];
        // passage en cartésien de la nouvelle vitesse
        newvelo[0]=velocyl[0]*cos(cyl[1])-velocyl[1]*sin(cyl[1]);
        newvelo[1]=velocyl[1]*cos(cyl[1])+velocyl[0]*sin(cyl[1]);
        newvelo[2]=_part[i].Getvelo()[2];
        // Calcul de la nouvelle coor
        double dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
        _part[i].Modifyvelo(newvelo);
        coorImpact[0]-=newvelo[0]*dt_rest;
        coorImpact[1]-=newvelo[1]*dt_rest;
        _part[i].Modifycoor(coorImpact);

      }
      if(_medge[ref_edges[j]].Getref() == 4)             // Si la particule sort du domaine
      {
        _TF[i]=false;
      }
      if(_medge[ref_edges[j]].Getref() == 3)
      {
        _TF[i]=false;
      }
    }
  }



  else if (_maille[_part[i].Getref()].Getref()==12)// L'erreur qui arete le programme est ici
  {
    vector<double> vect_dir,vect_lim,coor_coin;
    vect_dir.resize(2);    vect_lim.resize(2);  coor_coin.resize(2);
    vector<int> points;
    points.resize(4);
    points = _mquad[ref_maille].Getquadp();
    for(int j=0;j<4;j++)
    {
      if(_mpoint[points[j]].Getref()==40)
      coor_coin=_mpoint[points[j]].Getcoor();
    }
    vect_lim[0]=(coor_coin[0]-coor[0]);
    vect_lim[1]=(coor_coin[1]-coor[1]);
    double norm;
    norm=Norme(vect_lim);
    vect_lim[0]/=norm;
    vect_lim[1]/=norm;
    vect_dir[0]=(new_coor[0]-coor[0]);
    vect_dir[1]=(new_coor[1]-coor[1]);
    norm=Norme(vect_dir);
    vect_dir[0]/=norm;
    vect_dir[1]/=norm;
    if(vect_dir[1]<vect_lim[1]) // vers le bas 1
    {
      for(int j=0;j<4;j++)
      {
        if(_medge[ref_edges[j]].Getref() == 1)
        impact_edge = ref_edges[j];
      }
      // Coordonnées des sommets de l'arete par laquelle sort la particule
      sommets = _medge[impact_edge].Getedge();
      coorS1 = _mpoint[sommets[0]].Getcoor();
      coorS2 = _mpoint[sommets[1]].Getcoor();

      //Coefficient de la droite passant par l'arete
      if(coorS1[0]!=coorS2[0])
      {
        a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
        c2 = coorS1[1]-coorS1[0]*a2;

        // Coordonnées de l'impact
        coorImpact[0] = (c2-c1)/(a1-a2);
        coorImpact[1] = a1*coorImpact[0]+c1;
      }
      else
      {
        coorImpact[0]=coorS1[0];
        coorImpact[1]=a1*coorImpact[0]+c1;
      }

      // Calcul de distances
      distance_a_parcourir=Norme_entre(new_coor,coor);
      distance_parcourue=Norme_entre(coor,coorImpact);

      double dt_rest;
      vector<double> newvelo;
      newvelo.resize(3);
      // Nouvelle vitesse Vx=-Vx
      newvelo[0]=_part[i].Getvelo()[0];
      newvelo[1]=-_part[i].Getvelo()[1];
      newvelo[2]=_part[i].Getvelo()[2];
      // Calcul des nouvelles coor
      dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
      _part[i].Modifyvelo(newvelo);
      coor=coorImpact;
      coorImpact[0]-=newvelo[0]*dt_rest;
      coorImpact[1]-=newvelo[1]*dt_rest;
      _part[i].Modifycoor(coorImpact);
      new_coor=coorImpact;
      bool in_domain;
      in_domain = Find_Maille(i);
      if(not(in_domain))
      {
        for(int j=0;j<4;j++)
        {
          if(_medge[ref_edges[j]].Getref() == 2)
          impact_edge = ref_edges[j];
        }
        // Coordonnées des sommets de l'arete par laquelle sort la particule
        sommets = _medge[impact_edge].Getedge();
        coorS1 = _mpoint[sommets[0]].Getcoor();
        coorS2 = _mpoint[sommets[1]].Getcoor();

        //Coefficient de la droite passant par l'arete
        if(coorS1[0]!=coorS2[0])
        {
          a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
          c2 = coorS1[1]-coorS1[0]*a2;

          // Coordonnées de l'impact
          coorImpact[0] = (c2-c1)/(a1-a2);
          coorImpact[1] = a1*coorImpact[0]+c1;
        }
        else
        {
          coorImpact[0]=coorS1[0];
          coorImpact[1]=a1*coorImpact[0]+c1;
        }

        // Calcul de distances
        distance_a_parcourir=Norme_entre(new_coor,coor);
        distance_parcourue=Norme_entre(coor,coorImpact);

        vector<double> v, cyl, velocyl,newvelo;
        cyl.resize(2);   velocyl.resize(2);    newvelo.resize(3);
        cyl=Convert(coorImpact[0],coorImpact[1]);       //coordonnées cylindrique du point d'impact
        // vitesse incidente en cylindrique
        velocyl[0]=_part[i].Getvelo()[0]*cos(cyl[1])+_part[i].Getvelo()[1]*sin(cyl[1]);
        velocyl[1]=_part[i].Getvelo()[1]*cos(cyl[1])-_part[i].Getvelo()[0]*sin(cyl[1]);
        // vitesse selon r = -vitesse selon r (rebond)  --> Nouvelle vitesse
        velocyl[0]=-velocyl[0];
        // passage en cartésien de la nouvelle vitesse
        newvelo[0]=velocyl[0]*cos(cyl[1])-velocyl[1]*sin(cyl[1]);
        newvelo[1]=velocyl[1]*cos(cyl[1])+velocyl[0]*sin(cyl[1]);
        newvelo[2]=_part[i].Getvelo()[2];
        // Calcul de la nouvelle coor
        double dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
        _part[i].Modifyvelo(newvelo);
        coorImpact[0]-=newvelo[0]*dt_rest;
        coorImpact[1]-=newvelo[1]*dt_rest;
        _part[i].Modifycoor(coorImpact);
      }
    }
    else
    {
      for(int j=0;j<4;j++)
      {
        if(_medge[ref_edges[j]].Getref() == 2)
        impact_edge = ref_edges[j];
      }
      // Coordonnées des sommets de l'arete par laquelle sort la particule
      sommets = _medge[impact_edge].Getedge();
      coorS1 = _mpoint[sommets[0]].Getcoor();
      coorS2 = _mpoint[sommets[1]].Getcoor();

      //Coefficient de la droite passant par l'arete
      if(coorS1[0]!=coorS2[0])
      {
        a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
        c2 = coorS1[1]-coorS1[0]*a2;

        // Coordonnées de l'impact
        coorImpact[0] = (c2-c1)/(a1-a2);
        coorImpact[1] = a1*coorImpact[0]+c1;
      }
      else
      {
        coorImpact[0]=coorS1[0];
        coorImpact[1]=a1*coorImpact[0]+c1;
      }

      // Calcul de distances
      distance_a_parcourir=Norme_entre(new_coor,coor);
      distance_parcourue=Norme_entre(coor,coorImpact);

      vector<double> cyl, velocyl, newvelo;
      cyl.resize(2);    velocyl.resize(2);   newvelo.resize(3);
      cyl=Convert(coorImpact[0],coorImpact[1]);       //coordonnées cylindrique du point d'impact
      // vitesse incidente en cylindrique
      velocyl[0]=_part[i].Getvelo()[0]*cos(cyl[1])+_part[i].Getvelo()[1]*sin(cyl[1]);
      velocyl[1]=_part[i].Getvelo()[1]*cos(cyl[1])-_part[i].Getvelo()[0]*sin(cyl[1]);
      // vitesse selon r = -vitesse selon r (rebond)  --> Nouvelle vitesse
      velocyl[0]=-velocyl[0];
      // passage en cartésien de la nouvelle vitesse
      newvelo[0]=velocyl[0]*cos(cyl[1])-velocyl[1]*sin(cyl[1]);
      newvelo[1]=velocyl[1]*cos(cyl[1])+velocyl[0]*sin(cyl[1]);
      newvelo[2]=_part[i].Getvelo()[2];
      // Calcul de la nouvelle coor
      double dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
      _part[i].Modifyvelo(newvelo);
      coor=coorImpact;
      coorImpact[0]-=newvelo[0]*dt_rest;
      coorImpact[1]-=newvelo[1]*dt_rest;
      _part[i].Modifycoor(coorImpact);
      new_coor=coorImpact;
      bool in_domain;
      in_domain = Find_Maille(i);
      if(not(in_domain))
      {
        for(int j=0;j<4;j++)
        {
          if(_medge[ref_edges[j]].Getref() == 1)
          impact_edge = ref_edges[j];
        }
        // Coordonnées des sommets de l'arete par laquelle sort la particule
        sommets = _medge[impact_edge].Getedge();
        coorS1 = _mpoint[sommets[0]].Getcoor();
        coorS2 = _mpoint[sommets[1]].Getcoor();

        //Coefficient de la droite passant par l'arete
        if(coorS1[0]!=coorS2[0])
        {
          a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
          c2 = coorS1[1]-coorS1[0]*a2;

          // Coordonnées de l'impact
          coorImpact[0] = (c2-c1)/(a1-a2);
          coorImpact[1] = a1*coorImpact[0]+c1;
        }
        else
        {
          coorImpact[0]=coorS1[0];
          coorImpact[1]=a1*coorImpact[0]+c1;
        }

        // Calcul de distances
        distance_a_parcourir=Norme_entre(new_coor,coor);
        distance_parcourue=Norme_entre(coor,coorImpact);

        double dt_rest;
        vector<double> newvelo;
        newvelo.resize(3);
        // Nouvelle vitesse Vx=-Vx
        newvelo[0]=_part[i].Getvelo()[0];
        newvelo[1]=-_part[i].Getvelo()[1];
        newvelo[2]=_part[i].Getvelo()[2];
        // Calcul des nouvelles coor
        dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
        _part[i].Modifyvelo(newvelo);
        coorImpact[0]-=newvelo[0]*dt_rest;
        coorImpact[1]-=newvelo[1]*dt_rest;
        _part[i].Modifycoor(coorImpact);
      }
    }
  }



  else if (_maille[_part[i].Getref()].Getref()==23)
  {
    vector<double> vect_dir,vect_lim,coor_coin;
    vect_dir.resize(2);  vect_lim.resize(2); coor_coin.resize(2);
    vector<int> points;
    points.resize(4);
    points = _mquad[ref_maille].Getquadp();
    for(int j=0;j<4;j++)
    {
      if(_mpoint[points[j]].Getref()==50)
      coor_coin=_mpoint[points[j]].Getcoor();
    }
    vect_lim[0]=(coor_coin[0]-coor[0]);
    vect_lim[1]=(coor_coin[1]-coor[1]);
    double norm;
    norm=Norme(vect_lim);
    vect_lim[0]/=norm;
    vect_lim[1]/=norm;
    vect_dir[0]=(new_coor[0]-coor[0]);
    vect_dir[1]=(new_coor[1]-coor[1]);
    norm=Norme(vect_dir);
    vect_dir[0]/=norm;
    vect_dir[1]/=norm;
    if(vect_dir[1]<vect_lim[1])
    {
      for(int j=0;j<4;j++)
      {
        if(_medge[ref_edges[j]].Getref() == 2)
        impact_edge = ref_edges[j];
      }
      // Coordonnées des sommets de l'arete par laquelle sort la particule
      sommets = _medge[impact_edge].Getedge();
      coorS1 = _mpoint[sommets[0]].Getcoor();
      coorS2 = _mpoint[sommets[1]].Getcoor();

      //Coefficient de la droite passant par l'arete
      if(coorS1[0]!=coorS2[0])
      {
        a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
        c2 = coorS1[1]-coorS1[0]*a2;

        // Coordonnées de l'impact
        coorImpact[0] = (c2-c1)/(a1-a2);
        coorImpact[1] = a1*coorImpact[0]+c1;
      }
      else
      {
        coorImpact[0]=coorS1[0];
        coorImpact[1]=a1*coorImpact[0]+c1;
      }

      // Calcul de distances
      distance_a_parcourir=Norme_entre(new_coor,coor);
      distance_parcourue=Norme_entre(coor,coorImpact);

      vector<double> cyl, velocyl,newvelo;
      cyl.resize(2);   velocyl.resize(2);   newvelo.resize(3);
      cyl=Convert(coorImpact[0],coorImpact[1]);       //coordonnées cylindrique du point d'impact
      // vitesse incidente en cylindrique
      velocyl[0]=_part[i].Getvelo()[0]*cos(cyl[1])+_part[i].Getvelo()[1]*sin(cyl[1]);
      velocyl[1]=_part[i].Getvelo()[1]*cos(cyl[1])-_part[i].Getvelo()[0]*sin(cyl[1]);
      // vitesse selon r = -vitesse selon r (rebond)  --> Nouvelle vitesse
      velocyl[0]=-velocyl[0];
      // passage en cartésien de la nouvelle vitesse
      newvelo[0]=velocyl[0]*cos(cyl[1])-velocyl[1]*sin(cyl[1]);
      newvelo[1]=velocyl[1]*cos(cyl[1])+velocyl[0]*sin(cyl[1]);
      newvelo[2]=_part[i].Getvelo()[2];
      // Calcul de la nouvelle coor
      double dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
      _part[i].Modifyvelo(newvelo);
      coor=coorImpact;
      coorImpact[0]-=newvelo[0]*dt_rest;
      coorImpact[1]-=newvelo[1]*dt_rest;
      _part[i].Modifycoor(coorImpact);
      new_coor=coorImpact;
      bool in_domain;
      in_domain = Find_Maille(i);
      if(not(in_domain))
      {
        _TF[i]=false;
      }
    }
    else
    {
      _TF[i]=false;
    }
  }



  else if (_maille[_part[i].Getref()].Getref()==14)
  {
    vector<double> vect_dir,vect_lim,coor_coin;
    vect_dir.resize(2);   vect_lim.resize(2); coor_coin.resize(2);
    vector<int> points;
    points.resize(4);
    points = _mquad[ref_maille].Getquadp();
    for(int j=0;j<4;j++)
    {
      if(_mpoint[points[j]].Getref()==10)
      {
        coor_coin=_mpoint[points[j]].Getcoor();
      }
    }
    vect_lim[0]=(coor_coin[0]-coor[0]);
    vect_lim[1]=(coor_coin[1]-coor[1]);
    double norm;
    norm=Norme(vect_lim);
    vect_lim[0]/=norm;
    vect_lim[1]/=norm;
    vect_dir[0]=(new_coor[0]-coor[0]);
    vect_dir[1]=(new_coor[1]-coor[1]);
    norm=Norme(vect_dir);
    vect_dir[0]/=norm;
    vect_dir[1]/=norm;
    if(vect_dir[1]<vect_lim[1]) // vers le bas 1
    {
      for(int j=0;j<4;j++)
      {
        if(_medge[ref_edges[j]].Getref() == 1)
        impact_edge = ref_edges[j];
      }
      // Coordonnées des sommets de l'arete par laquelle sort la particule
      sommets = _medge[impact_edge].Getedge();
      coorS1 = _mpoint[sommets[0]].Getcoor();
      coorS2 = _mpoint[sommets[1]].Getcoor();

      //Coefficient de la droite passant par l'arete
      if(coorS1[0]!=coorS2[0])
      {
        a2 = (coorS1[1]-coorS2[1])/(coorS1[0]-coorS2[0]);
        c2 = coorS1[1]-coorS1[0]*a2;

        // Coordonnées de l'impact
        coorImpact[0] = (c2-c1)/(a1-a2);
        coorImpact[1] = a1*coorImpact[0]+c1;
      }
      else
      {
        coorImpact[0]=coorS1[0];
        coorImpact[1]=a1*coorImpact[0]+c1;
      }

      // Calcul de distances
      distance_a_parcourir=Norme_entre(new_coor,coor);
      distance_parcourue=Norme_entre(coor,coorImpact);

      double dt_rest;
      vector<double> newvelo;
      newvelo.resize(3);
      // Nouvelle vitesse Vx=-Vx
      newvelo[0]=_part[i].Getvelo()[0];
      newvelo[1]=-_part[i].Getvelo()[1];
      newvelo[2]=_part[i].Getvelo()[2];
      // Calcul des nouvelles coor
      dt_rest=_dt-distance_parcourue/sqrt(pow(_part[i].Getvelo()[0],2)+pow(_part[i].Getvelo()[1],2)); // dt_rest correspond au temps restant après impact (compris entre 0 et _dt)
      _part[i].Modifyvelo(newvelo);
      coor=coorImpact;
      coorImpact[0]-=newvelo[0]*dt_rest;
      coorImpact[1]-=newvelo[1]*dt_rest;
      _part[i].Modifycoor(coorImpact);
      new_coor=coorImpact;
      bool in_domain;
      in_domain = Find_Maille(i);
      if(not(in_domain))
      {
        _TF[i]=false;
      }
    }
    else
    {
      _TF[i]=false;
    }
  }
  vector<double> NewVelo;
  NewVelo.resize(3);
  if(_methode == "Maxwellien")
  {
    NewVelo[0]=_part[i].Getvelo()[0] + sqrt(_maille[ref_maille].Gettemp()*287)*alea(0,1);
    NewVelo[1]=_part[i].Getvelo()[1] + sqrt(_maille[ref_maille].Gettemp()*287)*alea(0,1);
    NewVelo[2]=_part[i].Getvelo()[2];
  }
  _part[i].Modifyvelo(NewVelo);
}

double Mesh::Norme_entre(vector<double> Vec1, vector<double> Vec2)
{
  return sqrt(pow((Vec1[0]-Vec2[0]),2)+pow((Vec1[1]-Vec2[1]),2));
}

double Mesh::Norme(vector<double> Vec)
{
  return sqrt(pow(Vec[0],2)+pow(Vec[1],2));
}

void Mesh::Create_tau()
{
  double visc;
  visc = 8.8848*pow(10,-15)*pow(_T,3)-3.2398*pow(10,-11)*_T*_T+6.2657*pow(10,-8)*_T+2.3543*pow(10,-6);
  cout << "visc : " << visc << endl;
  _tau=visc/(_rho*287*_T);
}
