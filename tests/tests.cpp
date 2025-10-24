#include "../modele.hpp"

#include <iostream>

/** Infrastructure de test par défaut.
 **/
#define CHECK(test) if (!(test)) std::cerr << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " << #test << std::endl;

/** Tests dédiés à tirer un 2 ou un 4 aléatoirement.
 **/
void testTireDeuxOuQuatre()
{
    std::cout << "Appel de testTireDeuxOuQuatre()..." << std::endl;

    for (size_t i { 0 }; i != 1000; i++) // Test sur 1000 tirages.
    {
        const int nombre { tireDeuxOuQuatre() };
        //std::cout << nombre << std::endl;
        CHECK(nombre == 2 || nombre == 4);
    }
}

/** Test dédié à la génération d'un plateau vide.
 **/
void testPlateauVide()
{
    std::cout << "Appel de testPlateauVide()..." << std::endl;

    // Pas besoin d'une boucle.
    // On sait ce que plateauVide() renvoie réellement !

    const Plateau vide { plateauVide() };
    const Plateau plateauVide
    {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };
    CHECK(vide == plateauVide);
}

/** Vérifie si le plateau est bien initialisé. i.e contient un 2 ou (logique) un 4.
 * @param plateau une référence constante d'un Plateau initialisé.
 * @return vrai si le plateau est correctement initialisé, faux sinon.
 **/
bool plateauInitialisé(const Plateau& plateau)
{
    size_t nombresInit { 0 };

    for (const std::vector<int>& ligne : plateau)
    {
        for (int nombre : ligne)
        {
            if (nombre == 2 || nombre == 4) nombresInit++;
        }
    }

    return nombresInit == 2;
}

/** Tests dédiés à la génération de plateaux de départ.
 **/
void testPlateauInitial()
{
    std::cout << "Appel de testPlateauInitial()..." << std::endl;

    for (size_t i { 0 }; i != 1000; i++) // Test sur 1000 générations.
    {
        const Plateau plateau { plateauInitial() };
        CHECK(plateauInitialisé(plateau));
    }
}

/** Tests dédiés à vérifier si un plateau ne contient pas de valeur nulle.
 **/
void testPlateauOccupé()
{
	std::cout << "Appel de testPlateauOccupé()..." << std::endl;

    const Plateau plateau1
    {
    	{ 2, 4 , 2 , 8  },
        { 4, 4 , 16, 32 },
        { 8, 16, 64, 2  },
        { 2, 8 , 4 , 2  }
    };
    CHECK(plateauOccupé(plateau1));

    const Plateau plateau2
    {
    	{ 4, 4 , 2 , 8  },
        { 4, 4 , 64, 16 },
        { 2, 32, 2 , 2  },
        { 0, 8 , 4 , 4  }
    };
    CHECK(!plateauOccupé(plateau2));

    const Plateau plateau3
    {
    	{ 0, 2 , 0 , 4  },
        { 4, 0 , 16, 0  },
        { 0, 64, 2 , 2  },
        { 2, 8 , 4 , 0  }
    };
    CHECK(!plateauOccupé(plateau3));
}

/** Compte le nombre d'éléments non nuls.
 * @param plateau une référence constante d'un Plateau initialisé.
 * @return un std::size_t le nombre d'éléments non nuls.
 **/
std::size_t compteElémentsNonNuls(const Plateau& plateau)
{
    size_t compteur { 0 };

    for (const std::vector<int>& ligne : plateau)
    {
        for (int nombre : ligne)
        {
            if (nombre != 0) compteur++;
        }
    }

    return compteur;
}

/** Tests dédiés à vérifier si un élément est bien rajouté à un plateau.
 **/
void testAjouteElementPlateau()
{
	std::cout << "Appel de testAjouteElementPlateau()..." << std::endl;

	Plateau plateau1
    {
    	{ 2, 4 , 2 , 8  },
        { 4, 4 , 0 , 32 },
        { 8, 16, 64, 2  },
        { 2, 8 , 4 , 2  }
    };
    const std::size_t compteur1 { compteElémentsNonNuls(plateau1) };
    ajouteElementPlateau(plateau1);
    const std::size_t nouveauCompteur1 { compteElémentsNonNuls(plateau1) };
    CHECK(nouveauCompteur1 == compteur1 + 1);

    Plateau plateau2
    {
    	{ 0, 2, 2, 0 },
        { 2, 2, 2, 2 },
        { 2, 2, 2, 2 },
        { 0, 2, 2, 0 }
    };
    const std::size_t compteur2 { compteElémentsNonNuls(plateau2) };
    ajouteElementPlateau(plateau2);
    const std::size_t nouveauCompteur2 { compteElémentsNonNuls(plateau2) };
    CHECK(nouveauCompteur2 == compteur2 + 1);

    Plateau plateau3
    {
    	{ 2, 2, 2, 0 },
        { 0, 0, 0, 2 },
        { 2, 0, 2, 2 },
        { 2, 2, 2, 2 }
    };
    const std::size_t compteur3 { compteElémentsNonNuls(plateau3) };
    ajouteElementPlateau(plateau3);
    const std::size_t nouveauCompteur3 { compteElémentsNonNuls(plateau3) };
    CHECK(nouveauCompteur3 == compteur3 + 1);
}

/** Tests dédiés à la vérification de mouvements vers la gauche.
 **/
void testDéplacementGauche()
{
    std::cout << "Appel de testDéplacementGauche()..." << std::endl;

    StatsPlateau statsPlateau1;
    statsPlateau1.plateau = {
        { 0, 2, 0, 0  },
        { 4, 4, 4, 4  },
        { 2, 8, 2, 16 },
        { 0, 2, 0, 2  }
    };
    StatsPlateau resultat1;
    resultat1.plateau = {
      	{ 2, 0, 0, 0  },
        { 8, 8, 0, 0  },
        { 2, 8, 2, 16 },
        { 4, 0, 0, 0  },
    };
    resultat1.score = 0 + (8 + 8) + 0 + 4;
    CHECK(resultat1 == déplacementGauche(statsPlateau1));

    StatsPlateau statsPlateau2;
    statsPlateau2.plateau = {
        { 0, 16, 16, 16 },
        { 4, 0 , 4 , 0  },
        { 4, 4 , 8 , 16 },
        { 0, 2 , 0 , 2  }
    };
    StatsPlateau resultat2;
    resultat2.plateau = {
      	{ 32, 16, 0 , 0 },
        { 8 , 0 , 0 , 0 },
        { 8 , 8 , 16, 0 },
        { 4 , 0 , 0 , 0 },
    };
    resultat2.score = 32 + 8 + 8 + 4;
    CHECK(resultat2 == déplacementGauche(statsPlateau2));

    StatsPlateau statsPlateau3;
    statsPlateau3.plateau = {
        { 0, 0, 0, 64 },
        { 4, 0, 8, 8  },
        { 2, 0, 0, 2  },
        { 4, 4, 4, 0  }
    };
    StatsPlateau resultat3;
    resultat3.plateau = {
      	{ 64, 0 , 0, 0 },
        { 4 , 16, 0, 0 },
        { 4 , 0 , 0, 0 },
        { 8 , 4 , 0, 0 },
    };
    resultat3.score = 0 + 16 + 4 + 8;
    CHECK(resultat3 == déplacementGauche(statsPlateau3));
}

/** Tests dédiés à la vérification de mouvements vers la droite.
 **/
void testDéplacementDroite()
{
    std::cout << "Appel de testDéplacementDroite()..." << std::endl;

    StatsPlateau statsPlateau1;
    statsPlateau1.plateau = {
        { 2, 4, 4, 0 },
        { 0, 4, 4, 8 },
        { 2, 2, 2, 2 },
        { 4, 2, 4, 2 }
    };
    StatsPlateau resultat1;
    resultat1.plateau = {
      	{ 0, 0, 2, 8 },
        { 0, 0, 8, 8 },
        { 0, 0, 4, 4 },
        { 4, 2, 4, 2 },
    };
    resultat1.score = 8 + 8 + (4 + 4) + 0;
    CHECK(resultat1 == déplacementDroite(statsPlateau1));

    StatsPlateau statsPlateau2;
    statsPlateau2.plateau = {
        { 2 , 2, 0, 0 },
        { 4 , 0, 4, 4 },
        { 4 , 0, 0, 4 },
        { 16, 0, 0, 0 }
    };
    StatsPlateau resultat2;
    resultat2.plateau = {
      	{ 0, 0, 0, 4  },
        { 0, 0, 4, 8  },
        { 0, 0, 0, 8  },
        { 0, 0, 0, 16 },
    };
    resultat2.score = 4 + 8 + 8 + 0;
    CHECK(resultat2 == déplacementDroite(statsPlateau2));

    StatsPlateau statsPlateau3;
    statsPlateau3.plateau = {
        { 2 , 4, 8 , 16 },
        { 4 , 4, 0 , 8  },
        { 4 , 0, 2 , 4  },
        { 32, 0, 32, 64 }
    };
    StatsPlateau resultat3;
    resultat3.plateau = {
      	{ 2, 4, 8 , 16 },
        { 0, 0, 8 , 8  },
        { 0, 4, 2 , 4  },
        { 0, 0, 64, 64 },
    };
    resultat3.score = 0 + 8 + 0 + 64;
    CHECK(resultat3 == déplacementDroite(statsPlateau3));
}

/** Tests dédiés à la vérification de mouvements vers le haut.
 **/
void testDéplacementHaut()
{
    std::cout << "Appel de testDéplacementHaut()..." << std::endl;

    StatsPlateau statsPlateau1;
    statsPlateau1.plateau = {
        { 16, 4, 0, 0  },
        { 8 , 4, 8, 0  },
        { 4 , 4, 8, 16 },
        { 2 , 4, 0, 0  }
    };
    StatsPlateau resultat1;
    resultat1.plateau = {
      	{ 16, 8, 16, 16 },
        { 8 , 8, 0 , 0  },
        { 4 , 0, 0 , 0  },
        { 2 , 0, 0 , 0  },
    };
    resultat1.score = 0 + (8 + 8) + 16 + 0;
    CHECK(resultat1 == déplacementHaut(statsPlateau1));

    StatsPlateau statsPlateau2;
    statsPlateau2.plateau = {
        { 0  , 8, 0, 32 },
        { 128, 8, 0, 16 },
        { 0  , 4, 8, 16 },
        { 128, 8, 0, 0  }
    };
    StatsPlateau resultat2;
    resultat2.plateau = {
      	{ 256, 16, 8, 32 },
        { 0  , 4 , 0, 32 },
        { 0  , 8 , 0, 0  },
        { 0  , 0 , 0, 0  },
    };
    resultat2.score = 256 + 16 + 0 + 32;
    CHECK(resultat2 == déplacementHaut(statsPlateau2));

    StatsPlateau statsPlateau3;
    statsPlateau3.plateau = {
        { 256, 1024, 0, 0 },
        { 0,   512 , 0, 4 },
        { 128, 0   , 0, 0 },
        { 128, 512 , 2, 4 }
    };
    StatsPlateau resultat3;
    resultat3.plateau = {
      	{ 256, 1024, 2, 8 },
        { 256, 1024, 0, 0 },
        { 0  , 0   , 0, 0 },
        { 0  , 0   , 0, 0 },
    };
    resultat3.score = 256 + 1024 + 0 + 8;
    CHECK(resultat3 == déplacementHaut(statsPlateau3));
}

/** Tests dédiés à la vérification de mouvements vers le bas.
 **/
void testDéplacementBas()
{
    std::cout << "Appel de testDéplacementBas()..." << std::endl;

    StatsPlateau statsPlateau1;
    statsPlateau1.plateau = {
        { 2, 0, 2 , 8 },
        { 0, 0, 4 , 8 },
        { 2, 0, 8 , 8 },
        { 2, 0, 16, 8 }
    };
    StatsPlateau resultat1;
    resultat1.plateau = {
      	{ 0, 0, 2 , 0  },
        { 0, 0, 4 , 0  },
        { 2, 0, 8 , 16 },
        { 4, 0, 16, 16 },
    };
    resultat1.score = 4 + 0 + 0 + (16 + 16);
    CHECK(resultat1 == déplacementBas(statsPlateau1));

    StatsPlateau statsPlateau2;
    statsPlateau2.plateau = {
        { 4, 4, 2, 8 },
        { 4, 0, 0, 4 },
        { 4, 4, 0, 4 },
        { 8, 0, 0, 8 }
    };
    StatsPlateau resultat2;
    resultat2.plateau = {
      	{ 0, 0, 0, 0 },
        { 4, 0, 0, 8 },
        { 8, 0, 0, 8 },
        { 8, 8, 2, 8 },
    };
    resultat2.score = 8 + 8 + 0 + 8;
    CHECK(resultat2 == déplacementBas(statsPlateau2));

    StatsPlateau statsPlateau3;
    statsPlateau3.plateau = {
        { 2, 4, 8, 4 },
        { 0, 4, 0, 4 },
        { 0, 2, 4, 8 },
        { 2, 0, 4, 8 }
    };
    StatsPlateau resultat3;
    resultat3.plateau = {
      	{ 0, 0, 0, 0  },
        { 0, 0, 0, 0  },
        { 0, 8, 8, 8  },
        { 4, 2, 8, 16 },
    };
    resultat3.score = 4 + 8 + 8 + (16 + 8);
    CHECK(resultat3 == déplacementBas(statsPlateau3));
}

/** Tests dédiés à la vérification des mouvements.
 **/
void testDéplacements()
{
 	std::cout << "Appel de testDéplacements()..." << std::endl;

    StatsPlateau statsPlateau1;
    statsPlateau1.plateau = {
        { 2, 0, 8, 0 },
        { 2, 2, 8, 0 },
        { 4, 0, 8, 8 },
        { 2, 0, 8, 8 }
    };
    const StatsPlateau resultat1 { déplacement(statsPlateau1, 0) };
    const size_t scoreAttendu1 { 4 + 0 + (16 + 16) + 16 };
    CHECK(statsPlateau1.plateau != resultat1.plateau && scoreAttendu1 == resultat1.score);

    StatsPlateau statsPlateau2;
    statsPlateau2.plateau = {
        { 16, 16, 32, 32 },
        { 16, 8 , 8 , 0  },
        { 32, 8 , 8 , 16 },
        { 32, 0 , 32, 0  }
    };
    const StatsPlateau resultat2 { déplacement(statsPlateau2, 1) };
    const size_t scoreAttendu2 { (64 + 32) + 16 + 16 + 64 };
    CHECK(statsPlateau2.plateau != resultat2.plateau && scoreAttendu2 == resultat2.score);

    StatsPlateau statsPlateau3;
    statsPlateau3.plateau = {
        { 32 , 16, 4, 0 },
        { 32 , 8 , 0, 2 },
        { 64 , 8 , 2, 2 },
        { 128, 16, 0, 0 }
    };
    const StatsPlateau resultat3 { déplacement(statsPlateau3, 2) };
    const size_t scoreAttendu3 { 64 + 16 + 0 + 4 };
    CHECK(statsPlateau3.plateau != resultat3.plateau && scoreAttendu3 == resultat3.score);

    StatsPlateau statsPlateau4;
    statsPlateau4.plateau = {
        { 0  , 16, 16, 0 },
        { 0  , 0 , 2 , 2 },
        { 64 , 64, 64, 2 },
        { 128, 64, 64, 0 }
    };
    const StatsPlateau resultat4 { déplacement(statsPlateau4, 3) };
    const size_t scoreAttendu4 { 32 + 4 + 128 + 128 };
    CHECK(statsPlateau4.plateau != resultat4.plateau && scoreAttendu4 == resultat4.score);
}

/** Tests dédiés aux dessins de plateaux.
 **/
void testDessinePlateau()
{
    std::cout << "Appel de testDessinePlateau()..." << std::endl;

    /*
     * Configuration inventée :
     * - Pour voir si on prend bien 2048 en tant que maximum (1 caractère par case).
     * - Pour voir ce qu'il se passe quand on doit représenter
     *   toutes les cases sous 1 caractère.
     */
    const Plateau plateau1
    {
        { 0, 0, 0, 0 },
        { 2, 0, 2, 4 },
        { 0, 0, 0, 0 },
        { 2, 4, 8, 0 }
    };
    const std::string dessin1
    {
        "*****************\n"
        "*   *   *   *   *\n"
        "*****************\n"
        "* 2 *   * 2 * 4 *\n"
        "*****************\n"
        "*   *   *   *   *\n"
        "*****************\n"
        "* 2 * 4 * 8 *   *\n"
        "*****************"
    };
    //std::cout << dessine(plateau1) << std::endl;
    CHECK(dessine(plateau1) == dessin1);

    /*
     * Configuration donnée par défaut sur la page du projet.
     * - Pour voir si on prend bien 32 en tant que maximum (2 caractères par case).
     * - Pour voir ce qu'il se passe quand on doit représenter
     *   toutes les cases sous 2 caractères.
     */
    const Plateau plateau2
    {
        { 0 , 16, 0, 0  },
        { 2 , 0 , 4, 2  },
        { 32, 0 , 0, 0  },
        { 2 , 0 , 0, 32 }
    };
    const std::string dessin2
    {
        "*********************\n"
        "*    * 16 *    *    *\n"
        "*********************\n"
        "* 2  *    * 4  * 2  *\n"
        "*********************\n"
        "* 32 *    *    *    *\n"
        "*********************\n"
        "* 2  *    *    * 32 *\n"
        "*********************"
    };
    //std::cout << dessine(plateau2) << std::endl;
    CHECK(dessine(plateau2) == dessin2);

    /*
     * Configuration donnée par défaut sur la page du projet.
     * - Pour voir si on prend bien 128 en tant que maximum (3 caractères par case).
     * - Pour voir ce qu'il se passe quand on doit représenter
     *   toutes les cases sous 3 caractères.
     */
    const Plateau plateau3
    {
        { 128, 128, 0, 0 },
        { 2  , 16 , 4, 2 },
        { 0  , 0  , 0, 0 },
        { 2  , 0  , 0, 0 }
    };
    const std::string dessin3
    {
        "*************************\n"
        "* 128 * 128 *     *     *\n"
        "*************************\n"
        "*  2  * 16  *  4  *  2  *\n"
        "*************************\n"
        "*     *     *     *     *\n"
        "*************************\n"
        "*  2  *     *     *     *\n"
        "*************************"
    };
    //std::cout << dessine(plateau3) << std::endl;
    CHECK(dessine(plateau3) == dessin3);

    /*
     * Configuration inventée :
     * - Pour voir si on prend bien 2048 en tant que maximum (4 caractères par case).
     * - Pour voir ce qu'il se passe quand on doit représenter
     *   toutes les cases sous 2 caractères.
     * - Pour plus de valeurs différentes.
     */
    const Plateau plateau4
    {
        { 0, 8   , 0   , 4    },
        { 0, 4   , 64  , 0    },
        { 2, 256 , 2048, 512  },
        { 0, 0   , 512 , 0    }
    };
    const std::string dessin4
    {
        "*****************************\n"
        "*      *  8   *      *  4   *\n"
        "*****************************\n"
        "*      *  4   *  64  *      *\n"
        "*****************************\n"
        "*  2   * 256  * 2048 * 512  *\n"
        "*****************************\n"
        "*      *      * 512  *      *\n"
        "*****************************"
    };
    //std::cout << dessine(plateau4) << std::endl;
    CHECK(dessine(plateau4) == dessin4);

    /*
     * Configuration inventée :
     * - Pour voir si on prend bien 16384 en tant que maximum (5 caractères par case).
     * - Pour voir ce qu'il se passe quand on doit représenter
     *   toutes les cases sous 5 caractères.
     * - Plus de valeurs différentes.
     */
    const Plateau plateau5
    {
        { 0, 0   , 512 , 1024  },
        { 0, 2048, 0   , 0     },
        { 2, 0   , 8192, 4     },
        { 4, 4   , 2   , 16384 }
    };
    const std::string dessin5
    {
        "*********************************\n"
        "*       *       *  512  * 1024  *\n"
        "*********************************\n"
        "*       * 2048  *       *       *\n"
        "*********************************\n"
        "*   2   *       * 8192  *   4   *\n"
        "*********************************\n"
        "*   4   *   4   *   2   * 16384 *\n"
        "*********************************"
    };
    //std::cout << dessine(plateau5) << std::endl;
    CHECK(dessine(plateau5) == dessin5);
}

/** Tests dédiés à déterminer si la partie est terminée.
 **/
void testEstTerminé()
{
    std::cout << "Appel de testEstTerminé()..." << std::endl;

    /*
     * Configuration inventée :
     * - Pour voir si effectivement la partie est perdue.
     */
    const Plateau gameover1
    {
        { 4, 2  , 16, 2  },
        { 2, 8  , 64, 4  },
        { 4, 128, 8 , 2  },
        { 2, 4  , 2 , 16 }
    };
    CHECK(estTerminé(gameover1));

    /*
     * Configuration inventée :
     * - Pour voir si la partie n'est pas terminée.
     */
    const Plateau pasTerminé1
    {
        { 4, 2  , 16 , 4  },
        { 2, 8  , 64 , 2  },
        { 4, 128, 128, 4  },
        { 2, 4  , 2  , 16 }
    };
    CHECK(!estTerminé(pasTerminé1));

    const Plateau pasTerminé2
    {
        { 4, 2  , 16, 4  },
        { 2, 2  , 64, 2  },
        { 4, 64 , 4 , 8  },
        { 2, 4  , 2 , 16 }
    };
    CHECK(!estTerminé(pasTerminé2));
}

/** Tests dédiés à déterminer si le joueur a gagné (i.e. a dépassé ou a au moins un 2048).
 **/
void testEstGagnant()
{
    std::cout << "Appel de testEstGagnant()..." << std::endl;

    /*
     * Configuration inventée :
     * - Pour voir si effectivement la partie est gagnée car le plateau
     *   contient au moins un 2048.
     */
    const Plateau win1
    {
        { 0  , 512, 16  , 2  },
        { 256, 0  , 0   , 0  },
        { 128, 16 , 2048, 2  },
        { 32 , 4  , 32  , 64 }
    };
    CHECK(estGagnant(win1));

    /*
     * Configuration inventée :
     * - Pour voir si effectivement la partie est gagnée car le plateau
     *   contient au moins une valeur supérieure à 2048.
     */
    const Plateau win2
    {
        { 0  , 512, 16  , 0   },
        { 64 , 0  , 0   , 4   },
        { 0  , 128, 4096, 0   },
        { 2  , 4  , 0   , 128 }
    };
    CHECK(estGagnant(win2));

    /*
     * Configuration inventée :
     * - Pour voir si la partie n'est pas gagnée car le plateau
     *   ne contient pas un 2048 (ou plus).
     */
    const Plateau pasGagné
    {
        { 0  , 512, 16  , 2  },
        { 256, 0  , 0   , 0  },
        { 128, 16 , 2048, 2  },
        { 32 , 4  , 32  , 64 }
    };
    CHECK(estGagnant(win1));
}

int main()
{
    testTireDeuxOuQuatre();
    testPlateauVide();
    testPlateauInitial();
    testPlateauOccupé();
    testAjouteElementPlateau();

    testDéplacementGauche();
    testDéplacementDroite();
    testDéplacementHaut();
    testDéplacementBas();
    testDéplacements();

    testDessinePlateau();
    testEstTerminé();
    testEstGagnant();

    return 0;
}