#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;

Point::Point(Eigen::Vector2d coor, int ref) : _coor(coor), _ref(ref)
{}

Edge::Edge(Eigen::Vector2i v, int ref) : _v(v), _ref(ref)
{}

Part::Part(Eigen::Vector2d coor,Eigen::Vector3d velo, int ref) : _coor(coor), _velo(velo), _ref(ref)
{}

Quad::Quad(Eigen::Vector4i p, Eigen::Vector4i v, int ref) : _p(p), _v(v), _ref(ref)
{}

Maille::Maille(vector<int> indices,Eigen::Vector3d prop,Eigen::Vector3d average,double surf,Eigen::Vector4i voisins)
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
  Eigen::Vector2i edge;
  // coor1 et coor2 sont les coordonnées des deux points de l'arête
  Eigen::Vector2d coor1, coor2;
  for (int i = 0; i < _medge.size(); i++) {
    // On récupère les numéros des points des extrémités de l'arête
    edge = _medge[i].Getedge();
    // On récupère les coordonnees de ces points
    coor1 = _mpoint[edge[0]].Getcoor();
    coor2 = _mpoint[edge[1]].Getcoor();
    _lenghts[i] = sqrt(pow((coor1[0]-coor2[0]),2)+pow((coor1[1]-coor2[1]),2));
  }
}

// --------- Construction de _surfaces comprenant la surface de chaque maille ----------
// - /!\ ATTENTION : il faut avoir appelé Mesh::Buildlenght() avant Mesh::Buildsurfaces() /!\ -
// Attention à vérifier aussi le résultat comme on a fait une approximation
void Mesh::Buildsurfaces()
{
  _surfaces.resize(_mquad.size());
  // quad_edge comprend les quatre arêtes du quadrilatère
  Eigen::Vector4i quad_edge;

  Eigen::Vector2i edge;
  Eigen::Vector2d coor1, coor2;
  Eigen::Vector2d coor_cyl1, coor_cyl2;
  _surf_tot = 0.;

  for (int i = 0; i < _mquad.size(); i++) {
    // On récupère les arêtes
    quad_edge = _mquad[i].Getquadv();

    // Pour l'aire on suppose que les quadrilatère sont des rectangles
    // (supposition que la hauteur du trapèze est négligeable)
    for (int j = 0; j < 4; j++) {
      // On récupère les numéros des points des extrémités de l'arête
      edge = _medge[quad_edge[j]].Getedge();
      // On récupère les coordonnees de ces points
      coor1 = _mpoint[edge[0]].Getcoor();
      coor2 = _mpoint[edge[1]].Getcoor();
      // On passe les coordonnées en cylindrique
      coor_cyl1 = Convert(coor1[0], coor1[1]);
      coor_cyl2 = Convert(coor2[0], coor2[1]);
      // On regarde si l'arête est en haut/bas du quadrilatère
      if (abs(coor_cyl1[0]-coor_cyl2[0])<coor_cyl1[0]*0.00001) {
        // On fait une boucle sur les trois autres arêtes
        for (int k = 0; k < 4; k++) {
          if (k!=j) {
            // On récupère les numéros des points des extrémités de l'arête
            edge = _medge[quad_edge[k]].Getedge();
            // On récupère les coordonnees de ces points
            coor1 = _mpoint[edge[0]].Getcoor();
            coor2 = _mpoint[edge[1]].Getcoor();
            // On passe les coordonnées en cylindrique
            coor_cyl1 = Convert(coor1[0], coor1[1]);
            coor_cyl2 = Convert(coor2[0], coor2[1]);
            // On regarde si l'arête est sur le côté du quadrilatère
            // Si c'est le cas on calcule la surface à partir de l'arête j et de l'arête k
            if (abs(coor_cyl1[1]-coor_cyl2[1])<coor_cyl1[1]*0.00001)
              _surfaces(i) = _lenghts[j]*_lenghts[k];
          }
        }
      }
    }
    //On stocke en double la surface donc il faudrait voir pour supprimer _surfaces
    _maille[i].Modifysurf(_surfaces(i));
    _surf_tot +=_surfaces(i);
  }
}

// On cherche les voisins de chaque maille et on les ajoute à _maille
// On a tous participé à cette fonction sauf Camille qui s'est défilé
void Mesh::Buildvoisins()
{
  //edge1 comprend les arêtes du quad dont on cherche les voisins
  //edge2 comprend les arêtes des quad qu'on teste pour être les voisins
  Eigen::Vector4i edge1, edge2;
  // Un vecteur colors qui comprend le nbre de voisins déjà trouvé pour chaque maille
  Eigen::VectorXi colors;
  colors.setZero(_mquad.size());

  //Booléen qui permet de stopper les tests sur j si on a trouvé que c'était un voisin
  bool vois(false);

  // i : quad dont on cherche les voisins
  for (int i = 0; i < _mquad.size(); i++) {
    //Si on a pas déjà tout les voisins de i :
    if (colors[i]!=4) {
      edge1 = _mquad[i].Getquadv();
      // j : quad qu'on teste sur son voisinage avec i
      // On teste seulement les quad dont on a pas encore determiné les voisins
      // c'est pour ça qu'on part de i+1
      for (int j = i+1; j < _mquad.size(); j++) {
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
  for (int i = 0; i < _mquad.size(); i++)
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
