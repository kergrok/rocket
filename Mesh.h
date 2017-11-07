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
    double _dt;
    // Surface des quadrilatères
    Eigen::VectorXd _surfaces;
    // Longueur des arêtes
    Eigen::VectorXd _lenghts;
    // Les quadrilatères collés aux arêtes
    // Si _edg_Q2[i] == -1 alors c'est une arête au bord
    Eigen::VectorXi _edg_Q1, _edg_Q2;

  public:
    // Constructeur
    Mesh(std::string meshname);
    // ------------------ Lecture du maillage  ---------------------
    void readmesh();
    // ------------ Construction des arêtes à l'intérieur du domaine -----------
    void BuildEdges();
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

    void Run(double t_final);

    void CFL();
    // ------------------ Calcul des voisins ---------------------
    void Buildvoisins();

    void Find_Maille(int i);

    bool is_in(int maille, Eigen::Vector2d Position);
};
