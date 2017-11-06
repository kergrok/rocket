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

Quad::Quad(Eigen::Vector4i p, Eigen::Vector4i v, int ref) : _p(p), _v(v), _ref(ref)
{}

Mesh::Mesh(string meshname): _meshname(meshname)
{}

// --------- Construction de _lenghts comprenant la taille de chaque arête ----------
Mesh::Buildlenght()
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
    coor1 = edge[0].Getcoor();
    coor2 = edge[1].Getcoor();
    _lenghts[i] = sqrt(pow((coor1[0]-coor2[0]),2)+pow((coor1[1]-coor2[1]),2)));
  }
}

// --------- Construction de _surfaces comprenant la surface de chaque maille ----------
// - /!\ ATTENTION : il faut avoir appelé Mesh::Buildlenght() avant Mesh::Buildsurfaces() /!\ -
Mesh::Buildsurfaces()
{
  _surfaces.resize(_mquad.size());
  // quad_edge comprend les quatre arêtes du quadrilatère
  Eigen::Vector4i quad_edge;

  Eigen::Vector2i edge;
  Eigen::Vector2d coor1, coor2;
  Eigen::Vector2d coor_cyl1, coor_cyl2;

  for (int i = 0; i < _mquad.size(); i++) {
    // On récupère les arêtes
    quad_edge = _mquad[i].Getquadv();

    // Pour l'aire on suppose que les quadrilatère sont des rectangles
    // (supposition que la hauteur du trapèze est négligeable)
    for (int j = 0; j < 4; j++) {
      // On récupère les numéros des points des extrémités de l'arête
      edge = quad_edge[j].Getedge();
      // On récupère les coordonnees de ces points
      coor1 = edge[0].Getcoor();
      coor2 = edge[1].Getcoor();
      // On passe les coordonnées en cylindrique
      coor_cyl1 = Mesh::Convert(coor1[0], coor1[1]);
      coor_cyl2 = Mesh::Convert(coor2[0], coor2[1]);
      // On regarde si l'arête est en haut/bas du quadrilatère
      if (abs(coor_cyl1[0]-coor_cyl2[0])<coor_cyl1[0]*0.00001) {
        // On fait une boucle sur les trois autres arêtes
        for (int k = 0; k < 4; k++) {
          if (k!=j) {
            // On récupère les numéros des points des extrémités de l'arête
            edge = quad_edge[k].Getedge();
            // On récupère les coordonnees de ces points
            coor1 = edge[0].Getcoor();
            coor2 = edge[1].Getcoor();
            // On passe les coordonnées en cylindrique
            coor_cyl1 = Mesh::Convert(coor1[0], coor1[1]);
            coor_cyl2 = Mesh::Convert(coor2[0], coor2[1]);
            // On regarde si l'arête est sur le côté du quadrilatère
            // Si c'est le cas on calcule la surface à partir de l'arête j et de l'arête k
            if (abs(coor_cyl1[1]-coor_cyl2[1])<coor_cyl1[1]*0.00001)
              _surfaces(i) = _lenghts[j]*_lenghts[k];
          }
        }
      }
    }
  }
}
