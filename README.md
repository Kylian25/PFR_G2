# PFR1 G2

Le projet a été développé exclusivement pour un environnement Unix/Linux

## Prérequis et installation 
#### Cloner le projet : 

```
git clone https://github.com/Kylian25/PFR_G2
```
#### Dépendances : 


- Langage C : un compilateur comme gcc

- Python3 : pour la partie simulation et transcription vocale

- Shell unix : pour l'orchestration des modules


#### Bibliothèques Python utilisées : 

- Turtle (simulation)
- Matplotlib (traitement d'images)
- numpy
- PIL (simulation + traitement d'images)
- speech_recognition (transcription)
- gTTS (transcription)

## Compilation 


Se placer dans le répertoire principal :
```
cd PFR_G2
```

Le projet comprte un Makefile, pour compiler le projet il faut exécuter la commande suivante : 
```
make
```

## Fonctionnement

Configuration et logs : Au démarrage, le programme charge les paramètres (langue, distances par défaut) depuis un fichier texte. Il est possible de modifier les paramètres en modifiant directement ceux ci dans le fichier configuration/parametres lorsque le programme n'est pas en fonctionnement ou lorsque le mode administrateur est actif.

Interface : un menu bilingue (FR/EN) permet de choisir entre le mode Utilisateur et le mode Administrateur.

Traitement des requêtes : les commandes vocales ou textuelles sont traduites en instructions compréhensibles par le robot.

Simulation : le robot exécute les commandes dans une fenêtre graphique via le script Python utilisant Turtle.

## Utilisation
Pour lancer le programme, exécuter la commande suivante dans le terminal 
```
./main
```
#### Points importants : 
- Dans le mode utilisateur, lorsque le nom de l'image est demandé il faut simplement donné le nom d'une image située dans le répertoire donnees sans écrire l'extension.
- Pour énoncer les différents paramètres d’une commande, ne pas les séparer par “puis”, mais simplement par des espaces
- Pour écrire ou dicter une suite d'instructions il faut utiliser "puis" entre deux commandes, sinon l'instruction suivante ne sera interprétée.

#### Pour le mode administrateur, le mot de passe par défaut est 123 (modifiable dans le code)
  
