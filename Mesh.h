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
    double _omega;

  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    void readmesh();
    // ------------------ Conversion cartésien <-> cylindrique -----------------
    double Convert(double x, double y);
    double Convertinv(double r, double theta);
    // ------------------ calcul de l'angle d'incidence sur la paroi ----------------
    double Angle(int i);
};
