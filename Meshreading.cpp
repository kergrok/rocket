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

  double purcent = 0;  // Sert à quoi ?
  // Boucle sur les quadrilatères
  for (int i = 0 ; i < _mquad.size() ; ++i)
  {
    if (i%500 == 0) // 500 à modifier suivant le nombre d'edges de notre maillage
    {
      cout << 100*i/_mquad.size() << "% of edges are built ..." << endl;
    }
     vector<int> p = { _mquad[i].Getquadp()[0], _mquad[i].Getquadp()[1], _mquad[i].Getquadp()[2], _mquad[i].Getquadp()[3]};
    // Boucle sur les arêtes
    for (int k = 0 ; k < 4 ; ++k)
    {
      Eigen::VectorXi p1_p2(2); p1_p2 << p[k] , p[(k+1)%4];
      Edge edge(p1_p2,0);

      // Est ce que l'arête est déjà dans le vecteur _medge ?
      bool edge_found = false;
      for (int l = 0 ; l < _medge.size() ; ++l)
      {
        if (((_medge[l].Getedge()[0] == edge.Getedge()[0]) && (_medge[l].Getedge()[1] == edge.Getedge()[1])) ||
        ((_medge[l].Getedge()[0] == edge.Getedge()[1]) && (_medge[l].Getedge()[1] == edge.Getedge()[0])) )
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

  for (int i = 0 ; i < _medge.size() ; ++i)
  {
    Eigen::Vector2i points;
    int r1,r2 ;
    points = _medge[i].Getedge();
    r1 = _mpoint[points[0]].Getref();
    r2 = _mpoint[points[1]].Getref();
    if (r1 == 6) r1 = 0;
    if (r2 == 6) r2 = 0;
    if ((r1 == 0) || (r2 == 0))
      _medge[i].Modifyref(0);
    else if (r1 == r2)
      _medge[i].Modifyref(r1);
    else _medge[i].Modifyref(min(r1,r2));
  }

  if ((nb_edges != _medge.size()) || (nb_edges != _edg_Q1.size()) || (nb_edges != _edg_Q2.size()))
    cout << "Problem with the edges building !" << endl;

    //WriteEdgesAndAssociatedQuad();
}

void Mesh::WriteEdgesAndAssociatedQuad()
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
  Vector2d vert; Vector4i quad; Vector2i edg;
  // Contient les quatres arêtes du quadrilatère
  Vector4i edges(0,0,0,0);
  int ref(0), loop_pts(1);
  int np;
  int ned;
  int nqua;
  double z;

  while (!mesh_file.eof())
  {
    getline(mesh_file, file_line);
    if ((file_line.find("Vertices") != string::npos)&&(loop_pts))
    {
      mesh_file >> np;
      cout << "Nombre de points  (" << np << ")" << endl;
      for (int i = 0 ; i < np ; ++i)
      {
        mesh_file >> vert[0] >> vert[1] >> z >> ref;
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
        //A condition que dans le .geo on ait la ref de la surface valant 6 :
        if (ref == 6) ref = 0; //Tout les points à 0 sont des points milieux
        _medge.push_back(Edge(edg, ref));
      }
    }
    vector<int> indices;
    Vector3d prop,average;
    Vector4i Voisins;
    double surf;
    if (file_line.find("Quadrilaterals") != string::npos)
    {
      mesh_file >> nqua;
      cout << "Nombre de quad (" << nqua << ")" << endl;
      for (int i = 0 ; i < nqua ; ++i)
      {
        mesh_file >> quad[0] >> quad[1] >> quad[2] >> quad[3] >> ref;
        quad[0]--; quad[1]--; quad[2]--; quad[3]--;
        _mquad.push_back(Quad(quad, edges, ref));
        _maille.push_back(Maille(indices,prop,average,surf,Voisins));
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

//Est aussi dans la fonction buildedge, il faudrait l'écrire une seule fois pour les deux cas
// (a voir où bien la placer)
    for (int i = 0 ; i < _medge.size() ; ++i)
    {
      Eigen::Vector2i points;
      int r1,r2 ;
      points = _medge[i].Getedge();
      r1 = _mpoint[points[0]].Getref();
      r2 = _mpoint[points[1]].Getref();
      if (r1 == 6) r1 = 0;
      if (r2 == 6) r2 = 0;
      if ((r1 == 0) || (r2 == 0))
        _medge[i].Modifyref(0);
      else if (r1 == r2) {
        _medge[i].Modifyref(r1);}
      else _medge[i].Modifyref(min(r1,r2));
    }
  }
  // Ajout des arêtes dans la classe Quad
  VectorXi nb_voisins;
  nb_voisins.setZero(_mquad.size());
  for (int i = 0 ; i < _medge.size() ; i++) {
    _mquad[_edg_Q1[i]].Modifyv(i, nb_voisins[_edg_Q1[i]]);
    nb_voisins[_edg_Q1[i]]++;
    if (_edg_Q2[i]!=-1){
      _mquad[_edg_Q2[i]].Modifyv(i, nb_voisins[_edg_Q2[i]]);
      nb_voisins[_edg_Q2[i]]++;
    }
  }

  for (int i = 0 ; i < _mquad.size() ; i++)
  {
    bool if_found = false;
    Vector4i edges = _mquad[i].Getquadv();
    int ref_maille = 0;
    int nb_mailles_bord = 0;
    for (int j = 0 ; j< 4 ; j++)
    {
      if (_medge[edges[j]].Getref() == 6) _medge[edges[j]].Modifyref(0);
      ref_maille += _medge[edges[j]].Getref();
      if (_medge[edges[j]].Getref() != 0) nb_mailles_bord++;
    }
    if (ref_maille == 0 || ref_maille == 1 || ref_maille == 2 || ref_maille == 4) {
      _maille[i].Modifyref(ref_maille);
    }
    else
    {
      if (nb_mailles_bord == 1) _maille[i].Modifyref(3);
      else if ((ref_maille == 3) && (nb_mailles_bord == 2)) _maille[i].Modifyref(12);
      else if ((ref_maille == 7) && (nb_mailles_bord == 2)) _maille[i].Modifyref(34);
      else
      {
        for (int j = 0; j < 4; j++) {
          if (_medge[edges[j]].Getref() == 4)
          {
            _maille[i].Modifyref(14);
            if_found = true;
          }
        }
        if (if_found == false)
        {
          _maille[i].Modifyref(23);
        }
      }
    }
  }

  // ------------------- Calcul de la taille des arêtes -----------------------
  Buildlenghts();
  // ------------------- Calcul des surfaces des quad -----------------------
  Buildsurfaces();
}


void Mesh::ReadParameter()
{
  ifstream mesh_file("parametres.txt");
  string file_line;
  int loop_pts(1);
  if (!mesh_file.is_open())
  {
    cout << "Impossible d'ouvrir le fichier " << _meshname << endl;
    abort();
  }

  while (!mesh_file.eof())
  {
    getline(mesh_file, file_line);
    if ((file_line.find("Température") != string::npos)&&(loop_pts))
    {
      mesh_file >> _T;
    }
    if ((file_line.find("Densité") != string::npos)&&(loop_pts))
    {
      mesh_file >> _rho;
    }
    if ((file_line.find("N") != string::npos)&&(loop_pts))
    {
      mesh_file >> _N;
    }
    if ((file_line.find("Mach") != string::npos)&&(loop_pts))
    {
      mesh_file >> _Ma;
    }
    if ((file_line.find("Gamma") != string::npos)&&(loop_pts))
    {
      mesh_file >> _gamma;
    }
    if ((file_line.find("Temps Final") != string::npos)&&(loop_pts))
    {
      mesh_file >> _Temps_final;
    }
    if ((file_line.find("Methode") != string::npos)&&(loop_pts))
    {
      mesh_file >> _methode;
    }

  }
}
