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
    int _N;
    double _T;
    double _Ma;
    double _gamma;
    std::vector<bool> _TF;
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
    // ------- Déplace toutes les particules pour un pas de temps -------
    void Displacement(double vit_moy);
    // ---------------- Calcul dt grâce à la "CFL" --------------------
    void CFL();
    // ------------------ Calcul des voisins ---------------------
    void Buildvoisins();

    bool Find_Maille(int i);

    void find_impact(int i, Eigen::Vector2d coor, Eigen::Vector2d new_coor);

    bool is_in(int maille, Eigen::Vector2d Position);

    void Create_particules(int maille, int arete);

    void Create_in_Flow();
    // ------------- Ecriture des arêtes dans un fichier --------------
    void WriteEdgesAndAssociatedQuad();

    void write(std::string Nom_Fichier, Eigen::VectorXd U);
    // Lit les parametres dans un fichier
    void ReadParameter();
    // Initialisation du Problem
    void initialize();
    // boucle en tps du Problem
    void compute();
};
