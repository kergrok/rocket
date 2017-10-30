#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;

void Mesh::readmesh()
{
  ifstream mesh_file(_meshname.data());   
  if (!mesh_file.is_open())
  {
    cout << "Impossible d'ouvrir le fichier " << _meshname << endl;
    abort();
  }
  string file_line;
  Vector4d vert; Vector4i quad; Vector2i edg;
  int ref(0), loop_pts(1);
  int np;
  int ned;
  int nqua;

  while (!mesh_file.eof())
  {
    getline(mesh_file, file_line);
    if ((file_line.find("Vertices") != string::npos)&&(loop_pts))
    {
      mesh_file >> np;
      cout << "Nombre de points  (" << np << ")" << endl;
      for (int i = 0 ; i < np ; ++i)
      {
        mesh_file >> vert[0] >> vert[1] >> vert[2] >> ref;
        // coor, ref, exist, nxt = 0, tmp = 0
        _mpoint.push_back(Point(vert, ref));
      }
      loop_pts = 0;
    }
    if (file_line.find("Edges") != string::npos)
    {
      mesh_file >> ned;
      cout << "Nombre d'arêtes (" << ned << ")" << endl;
      for (int i = 0 ; i < ned ; ++i)
      {
        mesh_file >> edg[0] >> edg[1] >> ref;
        edg[0]--; edg[1]--;
        // v, ref, exist, inmesh
        _medge.push_back(Edge(edg, ref));
      }
    }
    if (file_line.find("Quadrilaterals") != string::npos)
    {
      mesh_file >> nqua;
      cout << "Nombre de quad (" << ned << ")" << endl;
      for (int i = 0 ; i < ned ; ++i)
      {
        mesh_file >> quad[0] >> quad[1] >> quad[2] >> quad[3] >> ref;
        quad[0]--; quad[1]--; quad[2]--; quad[3]--;
        _medge.push_back(Quad(quad, ref));
    }
  }
}
