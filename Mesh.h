#include <vector>
#include <string>
#include "Type.h"

class Mesh
{
  private:
    std::vector<Point> _mpoint;
    std::vector<Quad> _mquad;
    std::vector<Part> _part;
    std::vector<Maille> _maille;
    std::vector<Edge> _medge;  
    std::string _meshname;

  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    void readmesh();
};
