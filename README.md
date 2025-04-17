# B12 - TECHNICAL TEST - Laurelenn Sangaré

## Généralités :

### Intention :

Pour réaliser ce test, je me suis projetée dans l’optique suivante : _**“si les GD me présentaient ce jeu, quelles features et outils je développerais pour leur faciliter la vie ?”**_

Je me suis donc **principalement concentrée sur les outils et l’architecture générale, bien que simplifiée**, pour donner une idée de mes habitudes de travail et ma manière d’aborder la chose. Bien sûr, il est toujours possible de pousser et optimiser les outils en fonction du contexte !

### Temps de travail :

Un peu plus de 28h (~ 4j de travail)

‌

### Contrôles & Executable :

- La build est dans le dossier _Build_ !
- **Clavier** :
  - Déplacer : ZQSD
  - Sauter : Espace
  - Interagir (prendre/poser) : E
  - Caméra : Souris
- **Manette** :
  - Déplacer : Joystick gauche
  - Sauter : Bouton Bas (A/ Croix)
  - Interagir (prendre/poser) : bouton gauche (X/Carré)
  - Caméra : Joystick droit

---

---

## **Architecture globale et features :**

### **GameCaptureManager :**

#### Logique :

Singleton qui gère le jeu en général (game state, load des maps, events…), est un subsystem auquel chaque acteur/objet peuvent s’abonner. Load les maps GD et Arts donnés en paramètres dans la DA (présente dans le GameMode) afin que les GD puissent facilement tester des LD différents.

#### Approfondissement possibles :

- Pour un jeu de plus grande envergure, pourrait être un objet spawned dans la scène (si on a plusieurs mini-jeux par exemple)
- Après réflexion faire un enum bitflag pour les conditions de fin de jeu aurait été + pratique (Arena, Time, LifePlayer…)

### **ArenaManager :**

#### Logique :

Système d’arena permettant de créer des vagues de spawns d’objets aléatoirement sur le navmesh  et/ou des levels si on veut des placements plus précis.

#### Approfondissement possible :

- Seules 2 conditions de fin de wave et 1 condition de fin d’arena sont disponibles (timer et/ou cubes de la vague précédente capturés) mais c’est justement un contenu à enrichir en accord avec les perspectives des GD.
- J’ai opté pour un système “contrôlable” par les GDs, mais on peut envisager un système de vague infini avec un pourcentage de spawn en fonction de la classe souhaitée. On pourrait aussi ajouter une option de zone de collision définie pour établir la zone de spawn.

‌

### **CaptureZone :**

#### Logique :

Zone de capture des AIPreys (cubes). Les classes d’acteurs qui trigger cette zone (par collision) sont personnalisables, peut servir aux GD pour créer différentes zones de captures en fonction de la classe d’objet qu’on souhaite par exemple, ou ajouter tous les enfants d’une classe mère.

#### Approfondissement possible :

- Avoir une zone de collision personnalisable (sphere, custom ou box), à activer en fonction d’un enum donné en paramètre. _(Non réalisé pour des questions de simplicité et de temps)_

### **LifeComponent :**

#### Logique :

API globale permettant de gérer “la vie” d’un élément. Peut-être décliné pour être utilisé sur une IA, élément gameplay (bouclier, mur…), une jauge etc…

#### Approfondissement possible :

- On peut enrichir avec d’autres fonctions telle que la résurrection avec vie prédéfinie etc…

‌

### **HoldableComponent :**

#### Logique :

Component qui peut se placer facilement et rapidement sur n’importe quel acteur afin que le joueur puisse le prendre dans sa main. Descend d’une classe “InteractionComponent” qui pourrait avoir autant de classes filles que de comportements “d’interaction” communs différents (ouvrir porte, ouvrir coffre, observer de près, parler à PNJ etc…)

#### Approfondissement possible :

- Custom la socket à laquelle s’attache l’objet bien sûr !

‌

### **ProjectileLauncherComponent & Projectile :**

#### Logique :

Component autonome qui peut être placé sur n’importe quel acteur afin de tirer des projectiles (de la classe Projectile). A un système de “nombre de tir par round/salve” qui permet de varier les cadences de tir.

#### Approfondissement possible :

- Seule une version “ballistique” a été créée, mais déclinable en version “laser continu” par exemple !
- Projectile : Différences possibilités de collisions et sélection de visuels (mesh, vfx).  _(Non réalisé pour des questions de simplicité et de temps)_

‌

### **AIBase :**

#### Logique :

Classe mère de AIPrey (cubes) et AIEnemy (cones). Les informations importantes sont stockées dans des Datas Assets afin que les GD puissent facilement tweaker les paramètres et les appliquer sur toutes les IA concernées. Sont gérées par un BT.

#### Approfondissement possible :

- Peut customiser encore plus la DA afin qu’on n’ait même plus besoin de créer des Actor différents pour les déclinaisons d’une même IA. (En customisant la size, le mesh, le material etc…). Permettrait aux GD de changer très facilement le contenu d’une map sans avoir à remplacer les IA une par une.
- Utilisation des task un peu maladroite, en général une task était créée pour une IA particulière, mais on aurait pu factoriser davantage certaines d’entre elles (Move On NavMesh, Player Detection…)

‌

### **Navmesh :**

#### Logique :

Des recast différents pour les AIPreys et les AIEnemies (les preys n’ont pas accès à toute la zone de jeu, tandis que les Enemies oui)

#### Approfondissement possible :

- Plutôt que de limiter la zone de navigation, on aurait pu utiliser un navmodifier sur la zone de capture notamment, afin que le navmesh s’adapte en fonction de son apparence/collision (plutôt qu’une box)

‌

### **Généralités :**

#### Logique :

- **Data Assets :**  ont été crées afin de tweaker facilement certains éléments (les paramètres du jeu du GameManager notamment, les paramètres des IA…). Permets aux GD de garder une trace de leurs tweak, de tester facilement différents paramétrages, de modifier facilement une map, d’unifier les données, de créer des niveaux….
- **Git.ignore :** Ne pas ignorer les .dll permet aux GD et Art de ne pas avoir à recompiler le projet lors de petites modifications.

#### Approfondissement possible :

- **Data Assets :** Dans une version plus aboutie, on peut imaginer créer des DA pour tout (projectile launcher, projectile…), ce qui rendrait le tout plus modulaire, mais peut être + fatiguant pour les GDs. J’ajoute que tout créer dans des “structs” permet aux GD de copier-coller facilement les données d’une DA à l’autre si besoin.
- **SoundManager :** Un SoundManager pourrait être créé, il s’abonnerait aux events liés aux states et au gameplay pour jouer les sons en rapport avec le gameplay principal (musique qui change, son lors d’une capture…)
- **Feedback :** Peu de VFX et de feedback sonores par manque de ressources et de temps

---

## Difficultés rencontrées :

‌

- **Navmesh & AI** : Ma plus grande difficulté a été sur la gestion de mouvement des IA et du Navmesh, n’ayant pas pu expérimenter ce type de features lors de mes précédentes expériences (rail shooter, arène avec placement défini).
- Je note aussi une maladresse certaine dans l’architecture des Task IA créés, qui auraient pu être plus modulaires et factorisables.
