#include "modele.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <atomic>
#include <iostream>

constexpr int LONGUEUR_FENETRE          { 800 };
constexpr int LARGEUR_FENETRE           { 800 };

constexpr int LONGUEUR_PLATEAU          { 500 };
constexpr int MARGE_PLATEAU             { 15 };

constexpr int LONGUEUR_CASE_SCORE       { 150 };
constexpr int LARGEUR_CASE_SCORE        { 50 };

constexpr int ESPACE_ZONE               { 20 };

constexpr int LONGUEUR_CASE_MODE_JEU    { 500 };
constexpr int LARGEUR_CASE_MODE_JEU     { 100 };

constexpr int SPEED_2048_TIMER_INTERVAL { 1000 };

struct ModeJeu
{
    std::string titre;
    std::string description;
};

class GUIApplication
{
public:
    GUIApplication();
   	~GUIApplication();

    /** Affiche la fenêtre et écoute les événements.
     **/
    void afficheFenetre();

    /** Callback du timer qui rajoute un élément dans une tuile à chaque seconde.
     * Puis, il raffiche le plateau correctement.
     **/
	void speed2048Plateau();

private:
    /** Dessine les modes de jeux sur la page principale.
     * @param positionY un entier
     **/
    void dessineModesJeux(int positionY);

    /** Dessine le menu principal.
     **/
    void dessineMenuPrincipal();

    /** Dessine le mode de jeu sélectionné.
     **/
    void dessineModeJeu();

    /** Dessine les règles du mode de jeu sélectionné.
     **/
    void dessineRègles();

    /** Dessine le score du mode de jeu sélectionné.
     **/
    void dessineScore();

    /** Dessine gameover.
     * Attention: le plateau est supposé être déjà dessiné pour que
     * ce nouveau dessin soit sur le plateau.
     **/
    void dessineGameover();

    /** Dessine le carré représentant le plateau.
 	 **/
    void dessinePlateau();

    /** Dessine les cases du plateau et ses valeurs.
     **/
    void dessineCases();

    /** Dessine le mode de jeu sélectionné.
     **/
    void dessineJeu();

	/** Remise à jour des stats.
	 **/
    void resetJeu();

    const SDL_Color m_couleurBlanche = { 0xff, 0xff, 0xff, 0xff };
    const SDL_Color m_couleurNoire = { 0x33, 0x33, 0x33, 0xff };

    SDL_Window* m_fenetre { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Event m_event;

	const std::string m_gagneMsg { "Vous avez gagné !" };
	const std::string m_aideMsg
	{
		"Utiliser les flèches haut et bas ainsi que la touche entrer pour sélectionner le mode de jeu. "
		"Entrer échap pour revenir à ce menu principal."
	};

	const std::string m_règles
	{
		"Utiliser les flèches pour déplacer les tuiles. "
		"Lorsque deux tuiles ayant la même valeur se touchent, elles fusionnent !"
	};

    bool m_ecranPrincipal { true };
    bool m_gagnant { false };
    bool m_terminé { false };
    bool m_quit { false };

    const std::vector<ModeJeu> m_modes
    {
    	{ "2048",       "La version classique du 2048." },
        { "2048 Speed", "À chaque seconde, une nouvelle tuile apparaît." }
    };
    std::size_t m_modeJeu { 0 };
    SDL_TimerID m_timerID { 0 }; // Uniquement pour 2048 Speed.

    StatsPlateau m_statsPlateau;
};

/** Callback appelé toutes les secondes lorsque speed 2048 est sélectionné.
 **/
static Uint32 timerCallback(Uint32 interval, void* param)
{
	if (param == nullptr)
	{
		std::cerr << "param in timerCallback is nullptr." << std::endl;
		return interval;
	}

	std::atomic<bool>* timerExpiré = static_cast<std::atomic<bool>*>(param);
	timerExpiré->store(true);

	return interval;
}

GUIApplication::GUIApplication()
{
    m_statsPlateau.init();
}

GUIApplication::~GUIApplication()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_fenetre);
}

void GUIApplication::afficheFenetre()
{
    m_fenetre = SDL_CreateWindow("Jeux 2048", 20, 20, LONGUEUR_FENETRE, LARGEUR_FENETRE, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_fenetre, -1, SDL_RENDERER_PRESENTVSYNC);

    dessineMenuPrincipal();

	std::atomic<bool> timerExpiré { false };

    while (!m_quit)
    {
        while (SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
                /* Quand l'utilisateur appuie sur des touches de son clavier. */
                case SDL_KEYDOWN:
                {
                	if (!m_ecranPrincipal)
                    {
                        if(!m_terminé)
                        {
                          	const Plateau plateauActuel { m_statsPlateau.plateau };

                      		if(m_event.key.keysym.sym == SDLK_UP)
                        	{
                          		m_statsPlateau = déplacement(m_statsPlateau, 0);
                        	}
                        	else if(m_event.key.keysym.sym == SDLK_RIGHT)
                        	{
                          		m_statsPlateau = déplacement(m_statsPlateau, 1);
                        	}
                        	else if(m_event.key.keysym.sym == SDLK_DOWN)
                        	{
                          		m_statsPlateau = déplacement(m_statsPlateau, 2);
                        	}
                        	else if(m_event.key.keysym.sym == SDLK_LEFT)
                        	{
                            	m_statsPlateau = déplacement(m_statsPlateau, 3);
                        	}

                            if (m_modeJeu == 1 && (m_event.key.keysym.sym == SDLK_UP
                                                   || m_event.key.keysym.sym == SDLK_RIGHT
                                                   || m_event.key.keysym.sym == SDLK_DOWN
                                                   || m_event.key.keysym.sym == SDLK_LEFT) && plateauActuel != m_statsPlateau.plateau)
                            {
                            	if (SDL_RemoveTimer(m_timerID) == SDL_TRUE)
                                {
                                	m_timerID = SDL_AddTimer(SPEED_2048_TIMER_INTERVAL, &timerCallback, &timerExpiré);
                                }
                            }

                        	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);  // Blanc
                        	SDL_RenderClear(m_renderer); // Remplir l'écran avec la couleur

                        	dessineJeu();

                        	/* Détecter si le joueur à gagner. */
                        	m_gagnant = estGagnant(m_statsPlateau.plateau);
                        	/* Détecter si le jeu est terminé après le nouveau déplacement. */
                        	m_terminé = estTerminé(m_statsPlateau.plateau);

                        	if (m_terminé && !m_gagnant)
                        	{
                        		dessineGameover();
                        	}

                        	SDL_RenderPresent(m_renderer);
                        }

                        if(m_event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            if (m_modeJeu == 1) // 2048 Speed
                            {
                                SDL_RemoveTimer(m_timerID);
                            }

							resetJeu();

                            m_ecranPrincipal = true;
                            dessineMenuPrincipal();
                        }
                        else if(m_terminé && !m_gagnant && m_event.key.keysym.sym == SDLK_RETURN)
                        {
                        	resetJeu();

                        	if (m_modeJeu == 1) // 2048 Speed
                        	{
                        		m_timerID = SDL_AddTimer(SPEED_2048_TIMER_INTERVAL, &timerCallback, &timerExpiré);
                            }

                        	SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 255);  // Blanc
                        	SDL_RenderClear(m_renderer); // Remplir l'écran avec la couleur
                        	dessineJeu();
                        	SDL_RenderPresent(m_renderer);
                        }
                    }
                    else
                    {
                        if(m_event.key.keysym.sym == SDLK_UP || m_event.key.keysym.sym == SDLK_DOWN)
                        {
                            m_modeJeu += 1;
                            m_modeJeu %= 2;
                            dessineMenuPrincipal();
                        }
                        else if(m_event.key.keysym.sym == SDLK_RETURN)
                        {
                        	if (m_modeJeu == 1) // 2048 Speed
    						{
								m_timerID = SDL_AddTimer(SPEED_2048_TIMER_INTERVAL, &timerCallback, &timerExpiré);
    						}

                            m_ecranPrincipal = false;

                        	SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 255);  // Blanc
                        	SDL_RenderClear(m_renderer); // Remplir l'écran avec la couleur
                        	dessineJeu();
                        	SDL_RenderPresent(m_renderer);
                        }
                    }
                    break;
                }

                /* Quand l'utilisateur ferme la fenêtre. */
                case SDL_QUIT:
                {
                    m_quit = true;
                    break;
                }
            }
        }

        if (timerExpiré.load())
        {
    		speed2048Plateau();
        	timerExpiré.store(false);
        }
    }
}

void GUIApplication::speed2048Plateau()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);  // Blanc
    SDL_RenderClear(m_renderer); // Remplir l'écran avec la couleur

    const bool plateauEstOccupé { plateauOccupé(m_statsPlateau.plateau) };

    if (!plateauEstOccupé)
    {
      	ajouteElementPlateau(m_statsPlateau.plateau);
    }

    // On dessine à nouveau le jeu.
	dessineJeu();

    if (plateauEstOccupé)
    {
      	m_terminé = true;
      	dessineGameover();
        SDL_RemoveTimer(m_timerID);
    }

    SDL_RenderPresent(m_renderer);
}

void GUIApplication::dessineModesJeux(int positionY)
{
    TTF_Font* fontTitre { TTF_OpenFont("fonts/ClearSans-Bold.ttf", 25) };
    TTF_Font* fontDescription { TTF_OpenFont("fonts/ClearSans-Medium.ttf", 20) };

    const int positionCaseX { (LONGUEUR_FENETRE - LONGUEUR_CASE_MODE_JEU) / 2 };
    int positionCaseY { 0 };

    for (std::size_t i { 0 }; i != m_modes.size(); i++)
    {
        if (m_modeJeu == i)
        {
            SDL_SetRenderDrawColor(m_renderer, 0x99, 0x99, 0x99, 0xff);
        }
        else
        {
            SDL_SetRenderDrawColor(m_renderer, 0xcc, 0xcc, 0xcc, 0xff);
        }

        positionCaseY = positionY + static_cast<int>(i) * (ESPACE_ZONE + LARGEUR_CASE_MODE_JEU);

        const int positionTitreX { positionCaseX + MARGE_PLATEAU };
        const int longueurTitre { 150 };
        const int positionDescriptionX { positionCaseX + 2 * MARGE_PLATEAU + longueurTitre };
        const int longueurDescription { LONGUEUR_CASE_MODE_JEU - (2 * MARGE_PLATEAU + longueurTitre) };

		SDL_Rect casePlateau { positionCaseX, positionCaseY, LONGUEUR_CASE_MODE_JEU, LARGEUR_CASE_MODE_JEU };
        SDL_RenderFillRect(m_renderer, &casePlateau);

        // Ecriture du titre du jeu (2048 / 2048 Speed ...)
        SDL_Surface* surfaceTitre = TTF_RenderText_Blended(fontTitre, m_modes[i].titre.data(), m_couleurBlanche);

        SDL_Texture* textureTitre = SDL_CreateTextureFromSurface(m_renderer, surfaceTitre);

		SDL_Rect titreRect;
		titreRect.x = positionTitreX;
		titreRect.y = positionCaseY + ((LARGEUR_CASE_MODE_JEU - surfaceTitre->h) / 2);
		titreRect.w = surfaceTitre->w;
		titreRect.h = surfaceTitre->h;

		SDL_RenderCopy(m_renderer, textureTitre, NULL, &titreRect);

		SDL_FreeSurface(surfaceTitre);
		SDL_DestroyTexture(textureTitre);

        // Ecriture de la description du jeu.
        SDL_Surface* surfaceDescription = TTF_RenderUTF8_Blended_Wrapped(fontDescription, m_modes[i].description.data(), m_couleurBlanche,
                                                                         longueurDescription);

        SDL_Texture* textureDescription = SDL_CreateTextureFromSurface(m_renderer, surfaceDescription);

		SDL_Rect descriptionRect;
		descriptionRect.x = positionDescriptionX;
		descriptionRect.y = positionCaseY + ((LARGEUR_CASE_MODE_JEU - surfaceDescription->h) / 2);
		descriptionRect.w = surfaceDescription->w;
		descriptionRect.h = surfaceDescription->h;

		SDL_RenderCopy(m_renderer, textureDescription, NULL, &descriptionRect);

		SDL_FreeSurface(surfaceDescription);
		SDL_DestroyTexture(textureDescription);
    }

    SDL_Surface* messageAideSurface = TTF_RenderUTF8_Blended_Wrapped(fontDescription, m_aideMsg.data(), m_couleurNoire, LONGUEUR_CASE_MODE_JEU);
    SDL_Texture* aideTexture = SDL_CreateTextureFromSurface(m_renderer, messageAideSurface);
	SDL_Rect aideRect;
	aideRect.x = positionCaseX;
	aideRect.y = positionCaseY + LARGEUR_CASE_MODE_JEU + ESPACE_ZONE;
	aideRect.w = messageAideSurface->w;
	aideRect.h = messageAideSurface->h;
	SDL_RenderCopy(m_renderer, aideTexture, NULL, &aideRect);
    SDL_FreeSurface(messageAideSurface);
    SDL_DestroyTexture(aideTexture);

    TTF_CloseFont(fontTitre);
    TTF_CloseFont(fontDescription);
}

void GUIApplication::dessineMenuPrincipal()
{
  	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);  // Blanc
    SDL_RenderClear(m_renderer); // Remplir l'écran avec la couleur

    const int positionCaseY { ((LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2) - 2 * LARGEUR_CASE_SCORE - ESPACE_ZONE };

    TTF_Font* font { TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50) };

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, "Modes de jeux 2048", m_couleurNoire);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);
	SDL_Rect messageRect;
	messageRect.x = ((LONGUEUR_FENETRE - surfaceMessage->w) / 2);
	messageRect.y = positionCaseY + ((LARGEUR_CASE_SCORE - surfaceMessage->h) / 2);
	messageRect.w = surfaceMessage->w;
	messageRect.h = surfaceMessage->h;
	SDL_RenderCopy(m_renderer, texture, NULL, &messageRect);

	SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(font);

    dessineModesJeux(positionCaseY + messageRect.h);

    SDL_RenderPresent(m_renderer);
}

void GUIApplication::dessineModeJeu()
{
    const int positionCaseY { ((LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2) - 2*LARGEUR_CASE_SCORE - 2*ESPACE_ZONE };

    TTF_Font* font { TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50) };

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, m_modes[m_modeJeu].titre.data(), m_couleurNoire);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);

	SDL_Rect messageRect;
	messageRect.x = ((LONGUEUR_FENETRE - surfaceMessage->w) / 2);
	messageRect.y = positionCaseY + ((LARGEUR_CASE_SCORE - surfaceMessage->h) / 2);
	messageRect.w = surfaceMessage->w;
	messageRect.h = surfaceMessage->h;

	SDL_RenderCopy(m_renderer, texture, NULL, &messageRect);

	SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(font);
}

void GUIApplication::dessineRègles()
{
    const int positionCaseY { ((LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2) + LONGUEUR_PLATEAU + ESPACE_ZONE };

    TTF_Font* font { TTF_OpenFont("fonts/ClearSans-Bold.ttf", 20) };

    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Blended_Wrapped(font, m_règles.data(), m_couleurNoire, LONGUEUR_PLATEAU);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);

	SDL_Rect messageRect;
	messageRect.x = ((LONGUEUR_FENETRE - surfaceMessage->w) / 2);
	messageRect.y = positionCaseY;
	messageRect.w = surfaceMessage->w;
	messageRect.h = surfaceMessage->h;

	SDL_RenderCopy(m_renderer, texture, NULL, &messageRect);

	SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(font);
}

/** Dessine la case et la valeur du score au haut à droite.
 */
void GUIApplication::dessineScore()
{
    // Dessin du rectangle
    const int positionCaseX { ((LONGUEUR_FENETRE + LONGUEUR_PLATEAU) / 2) - LONGUEUR_CASE_SCORE };
    const int positionCaseY { ((LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2) - LARGEUR_CASE_SCORE - ESPACE_ZONE };

    SDL_Rect rectangle { positionCaseX, positionCaseY, LONGUEUR_CASE_SCORE, LARGEUR_CASE_SCORE };
    SDL_SetRenderDrawColor(m_renderer, 0xcc, 0xcc, 0xcc, 0xff); //#bcaca0
    SDL_RenderFillRect(m_renderer, &rectangle);

    // Dessine la valeur
    TTF_Font* font { TTF_OpenFont("fonts/ClearSans-Bold.ttf", 25) };

    SDL_Surface* scoreSurface = TTF_RenderText_Blended(font, std::to_string(m_statsPlateau.score).data(), m_couleurBlanche);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(m_renderer, scoreSurface);
	SDL_Rect scoreRect;
	scoreRect.x = positionCaseX + ((LONGUEUR_CASE_SCORE - scoreSurface->w) / 2);
	scoreRect.y = positionCaseY + ((LARGEUR_CASE_SCORE - scoreSurface->h) / 2);
	scoreRect.w = scoreSurface->w;
	scoreRect.h = scoreSurface->h;
	SDL_RenderCopy(m_renderer, scoreTexture, NULL, &scoreRect);
	SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);

    if (m_gagnant)
    {
		SDL_Surface* gagneSurface = TTF_RenderUTF8_Blended_Wrapped(font, m_gagneMsg.data(), m_couleurNoire,
	                                                               positionCaseX - ((LONGUEUR_FENETRE - LONGUEUR_PLATEAU) / 2));
		SDL_Texture* gagneTexture = SDL_CreateTextureFromSurface(m_renderer, gagneSurface);
		SDL_Rect gagneRect;
		gagneRect.x = (LONGUEUR_FENETRE - LONGUEUR_PLATEAU) / 2;
		gagneRect.y = scoreRect.y;
		gagneRect.w = gagneSurface->w;
		gagneRect.h = gagneSurface->h;
		SDL_RenderCopy(m_renderer, gagneTexture, NULL, &gagneRect);
		SDL_FreeSurface(gagneSurface);
		SDL_DestroyTexture(gagneTexture);
    }

    TTF_CloseFont(font);
}

void GUIApplication::dessineGameover()
{
    const int positionCaseX { (LONGUEUR_FENETRE - LONGUEUR_PLATEAU) / 2 };
    const int positionCaseY { (LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2 };

    SDL_Rect overlay { (LONGUEUR_FENETRE - LONGUEUR_PLATEAU) / 2, (LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2, LONGUEUR_PLATEAU, LONGUEUR_PLATEAU };
    SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 160);
    SDL_RenderFillRect(m_renderer, &overlay);

    TTF_Font* font { TTF_OpenFont("fonts/ClearSans-Bold.ttf", 35) };

    std::string message { "Game over !" };

    if (m_gagnant)
      message = "Partie terminée !";

    SDL_Surface* gameoverSurface = TTF_RenderUTF8_Blended(font, message.data(), m_couleurNoire);
    SDL_Texture* gameoverTexture = SDL_CreateTextureFromSurface(m_renderer, gameoverSurface);

	SDL_Surface* rejouerSurface = TTF_RenderUTF8_Blended(font, "Taper entrer pour rejouer.", m_couleurNoire);
	SDL_Texture* rejouerTexture = SDL_CreateTextureFromSurface(m_renderer, rejouerSurface);

	SDL_Rect gameoverRect;
	gameoverRect.x = positionCaseX + ((LONGUEUR_PLATEAU - gameoverSurface->w) / 2);
	gameoverRect.y = positionCaseY - (rejouerSurface->h / 2) + ((LONGUEUR_PLATEAU - gameoverSurface->h) / 2);
	gameoverRect.w = gameoverSurface->w;
	gameoverRect.h = gameoverSurface->h;

	SDL_Rect rejouerRect;
	rejouerRect.x = positionCaseX + ((LONGUEUR_PLATEAU - rejouerSurface->w) / 2);
	rejouerRect.y = positionCaseY + (gameoverSurface->h / 2) + ((LONGUEUR_PLATEAU - rejouerSurface->h) / 2);
	rejouerRect.w = rejouerSurface->w;
	rejouerRect.h = rejouerSurface->h;

	SDL_RenderCopy(m_renderer, gameoverTexture, NULL, &gameoverRect);
	SDL_FreeSurface(gameoverSurface);
	SDL_DestroyTexture(gameoverTexture);

	SDL_RenderCopy(m_renderer, rejouerTexture, NULL, &rejouerRect);
	SDL_FreeSurface(rejouerSurface);
	SDL_DestroyTexture(rejouerTexture);

    TTF_CloseFont(font);
}

void GUIApplication::dessinePlateau()
{
    // Dessin du rectangle
    SDL_Rect plateau { (LONGUEUR_FENETRE - LONGUEUR_PLATEAU) / 2, (LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2, LONGUEUR_PLATEAU, LONGUEUR_PLATEAU };
    SDL_SetRenderDrawColor(m_renderer, 0xcc, 0xcc, 0xcc, 0xff); //#bcaca0
    SDL_RenderFillRect(m_renderer, &plateau);
}

void GUIApplication::dessineCases()
{
    const int pointDepartX { (LONGUEUR_FENETRE - LONGUEUR_PLATEAU) / 2 + MARGE_PLATEAU };
    const int pointDepartY { (LARGEUR_FENETRE - LONGUEUR_PLATEAU) / 2 + MARGE_PLATEAU };

    const int longueurCase { (LONGUEUR_PLATEAU - 5 * MARGE_PLATEAU) / 4 };
    const int largeurCase  { (LONGUEUR_PLATEAU - 5 * MARGE_PLATEAU) / 4 };

    for (std::size_t i { 0 }; i != m_statsPlateau.plateau.size(); i++)
    {
      	for (std::size_t j { 0 }; j != m_statsPlateau.plateau[i].size(); j++)
        {
          	/* Choix de la couleur de la case et du texte, choix de la taille de la police. */

            /* Récupérer le plateau courant. */
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
			TTF_Font* font { nullptr };
			SDL_Color color { 0xff, 0xff, 0xff, 0xff };

            if (m_statsPlateau.plateau[i][j] == 0)
            {
            	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 15);
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 2)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
                color = { 0x77, 0x77, 0x77, 0xff };
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 4)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xed, 0xd9, 0xb4, 0xff); // #edd9b4
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 8)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xf2, 0xb1, 0x79, 0xff); // #f2b179
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 16)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xf5, 0x96, 0x63, 0xff); // #f59663
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 32)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xf6, 0x7c, 0x5f, 0xff); // #f67c5f
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 64)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xf6, 0x5e, 0x3b, 0xff); // #f65e3b
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 128)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xed, 0xcf, 0x72, 0xff); // #edcf72
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 256)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xee, 0xcc, 0x62, 0xff); // #eecc62
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 512)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xed, 0xc8, 0x51, 0xff); // #edc851
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 50);
            }
            else if (m_statsPlateau.plateau[i][j] == 1024)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xee, 0xc4, 0x3f, 0xff); // #eec43f
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 25);
            }
            else if (m_statsPlateau.plateau[i][j] == 2048)
            {
              	SDL_SetRenderDrawColor(m_renderer, 0xed, 0xc2, 0x2e, 0xff); // #edc22e
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 25);
            }
            else
            {
              	SDL_SetRenderDrawColor(m_renderer, 0x3c, 0x3a, 0x32, 0xff); // #3c3a32
                font = TTF_OpenFont("fonts/ClearSans-Bold.ttf", 25);
            }

            const int positionCaseX { pointDepartX + static_cast<int>(j) * (longueurCase + MARGE_PLATEAU) };
            const int positionCaseY { pointDepartY + static_cast<int>(i) * (longueurCase + MARGE_PLATEAU) };

			SDL_Rect casePlateau { positionCaseX, positionCaseY, longueurCase, largeurCase };
            SDL_RenderFillRect(m_renderer, &casePlateau);

            if (m_statsPlateau.plateau[i][j] != 0)
            {
            	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, std::to_string(m_statsPlateau.plateau[i][j]).data(), color);

            	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);

				SDL_Rect messageRect;
				messageRect.x = positionCaseX + ((longueurCase - surfaceMessage->w) / 2);
				messageRect.y = positionCaseY + ((largeurCase - surfaceMessage->h) / 2);
				messageRect.w = surfaceMessage->w;
				messageRect.h = surfaceMessage->h;

				SDL_RenderCopy(m_renderer, texture, NULL, &messageRect);

				SDL_FreeSurface(surfaceMessage);
		    	SDL_DestroyTexture(texture);
            }

            TTF_CloseFont(font);
        }
    }
}

void GUIApplication::dessineJeu()
{
    /* Dessine le mode de jeu. */
    dessineModeJeu();
    /* Dessine les règles du jeu. */
    dessineRègles();
    /* Dessine le plateau. */
    dessinePlateau();
    /* Dessine le score. */
    dessineScore();
    /* Dessine les cases. */
    dessineCases();
}

void GUIApplication::resetJeu()
{
	m_gagnant = false;
	m_terminé = false;
    m_statsPlateau.init();
}

int main()
{
    // Initialiser SDL_TTF.
    if(TTF_Init() < 0)
    {
      	std::cerr << "Could not initialise SDL_TTF: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Initialiser SDL.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cerr << "Could not initialise SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Gestion de l'application.
    GUIApplication app;
    app.afficheFenetre();

    // Quitter SDL et SDL_TTF.
    SDL_Quit();
    TTF_Quit();

    return 0;
}