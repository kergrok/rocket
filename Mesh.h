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

    // Surface des quadrilatères
    Eigen::VectorXd _surfaces;
    // Longueur des arêtes
    Eigen::VectorXd _lenghts;

  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    void readmesh();
    // ------------------ Conversion cartésien <-> cylindrique -----------------
    Eigen::Vector2d Convert(double x, double y);
    Eigen::Vector2d Convertinv(double r, double theta);
    // ------------------ calcul de l'angle d'incidence sur la paroi ----------------
    double Angle(int i);
    // ------------------ Calcul de la longueur des arêtes -----------------
    void Buildlenghts();
    // ------------------ Calcul des surfaces des quad ---------------------
    void Buildsurfaces();
    // ----------- Calcul des propriétés dans la maille i --------------
    void Calc_prop(int i);

    void Run(double t_final, );

    double CFL();
};
