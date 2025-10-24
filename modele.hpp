#include <vector>
#include <string>

using Plateau = std::vector<std::vector<int>>;

/**
 * Structure permettant de représenter le plateau et le score.
 **/
struct StatsPlateau
{
    /** Initialise le plateau et le score.
     **/
    void init(); // PAS DE TEST DIRECT (c.f. définition).
    bool operator==(const StatsPlateau& autreStatsPlateau) const; // PAS DE TEST DIRECT (c.f. définition).

    Plateau plateau;
    std::size_t score { 0 };
};

// Vous pouvez ajouter des fonctions à ce fichier si besoin est: OK

/** Génère aléatoirement un 2 ou un 4 avec des probabilités respectives de 9/10 et 1/10.
 * @return un entier 2 ou 4
 **/
int tireDeuxOuQuatre();

/** Génère un plateau de dimensions 4*4 ne contenant que des 0.
 * @return un Plateau vide
 **/
Plateau plateauVide();

/** Génère deux nombres sur des cases aléatoires d'un plateau vide.
 * @return un Plateau en début de jeu
 **/
Plateau plateauInitial();

/** Vérifie si le plateau est rempli.
 * @param plateau une référence constante de Plateau
 * @return vrai si le plateau est rempli, faux sinon
 **/
bool plateauOccupé(const Plateau& plateau);

/** Ajoute un 2 ou un 4 au plateau. Il est nécessaire que celui-ci ne soit pas rempli totalement.
 * @param plateau une référence de Plateau
 * @return le plateau initial avec un élément supplémentaire
 **/
void ajouteElementPlateau(Plateau& plateau);

/** Déplace les tuiles d'un plateau vers la gauche et les combine si possible.
 * @param statsPlateau une référence constante de StatsPlateau
 * @return le statsPlateau une fois déplacé vers la gauche
 **/
StatsPlateau déplacementGauche(const StatsPlateau& statsPlateau);

/** Déplace les tuiles d'un plateau vers la droite et les combine si possible.
 * @param statsPlateau une référence constante de StatsPlateau
 * @return le statsPlateau une fois déplacé vers la droite
 **/
StatsPlateau déplacementDroite(const StatsPlateau& statsPlateau);

/** Déplace les tuiles d'un plateau vers le haut et les combine si possible.
 * @param statsPlateau une référence constante de StatsPlateau
 * @return le statsPlateau une fois déplacé vers le haut
 **/
StatsPlateau déplacementHaut(const StatsPlateau& statsPlateau);

/** Déplace les tuiles d'un plateau vers le bas et les combine si possible.
 * @param statsPlateau une référence constante de StatsPlateau
 * @return le statsPlateau une fois déplacé vers le bas
 **/
StatsPlateau déplacementBas(const StatsPlateau& statsPlateau);

/** Déplace les tuiles d'un plateau dans la direction donnée et génère une nouvelle tuile si le déplacement est valide.
 * @param statsPlateau une référence constante de StatsPlateau
 * @param direction un entier décrivant la direction à prendre
 * @return le statsPlateau déplacé dans la direction
 **/
StatsPlateau déplacement(const StatsPlateau& statsPlateau, int direction);

/** Affiche un plateau.
 * @param plateau une référence constante de Plateau
 * @return une chaîne de caractère représentant le plateau
 **/
std::string dessine(const Plateau& plateau);

/** Permet de savoir si une partie est terminée.
 * @param plateau une référence constante de Plateau
 * @return vrai si le plateau est vide, faux sinon
 **/
bool estTerminé(const Plateau& plateau);

/** Permet de savoir si une partie est gagnée.
 * @param plateau une référence constante de Plateau
 * @return vrai si le plateau contient un 2048, faux sinon
 **/
bool estGagnant(const Plateau& plateau);