#include "modele.hpp"

#include <iostream>

#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

void StatsPlateau::init()
{
	plateau = plateauInitial();
    score = 0;
}

bool StatsPlateau::operator==(const StatsPlateau& autreStatsPlateau) const
{
    return plateau == autreStatsPlateau.plateau
        && score == autreStatsPlateau.score;
}

int tireDeuxOuQuatre()
{
 	/* Génère un nombre aléatoire a tel que 1 <= a <= 10. */
 	const int nombreAleatoire { rand() % 10 + 1 };

    if (nombreAleatoire <= 9)
        return 2;

    return 4;
}

Plateau plateauVide()
{
    return Plateau(4, std::vector<int>(4, 0));
}

Plateau plateauInitial()
{
    srand(time(0)); /* Initialisation de la graine. */

    Plateau plateauInitial { plateauVide() };
	const int taillePlateau { static_cast<int>(plateauInitial.size() * plateauInitial[0].size()) };

	/* Génère un nombre aléatoire a tel que 0 <= a <= 15 */
 	const int placementAleatoire1 { rand() % taillePlateau };
    int placementAleatoire2 { 0 };

    do
    {
		placementAleatoire2 = rand() % taillePlateau;
    }
    while(placementAleatoire1 == placementAleatoire2);

    /* Place le 2 ou le 4 a deux endroits totalement différent. */
	plateauInitial[placementAleatoire1 / 4][placementAleatoire1 % 4] = tireDeuxOuQuatre();
    plateauInitial[placementAleatoire2 / 4][placementAleatoire2 % 4] = tireDeuxOuQuatre();

    return plateauInitial;
}

bool plateauOccupé(const Plateau& plateau)
{
    for (const std::vector<int>& ligne : plateau)
    {
    	for (int nombre : ligne)
        {
          	if (nombre == 0)
            {
            	return false;
            }
        }
    }

    return true;
}

void ajouteElementPlateau(Plateau& plateau)
{
	const int taillePlateau { static_cast<int>(plateau.size() * plateau[0].size()) };
    int placementAleatoire { rand() % taillePlateau };

   	do
    {
		placementAleatoire = rand() % taillePlateau;
    }
    while(plateau[placementAleatoire / 4][placementAleatoire % 4] != 0);

    // Place le 2 ou le 4 a un nouvel endroit.
	plateau[placementAleatoire / 4][placementAleatoire % 4] = tireDeuxOuQuatre();
}

StatsPlateau déplacementGauche(const StatsPlateau& statsPlateau)
{
    StatsPlateau nouveauStatsPlateau;
    nouveauStatsPlateau.plateau = statsPlateau.plateau;

	const std::size_t l = nouveauStatsPlateau.plateau.size();    // Nombre de lignes
    const std::size_t c = nouveauStatsPlateau.plateau[0].size(); // Nombre de colonnes

    for (std::size_t i { 0 }; i != l; i++)
    {
        // Placer les éléments à gauche.
        std::vector<int> nouvelleLigne(c, 0);
        for (std::size_t j = 0; j != l; j++)
        {
            for (std::size_t k = j; k != l; k++)
            {
				if (nouveauStatsPlateau.plateau[i][k] != 0)
                {
                    nouvelleLigne[j] = nouveauStatsPlateau.plateau[i][k];
                    nouveauStatsPlateau.plateau[i][k] = 0;
                    break;
                }
            }
        }

        // Fusionner les éléments adjacents identiques.
        for (std::size_t j = 0; j != c - 1; j++)
        {
            if (nouvelleLigne[j] == nouvelleLigne[j + 1] && nouvelleLigne[j] != 0)
          	{
                nouvelleLigne[j] *= 2;    // Fusionner les cases
                nouvelleLigne[j + 1] = 0; // La case qui précède celle fusionnée devient 0.
                nouveauStatsPlateau.score += nouvelleLigne[j];
            }
        }

        // Replacer la nouvelle ligne dans le plateau
        nouveauStatsPlateau.plateau[i] = nouvelleLigne;

        // Placer les éléments à gauche.
        nouvelleLigne = std::vector<int>(c, 0);
        for (std::size_t j = 0; j != l; j++)
        {
            for (std::size_t k = j; k != l; k++)
            {
				if (nouveauStatsPlateau.plateau[i][k] != 0)
                {
                    nouvelleLigne[j] = nouveauStatsPlateau.plateau[i][k];
                    nouveauStatsPlateau.plateau[i][k] = 0;
                    break;
                }
            }
        }

        // Replacer la nouvelle ligne dans le plateau
        nouveauStatsPlateau.plateau[i] = nouvelleLigne;
    }

    return nouveauStatsPlateau;
}

StatsPlateau déplacementDroite(const StatsPlateau& statsPlateau)
{
    StatsPlateau nouveauStatsPlateau;
    nouveauStatsPlateau.plateau = statsPlateau.plateau;

	const std::size_t l { nouveauStatsPlateau.plateau.size() };    // Nombre de lignes
    const std::size_t c { nouveauStatsPlateau.plateau[0].size() }; // Nombre de colonnes

    for (std::size_t i = 0; i != l; i++)
    {
        // Placer les éléments à droite.
        std::vector<int> nouvelleLigne(c, 0);
        for (std::size_t j = l; j--;)
        {
            for (std::size_t k = j + 1; k--;)
            {
				if (nouveauStatsPlateau.plateau[i][k] != 0)
                {
                    nouvelleLigne[j] = nouveauStatsPlateau.plateau[i][k];
                    nouveauStatsPlateau.plateau[i][k] = 0;
                    break;
                }
            }
        }

        // Fusionner les éléments adjacents identiques.
        for (std::size_t j { c }; j-- && j != 0;) //
        {
            if (nouvelleLigne[j] == nouvelleLigne[j - 1] && nouvelleLigne[j] != 0)
          	{
                nouvelleLigne[j] *= 2;    // Fusionner les cases
                nouvelleLigne[j - 1] = 0; // La case qui précède celle fusionnée devient 0.
                nouveauStatsPlateau.score += nouvelleLigne[j];
            }
        }

        // Replacer la nouvelle ligne dans le plateau
        nouveauStatsPlateau.plateau[i] = nouvelleLigne;

        // Placer les éléments à droite.
        nouvelleLigne = std::vector<int>(c, 0);
        for (std::size_t j = l; j--;)
        {
            for (std::size_t k = j + 1; k--;)
            {
				if (nouveauStatsPlateau.plateau[i][k] != 0)
                {
                    nouvelleLigne[j] = nouveauStatsPlateau.plateau[i][k];
                    nouveauStatsPlateau.plateau[i][k] = 0;
                    break;
                }
            }
        }

        nouveauStatsPlateau.plateau[i] = nouvelleLigne;
    }

    return nouveauStatsPlateau;
}

StatsPlateau déplacementHaut(const StatsPlateau& statsPlateau)
{
    StatsPlateau nouveauStatsPlateau;
    nouveauStatsPlateau.plateau = statsPlateau.plateau;

    const std::size_t l { nouveauStatsPlateau.plateau.size() };    // Nombre de lignes

    for (std::size_t i { 0 }; i != l; i++)
    {
        // Placer les éléments en haut.
        std::vector<int> nouvelleColonne(l, 0);
        for (std::size_t j = 0; j != l; j++)
        {
            for (std::size_t k = j; k != l; k++)
            {
				if (nouveauStatsPlateau.plateau[k][i] != 0)
                {
                    nouvelleColonne[j] = nouveauStatsPlateau.plateau[k][i];
                    nouveauStatsPlateau.plateau[k][i] = 0;
                    break;
                }
            }
        }

        // Fusionner les éléments adjacents identiques.
        for (std::size_t j = 0; j != l - 1; j++)
        {
            if (nouvelleColonne[j] == nouvelleColonne[j + 1] && nouvelleColonne[j] != 0)
          	{
                nouvelleColonne[j] *= 2;    // Fusionner les cases
                nouvelleColonne[j + 1] = 0; // La case qui précède celle fusionnée devient 0.
                nouveauStatsPlateau.score += nouvelleColonne[j];
            }
        }

        // Mettre à jour le plateau.
        for (std::size_t j = 0; j < l; j++)
        {
            nouveauStatsPlateau.plateau[j][i] = nouvelleColonne[j];
        }

        // Placer à nouveau les éléments fusionnés en haut.
        nouvelleColonne = std::vector<int>(l, 0);
        for (std::size_t j = 0; j != l; j++)
        {
            for (std::size_t k = j; k != l; k++)
            {
				if (nouveauStatsPlateau.plateau[k][i] != 0)
                {
                    nouvelleColonne[j] = nouveauStatsPlateau.plateau[k][i];
                    nouveauStatsPlateau.plateau[k][i] = 0;
                    break;
                }
            }
        }

        // Mettre à jour le plateau.
        for (std::size_t j = 0; j < l; j++)
        {
            nouveauStatsPlateau.plateau[j][i] = nouvelleColonne[j];
        }
    }

    return nouveauStatsPlateau;
}

StatsPlateau déplacementBas(const StatsPlateau& statsPlateau)
{
    StatsPlateau nouveauStatsPlateau;
    nouveauStatsPlateau.plateau = statsPlateau.plateau;

  	const std::size_t l { nouveauStatsPlateau.plateau.size() }; // Nombre de lignes

    for (std::size_t i = 0; i != l; i++)
    {
        // Placer les éléments en bas.
        std::vector<int> nouvelleColonne(l, 0);
        for (std::size_t j = l; j--;)
        {
            for (std::size_t k = j + 1; k--;)
            {
				if (nouveauStatsPlateau.plateau[k][i] != 0)
                {
                    nouvelleColonne[j] = nouveauStatsPlateau.plateau[k][i];
                    nouveauStatsPlateau.plateau[k][i] = 0;
                    break;
                }
            }
        }

        // Fusionner les éléments adjacents identiques.
        for (std::size_t j = l; j-- && j != 0;)
        {
        	if (nouvelleColonne[j] == nouvelleColonne[j - 1] && nouvelleColonne[j] != 0)
          	{
                nouvelleColonne[j] *= 2;    // Fusionner les cases
                nouvelleColonne[j - 1] = 0; // La case qui précède celle fusionnée devient 0.
                nouveauStatsPlateau.score += nouvelleColonne[j];
            }
        }

        // Mettre à jour le plateau.
        for (std::size_t j = 0; j != l; j++)
        {
            nouveauStatsPlateau.plateau[j][i] = nouvelleColonne[j];
        }

        // Placer à nouveau les éléments fusionnés en bas.
        nouvelleColonne = std::vector<int>(l, 0);
        for (std::size_t j = l; j--;)
        {
            for (std::size_t k = j + 1; k--;)
            {
				if (nouveauStatsPlateau.plateau[k][i] != 0)
                {
                    nouvelleColonne[j] = nouveauStatsPlateau.plateau[k][i];
                    nouveauStatsPlateau.plateau[k][i] = 0;
                    break;
                }
            }
        }

        // Mettre à jour le plateau.
        for (std::size_t j = 0; j < l; j++)
        {
            nouveauStatsPlateau.plateau[j][i] = nouvelleColonne[j];
        }
    }

	return nouveauStatsPlateau;
}

StatsPlateau déplacement(const StatsPlateau& statsPlateau, int direction)
{
    StatsPlateau nouveauStatsPlateau;

    if (direction == 0)      // Haut
    {
		nouveauStatsPlateau = déplacementHaut(statsPlateau);
    }
    else if (direction == 1) // Droite
    {
		nouveauStatsPlateau = déplacementDroite(statsPlateau);
    }
    else if (direction == 2) // Bas
    {
		nouveauStatsPlateau = déplacementBas(statsPlateau);
    }
    else if (direction == 3) // Gauche
    {
		nouveauStatsPlateau = déplacementGauche(statsPlateau);
    }

    nouveauStatsPlateau.score += statsPlateau.score;

    /* Génération d'une nouvelle tuile si le déplacement est valide. */
    if (!plateauOccupé(nouveauStatsPlateau.plateau) && nouveauStatsPlateau.plateau != statsPlateau.plateau)
    {
      	ajouteElementPlateau(nouveauStatsPlateau.plateau);
    }

	return nouveauStatsPlateau;
}

std::string dessine(const Plateau& plateau)
{
    std::string dessin;

    /* Déterminer le nombre maximal dans le plateau. */
    int nombreMax { 0 };

    for (const std::vector<int>& l : plateau)
    {
        for (int n : l)
        {
            if (n > nombreMax) nombreMax = n;
        }
    }

    /* On obtient donc le nombre de caractère que cotient ce nombre maximal. */
    const std::size_t espaceNombreMax { std::to_string(nombreMax).size() };

    /* On calcul le nombre de caractère sur une ligne. */
    const std::size_t longueurLigneDessin { 1 + 4 * (1 + 2 + espaceNombreMax) };
    std::string etoiles;
    etoiles.resize(longueurLigneDessin, '*');

    /* On dessine ligne par ligne. */
    for (const std::vector<int>& l : plateau)
    {
        dessin += etoiles;
        dessin += "\n";

        /* Début du cloisonnement de la case. */
        dessin += "*";

        for (int n : l)
        {
            dessin += " ";

            std::string nouvelleCase = std::to_string(n);
            if (n == 0) nouvelleCase = " ";

            /* On calcul le nombre d'espaces totaux qu'on devra mettre. */
            const std::size_t nbEspaces       { espaceNombreMax - nouvelleCase.size() };
            const std::size_t nbEspacesGauche { nbEspaces / 2 };               // à gauche du nombre courant
            const std::size_t nbEspacesDroite { nbEspaces - nbEspacesGauche }; // à droite du nombre courant.

            /* Ajout des espaces dans la case. */
            dessin.append(nbEspacesGauche, ' '); // à gauche
            dessin += nouvelleCase;
            dessin.append(nbEspacesDroite, ' '); // à droite

            /* Cloisonnage de la case. */
            dessin += " *";
        }

        dessin += "\n";
    }

    dessin += etoiles;

    return dessin;
}

bool estTerminé(const Plateau& plateau)
{
    StatsPlateau statsPlateau;
    statsPlateau.plateau = plateau;

    return plateau == déplacementHaut(statsPlateau).plateau
        && plateau == déplacementDroite(statsPlateau).plateau
    	&& plateau == déplacementBas(statsPlateau).plateau
        && plateau == déplacementGauche(statsPlateau).plateau;
}

bool estGagnant(const Plateau& plateau)
{
    for (const std::vector<int>& ligne : plateau)
    {
      	for (int nombre : ligne)
        {
            if (nombre >= 2048) return true;
        }
    }

    return false;
}