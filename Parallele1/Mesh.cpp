#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;

Point::Point(std::vector<double> coor, int ref) : _coor(coor), _ref(ref)
{}

Edge::Edge(std::vector<int> v, int ref) : _v(v), _ref(ref)
{}

Part::Part(std::vector<double> coor,std::vector<double> velo, int ref) : _coor(coor), _velo(velo), _ref(ref)
{}

Quad::Quad(std::vector<int> p, std::vector<int> v, int ref) : _p(p), _v(v), _ref(ref)
{
  _normale.resize(4);
}

Maille::Maille(vector<int> indices,std::vector<double> prop,std::vector<double> average,double surf,std::vector<int> voisins)
{
  _indices=indices;
  _prop=prop;
  _average=average;
  _surf=surf;
  _voisins=voisins;
}

Mesh::Mesh(string meshname): _meshname(meshname)
{}

// --------- Construction de _lenghts comprenant la taille de chaque arête ----------
void Mesh::Buildlenghts()
{
  _lenghts.resize(_medge.size());
  // edge comprend les réferences des deux points de l'arête
  std::vector<int> edge;
  // coor1 et coor2 sont les coordonnées des deux points de l'arête
  std::vector<double> coor1, coor2;
  for (size_t i = 0; i < _medge.size(); i++) {
    // On récupère les numéros des points des extrémités de l'arête
    edge = _medge[i].Getedge();
    // On récupère les coordonnees de ces points
    coor1 = _mpoint[edge[0]].Getcoor();
    coor2 = _mpoint[edge[1]].Getcoor();
    _lenghts[i] = sqrt(pow((coor1[0]-coor2[0]),2)+pow((coor1[1]-coor2[1]),2));
  }
}

// --------- Construction des surfaces dans Maille ----------
// - /!\ ATTENTION : il faut avoir appelé Mesh::Buildlenght() avant Mesh::Buildsurfaces() /!\ -
// Attention à vérifier aussi le résultat comme on a fait une approximation
void Mesh::Buildsurfaces()
{
  double surface(0);
  // quad_edge comprend les quatre arêtes du quadrilatère
  std::vector<int> quad_edge;

  std::vector<int> edge;
  std::vector<double> coor1, coor2;
  std::vector<double> coor_cyl1, coor_cyl2;
  _surf_tot = 0.;
  double taille_min(0), taille_max(0);

  for (size_t i = 0; i < _mquad.size(); i++) {
    // On récupère les arêtes
    quad_edge = _mquad[i].Getquadv();
    taille_min = _lenghts[quad_edge[0]];
    taille_max = _lenghts[quad_edge[0]];

    // Pour l'aire on suppose que les quadrilatère sont des rectangles
    // (supposition que la hauteur du trapèze est négligeable)
    for (int j = 0; j < 4; j++) {
      if (taille_min > _lenghts[quad_edge[j]]) taille_min = _lenghts[quad_edge[j]];
      if (taille_max < _lenghts[quad_edge[j]]) taille_max = _lenghts[quad_edge[j]];
    }
    surface = taille_min*taille_max;
    _maille[i].Modifysurf(surface);
    _surf_tot +=surface;
  }
}

// On cherche les voisins de chaque maille et on les ajoute à _maille
// On a tous participé à cette fonction sauf Camille qui s'est défilé
void Mesh::Buildvoisins()
{
  //edge1 comprend les arêtes du quad dont on cherche les voisins
  //edge2 comprend les arêtes des quad qu'on teste pour être les voisins
  std::vector<int> edge1, edge2;
  // Un vecteur colors qui comprend le nbre de voisins déjà trouvé pour chaque maille
  std::vector<int> colors;
  colors.resize(_mquad.size());
  for(int i=0;i<colors.size();i++)
  {
    colors[i]=0;
  }

  //Booléen qui permet de stopper les tests sur j si on a trouvé que c'était un voisin
  bool vois(false);

  // i : quad dont on cherche les voisins
  for (size_t i = 0; i < _mquad.size(); i++) {
    //Si on a pas déjà tout les voisins de i :
    if (colors[i]!=4) {
      edge1 = _mquad[i].Getquadv();
      // j : quad qu'on teste sur son voisinage avec i
      // On teste seulement les quad dont on a pas encore determiné les voisins
      // c'est pour ça qu'on part de i+1
      for (size_t j = i+1; j < _mquad.size(); j++) {
        //Si on a pas déjà tout les voisins de j :
        if (colors[j]!=4) {
          if (i != j) {
            edge2 = _mquad[j].Getquadv();
            vois = false;
            // k : on va faire les tests sur les 4 arêtes de i
            for (int k = 0; k<4; k++) {
              // Si vois est "true" ça veut dire que j est un voisin, ce n'est pas la peine de continuer la boucle k
              if (vois == false)
              if ((edge1[k]==edge2[0])||(edge1[k]==edge2[1])||(edge1[k]==edge2[2])||(edge1[k]==edge2[3])) {
                // On ajoute à la maille i le voisin j
                // cout<< " on a : "<<_maille[i].Getvoisins().size()<<" Voisins et j,colors= "<<j<<" "<<colors[i]<<endl;
                _maille[i].Modifyvoisins(j, colors[i]);
                // Et vice et versa
                _maille[j].Modifyvoisins(i, colors[j]);
                // On ajoute un voisin trouvé pour i et un pour j
                colors[i]++;
                colors[j]++;
                // On fait en sorte de sortir de la boucle k
                vois = true;
              }
            }
          }
        }
      }
    }
  }
  // Si la maille a juste 2 ou 3 voisins, on complete le vecteur de -1
  for (size_t i = 0; i < _mquad.size(); i++)
  {
      if (colors[i]==3)
      {
        _maille[i].Modifyvoisins(-1,3);
      }
      else if (colors[i]==2)
      {
        _maille[i].Modifyvoisins(-1,2);
        _maille[i].Modifyvoisins(-1,3);
      }
  }
}

// Calcul de la norme, du milieu et de la normale de chaque arêtes
void Mesh::Build_Center_Norm()
{
  for (size_t i = 0; i< _maille.size(); i++)
  {
    vector<double> Normale(2);
    vector<double> Middle_Edge(2);
    vector<double> MiEdge_Middle(2);
    vector<int> Aretes(4);

    Aretes=_mquad[i].Getquadv();

    //On récupère les numéros des sommets du quad testé
    vector<int> Sommets(4);
    Sommets=_mquad[i].Getquadp();

    // Coordonées du milieu du quad
    vector<double> Middle(2);
    Middle={0};

    for(int j=0; j<4;j++)
    {
      Middle[0] += _mpoint[Sommets[j]].Getcoor()[0]/4;
      Middle[1] += _mpoint[Sommets[j]].Getcoor()[1]/4;
    }
    _mquad[i].Modify_center(Middle);

    std::vector<std::vector<double>> norm,mid,mid_ed;
    norm.resize(0);
    mid.resize(0);
    mid_ed.resize(0);
    for(int j=0;j<4;j++)
    {

      Sommets[0]=_medge[Aretes[j]].Getedge()[0];
      Sommets[1]=_medge[Aretes[j]].Getedge()[1];

      Normale[0]=_mpoint[Sommets[0]].Getcoor()[1]-_mpoint[Sommets[1]].Getcoor()[1];
      Normale[1]=-_mpoint[Sommets[0]].Getcoor()[0]+_mpoint[Sommets[1]].Getcoor()[0];

      Middle_Edge[0]=(_mpoint[Sommets[0]].Getcoor()[0]+_mpoint[Sommets[1]].Getcoor()[0])/2;
      Middle_Edge[1]=(_mpoint[Sommets[0]].Getcoor()[1]+_mpoint[Sommets[1]].Getcoor()[1])/2;
      mid_ed.push_back(Middle_Edge);

      MiEdge_Middle[0]=Middle[0]-Middle_Edge[0];
      MiEdge_Middle[1]=Middle[1]-Middle_Edge[1];
      mid.push_back(MiEdge_Middle);
      if(Normale[0]*MiEdge_Middle[0]+Normale[1]*MiEdge_Middle[1]>0)
      {
        Normale[0]=-Normale[0];
        Normale[1]=-Normale[1];
      }
      norm.push_back(Normale);
    }
    _mquad[i].Modify_normale(norm);
    _mquad[i].Modify_mid_norm(mid);
    _mquad[i].Modify_mid_ed(mid_ed);
  }
}
