---
jupytext:
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
kernelspec:
  display_name: Python 3 (ipykernel)
  language: python
  name: python3
---

# Rapport de projet 2048

## Auteur

- Leandri, Pierre, pierre.plndr@gmail.com, LDD-IM1

## Résumé du travail effectué

### Niveau 0

1. Le jeu est fonctionnel. Les fonctions de déplacements ont été réalisées
avec succès. Elles ont été beaucoup testées et leur implémentation est
bien solide.
2. Le score est bien mis à jour à chaque mouvement.
3. Chacune des fonctions dans le fichier modele.hpp sont bien spécifiées,
documentées et testées.
4. Lors de l’affichage, les colonnes sont bien alignées, quelles que
soient les valeurs de chaque tuile. Cette fonction n'est pas utilisée en
production dans l'interface graphique. Mais elle devait être implémentée
et testée.

### Niveau 1

1. La couleur de fond d'une case a été rajoutée sur l'interface graphique 
selon la valeur d'une tuile.
2. Les flèches haut, bas, gauche et droite du clavier sont utilisées pour
jouer au jeu 2048. **On ne doit pas appuyer sur entrée à chaque fois, mais
uniquement sur les flèches !**
3. L'affichage d’un plateau se fait bien à la place du plateau précédent.
De même pour le score.
4. Aucune variable globale n'a été utilisée. Une structure ``StatsPlateau``
a été mis en place pour englober le plateau et le score.

### Niveau 2

1. Le projet a un makefile qui compile 2 executables : le jeu et les tests.
2. Le GitLab n'a pas encore pu être testé car inaccessible en dehors
de l'établissement.

### Niveau 3 : extentions

1. Une interface graphique a été également implémentée grâce à SDL2.
2. Une variante du jeu a été implémentée. Il s'agit de 2048 Speed.
C'était la variante la moins compliquée a faire car les dimensions du
plateau et les règles initiales sont conservées. Il faut juste faire
attention d'avoir toujours une case libre car sinon vous avez perdu !

**Attention: comme je suis l'unique auteur du projet, je ne précise
pas l'auteur des fonctions rédigées dans le programme !** En effet, j'ai
bien écrit l'entièreté du code moi même.

## Compilation du projet

Pour compiler le projet, il faut télécharger la librarie SDL2 et SDL2_TTF via
un gestionnaire de paquets (e.g. apt, homebrew, ...). Ensuite, vous devrez
lier les fichiers headers (*.hpp, *.h) des deux librairies dans le dossier libs/include
du projet, typiquement avec un lien symbolique. De même pour les libraries,
mais cette fois ci dans le dossier libs/lib. Il vous suffit donc de remplacer ``...``
par le chemin dans lequel les paquets respectifs SDL2 et SDL2_TTF sont installés. 
Attention à bien dissocier les headers des lib !

```{code-cell} ipython3
ln -s ... libs/include/SDL2
ln -s ... libs/include/SDL2_ttf

ln -s ... libs/lib/SDL2_ttf
ln -s ... libs/lib/SDL2_ttf
```

Il vous suffit ensuite d'executer la commande suivante pour compiler le
projet entier :

```{code-cell} ipython3
make
```

Vous pourrez alors lancer le jeu depuis un terminal avec :
```
./2048
```
Mais aussi lancer les tests depuis un terminal grâce à :
```
./gametests
```

## Organisation du travail

J'ai préféré travaillé seul pour le premier projet de licence.
C'est d'abord un avantage car l'apprentissage est plus poussé, alors que si
j'avais travaillé avec un binôme il le serait moins. Mais c'est aussi un
inconvénient ! En effet, cela requiert beaucoup plus de travail. Je n'ai pas
vraiment  compté le nombre d'heures que j'ai pu passer sur le projet mais je pense
être à plus d'environ 20 heures sans problèmes !

Aussi, réfléchir avant que le projet ne soit téléchargeable est bénéfique !
J'ai pensé directement à utiliser SDL et pas à faire l'interface
uniquement sur console.

## Prise de recul

### Ce que j'ai appris

1. Implémenter un 2048 et une de ses variantes.
2. Intéragir correctement avec SDL.
3. Faire un makefile.
4. Initiation à la programmation orientée objet (S2).
5. Initiation à la notion de référence et de pointeur (S2). 
6. Compréhension **simple** de thread avec ``SDL_AddTimer``
(pour la variante de 2048 speed).
7. Bien exploiter une documentation d'une bibliothèque.
8. Se familiariser avec les bonnes pratiques du C++.

### Difficultés rencontrées

1. Installer une librairie convenablement.
2. Faire une interface graphique avec SDL. C'était long ! Peut-être
utiliser GTK+ la prochaine fois ? 
3. Multithreading : le callback de ``SDL_AddTimer`` est appelé dans un
autre thread que celui du thread principal !