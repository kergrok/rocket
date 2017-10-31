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

Quad::Quad(Eigen::Vector4i v, int ref) : _v(v), _ref(ref)
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

Mesh::Buildsurfaces()
{
  _surfaces.resize(_mquad.size());
  // quad_edge comprend les quatre arêtes du quadrilatère
  Eigen::Vector4i quad_edge;

/*
  // quag_point comprends les quatres sommets du quad
  Eigen::Vector4i quad_point;
*/

  for (int i = 0; i < _mquad.size(); i++) {
    // On récupère les arêtes
    quad_edge = _mquad[i].Getquadv();
  }
}
