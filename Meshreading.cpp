#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace Eigen;

void Mesh::BuildEdges()
{
  // Toutes les arêtes exterieures du maillage sont présentes
  int nb_edges = (4*_mquad.size() + _medge.size())/2;
  _medge.clear();

  double purcent = 0;
  // Boucle sur les quadrilatères
  for (int i = 0 ; i < _mquad.size() ; ++i)
  {
    if (i%500 == 0)
    {
      cout << 100*i/_mquad.size() << "% of edges are built ..." << endl;
    }
     vector<int> p = { _mquad[i].Getquadp()[0], _mquad[i].Getquadp()[1], _mquad[i].Getquadp()[2], _mquad[i].Getquadp()[3]};
    // Boucle sur les arêtes
    for (int k = 0 ; k < 4 ; ++k)
    {
      Eigen::VectorXi p1_p2(2); p1_p2 << p[k] , p[(k+1)%4];
      Edge edge(p1_p2,0);

      // Est ce que l'arête est déjà dans le vecteur _edges ?
      bool edge_found = false;
      for (int l = 0 ; l < _medge.size() ; ++l)
      {
        if (_medge[l].Getedge() == edge.Getedge())
        {
          edge_found = true;
          _edg_Q2[l] = i;
          break;
        }
      }
      if (!edge_found)
      {
        _medge.push_back(edge);
        _edg_Q1.conservativeResize(_edg_Q1.size() + 1, Eigen::NoChange);
        _edg_Q1[_edg_Q1.size()-1] = i;
        _edg_Q2.conservativeResize(_edg_Q2.size() + 1, Eigen::NoChange);
        _edg_Q2[_edg_Q2.size()-1] = -1;
      }
    }
  }

  if ((nb_edges != _medge.size()) || (nb_edges != _edg_Q1.size()) || (nb_edges != _edg_Q2.size()))
    cout << "Problem with the edges building !" << endl;

    WriteEdgesAndAssociatedTriangles();
}

void Mesh::WriteEdgesAndAssociatedTriangles()
{
  string edges_file_name = _meshname.substr(0,_meshname.size()-5)+"_edges.txt";
  std::ofstream out_file (edges_file_name);
  out_file << "Edges" << endl;
  for (int i = 0 ; i < _medge.size() ; ++i)
  {
    out_file << _medge[i].Getedge()[0] << " " << _medge[i].Getedge()[1] << " ";
  }
  out_file << endl;
  out_file << "EdgesQ1" << endl;
  for (int i = 0 ; i < _medge.size() ; ++i)
  {
    out_file << _edg_Q1[i] << " ";
  }
  out_file << endl;
  out_file << "EdgesQ2" << endl;
  for (int i = 0 ; i < _medge.size() ; ++i)
  {
    out_file << _edg_Q2[i] << " ";
  }
}

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
        _mquad.push_back(Quad(quad, ref));
      }
    }
  }

  cout << "---------Edges and Associated Triangles----------" << endl;
  string edges_file_name = _meshname.substr(0,_meshname.size()-5)+"_edges.txt";
  ifstream edges_file(edges_file_name.data());
  if (!edges_file.is_open())
  {
    BuildEdges();
  }
  else
  {
    cout << "A file which contains the edges has been found !"<< endl;
    string file_line;
    int loop_edges(1);
    int nb_edges = (4*_mquad.size() + _medge.size())/2;
    _medge.clear();
    _edg_Q1.resize(nb_edges);
    _edg_Q2.resize(nb_edges);
    int p1, p2, t1, t2;

    while (!edges_file.eof())
    {
      getline(edges_file, file_line);
      if ((file_line.find("Edges") != std::string::npos)&&(loop_edges))
      {
        for (int i = 0 ; i < nb_edges ; ++i)
        {
          edges_file >> p1 >> p2;
          Eigen::VectorXi p1_p2(2); p1_p2 << p1 , p2;
          Edge edge(p1_p2,0);
          _medge.push_back(edge);
        }
        loop_edges = 0.;
      }
      if ((file_line.find("EdgesQ1") != std::string::npos))
      {
        for (int i = 0 ; i < nb_edges ; ++i)
        {
          edges_file >> _edg_Q1[i];
        }
      }
      if ((file_line.find("EdgesQ2") != std::string::npos))
      {
        for (int i = 0 ; i < nb_edges ; ++i)
        {
          edges_file >> _edg_Q2[i];
        }
      }
    }
  }

  // ------------------- Calcul de la taille des arêtes -----------------------
  Buildlenght();
  // ------------------- Calcul des surfaces des quad -----------------------
  Buildsurfaces();
}
