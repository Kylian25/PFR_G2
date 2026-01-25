import turtle as tl
from PIL import Image
import numpy as np

# valeurs max/min

MAX_DIST_DEP = 300
MIN_DIST_DEP = 1
MAX_TPS_DEP = 20
MIN_TPS_DEP = 1
MAX_DIST_DET = 50
MIN_DIST_DET = 1
MAX_ANGLE = 360
MIN_ANGLE = 1

COULEURS = {

    "rouge" : "red",
    "bleu" : "blue",
    "jaune" : "yellow"
}


#----------------------------- Initialisation --------------------------------

image = Image.open("temp/resultat.jpg")
image.save("temp/simu.gif", format="GIF")

largeur, hauteur = image.size
x_start_tl = 0
y_start_tl = - hauteur/2

screen = tl.Screen()
screen.setup(width=largeur+200, height=hauteur+200)

tl.bgpic("temp/simu.gif")      # image de fond

#----------------------------- Config -----------------------------------
"""
Charger la configuration depuis le fichier parametres.txt
afin d'accéder aux valeurs par défaut 
"""
def charger_config(fichier):     
                      
    config = {}
    try:
        with open(fichier, "r") as f:
            for ligne in f:
                ligne = ligne.strip()        # ignore les lignes vides... 
                if not ligne:
                    continue
                
                elem = ligne.split()         # découpe la ligne, espaces comme séparateurs
                
                if len(elem) == 2:
                    cle = elem[0]
                    valeur = elem[1]
                    
                    try:                            #convertir les str en entier si possible
                        config[cle] = int(valeur)    
                    except ValueError:
                        config[cle] = valeur
        return config
    except FileNotFoundError:
        print("pas de fichier")
        return {}
    
chemin_config = "configuration/parametres"
config = charger_config(chemin_config)

#----------------------------- Fonctions -----------------------------------
def ecrire(message : str,couleur : str):
    tl.speed(100)
    tl.hideturtle()
    x,y = coord_image_turtle(5, hauteur+30)
    tl.pencolor(couleur)
    tl.up()
    tl.goto(x,y)
    tl.down()
    tl.write(message, font=("Arial",14, "bold"))
    tl.speed(1)


def message_console(FR,EN):
    if config["langue"] == "FR":
        print(FR)
    else: print(EN)

"""
Récupère les instructions dans le fichier texte et retourne une liste de celles ci,
si le fichier est vide, retourne un message d'erreur.
"""
def recup_infos(nom_fichier: str):
    infos = []
    chemin = "temp/" + nom_fichier

    try:
        with open(chemin, "r") as f:
            for ligne in f:
                ligne = ligne.strip()
                if not ligne:
                    continue            # ignore les lignes vides

                elem = ligne.split()
                infos.extend(elem)

        if not infos:
            message_console(
                "Aucune instruction dans le fichier",
                "Instructions file is empty"
            )

        return infos

    except FileNotFoundError:
        message_console("Pas de fichier", "No file")
        return []

def initialisation(x,y):
    tl.up()
    tl.goto(x,y)
    tl.setheading(90)                                 # orientation vers le haut 
    tl.down()
    tl.width(3)

def coord_image_turtle(x_image,y_image):              # convertion des coordonnées images à turtle (centre en 0,0)
    x = x_image - largeur/2
    y = hauteur/2 - y_image
    return x,y

def coord_turtle_image(x_turtle, y_turtle):              # convertion des coordonnées turtle à image
    x_image = x_turtle + largeur/2
    y_image = hauteur/2 - y_turtle
    return x_image, y_image

def est_dans_image(x_image,y_image):                  #vérifie si le robot est bien dans l'image
    if 0 <= x_image <= largeur and 0 <= y_image <= hauteur:
        return True
    else: return False

def retour_image(x_image,y_image):       
    x = max(0, min(x_image,largeur))                   #si x < 0 => revient en 0, sinon au x correspondant dans l'image si x <= largeur
    y = max(0 , min(y_image,hauteur))

#-------------------------- Fonctions de déplacement ------------------------------

def aller_a(x : int=x_start_tl,y : int =y_start_tl):
    if est_dans_image(x,y):
        x,y = coord_image_turtle(x,y)
        tl.goto(x,y)
    else:
        message_console("Coordonnées en dehors de l'image","Coordinates out of image")

def avancer(distance: int =30 ):
    if MIN_DIST_DEP <= distance <= MAX_DIST_DEP:
        tl.forward(distance)
    else:
        message_console(f"Distance de déplacement hors des limites [{MIN_DIST_DEP},{MAX_DIST_DEP}]",
                        f"distance is out of limits [{MIN_DIST_DEP},{MAX_DIST_DEP}]")
        
def revenir():
    
    tl.goto(x_start_tl,y_start_tl)
   
def tourner(angle : int = config["angle"]):
    if MIN_ANGLE <= angle <= MAX_ANGLE:
        tl.right(angle)
    else: message_console(f"Angle en dehors des limites [{MIN_ANGLE},{MAX_ANGLE}]",f"Angle out of limits [{MIN_ANGLE},{MAX_ANGLE}]")

def demi_tour():
    tl.right(180)

def dessiner_obstacle(forme : str,couleur : str ,x_HG : int,y_HG : int,x_BD : int,y_BD : int):
    x_robot_img,y_robot_img = coord_turtle_image(tl.pos()[0],tl.pos()[1])

    x_centre_obstacle = (x_HG + x_BD) / 2
    y_centre_obstacle = (y_HG + y_BD) / 2
    
    largeur_obstacle = abs(x_BD - x_HG)
    hauteur_obstacle = abs(y_BD - y_HG)

    couleur=COULEURS[couleur]

    if forme == "cube":
        #dessin
        tl.up()
        aller_a(x_HG,y_HG)

        tl.color(couleur)
        tl.fillcolor(couleur)
        tl.begin_fill()
        tl.setheading(1)
        tl.down()
        tl.forward(largeur_obstacle)
        tl.right(90)
        tl.forward(hauteur_obstacle)
        tl.right(90)
        tl.forward(largeur_obstacle)
        tl.right(90)
        tl.forward(hauteur_obstacle)
        tl.end_fill()
        tl.color("green")

        tl.up()

        aller_a(x_robot_img,y_robot_img)
    elif forme == "balle": 

        rayon = min(largeur_obstacle,hauteur_obstacle) /2

        tl.up()
        aller_a(x_centre_obstacle,y_centre_obstacle+rayon)
        tl.color(couleur)
        tl.fillcolor(couleur)
        tl.begin_fill()
        tl.down()
        tl.setheading(0)
        tl.circle(rayon)
        tl.end_fill()
        tl.color("green")

        tl.up()
        aller_a(x_robot_img,y_robot_img)
        tl.down()
    else: message_console("Cette forme n'est pas reconnue", "This shape is not recognized")

def eviter_obstacle(forme : str = "balle" , couleur : str = "bleu"): 

    formes = recup_infos("forme_couleur.txt")
    coords = []
    for i in range(len(formes)):
        if formes[i] == forme and formes[i+1] == couleur:
            indice_objet = i
            message_console(f"L'image contient bien un objet de type {forme} et de couleur {couleur}",
                            f"The image does contain an object of type {forme} and color {couleur}.")
            for k in range(i+2,i+6):
                coords.append(int(formes[k]))
            #dessiner_obstacle(forme, couleur,coords[0],coords[2], coords[1],coords[3])   

            x_HG = coords[0]
            x_BD = coords[1]
            y_HG = coords[2]
            y_BD = coords[3] 

            x_robot,y_robot = coord_turtle_image(tl.pos()[0],tl.pos()[1])

            x_centre_obstacle = (x_HG + x_BD) / 2
            y_centre_obstacle = (y_HG + y_BD) / 2
            
            largeur_obstacle = abs(x_BD - x_HG)
            hauteur_obstacle = abs(y_BD - y_HG)
            
            
            if y_BD + 30 < hauteur:   # verifier si on peut aller en dessous
                tl.up()
                delta = 30
                if est_dans_image(x_centre_obstacle,y_BD + delta):
                    tl.up()
                    revenir()
                    tl.down()
                    aller_a(x_centre_obstacle, y_BD + delta)
                    tl.setheading(90)  # Orientation vers le haut
                    
                    # Contournement
                    tl.forward(10)
                    tl.right(90)
                    tl.forward(largeur_obstacle/2 + delta)
                    tl.left(90)
                    tl.forward(hauteur_obstacle + delta)    
                    tl.left(90)
                    tl.forward(largeur_obstacle/2 + delta)
                    tl.right(90)
                else: message_console("Impossible de contourner l'obstacle par le bas", "impossible to go around the obstacle from below")
            else:  
                if est_dans_image(x_HG - delta , y_centre_obstacle):      #contournement par le coté
                    tl.up()
                    revenir()
                    tl.down()
                    aller_a(x_HG - delta, y_centre_obstacle)
                    tl.setheading(0)  
                    tl.down()
                    
                    tl.forward(10)
                    tl.left(90)
                    tl.forward(hauteur_obstacle/2 + delta)
                    tl.right(90)
                    tl.forward(largeur_obstacle + delta)
                    tl.right(90)
                    tl.forward(hauteur_obstacle/2 + delta)
                    tl.left(90)
                else: message_console("Impossible de contourner l'obstacle","impossible to go around the obstacle" )
   
def zigzag(distance : int = config["dist_dep"]):
    tl.setheading(90)
    x_max = int(distance * np.cos((np.pi)/4))
    y_max = int(tl.pos()[1] + 2 * distance * np.sin((np.pi)/4))

    while x_max < largeur and y_max < coord_image_turtle(0,0)[1]:

        tl.right(45)
        tl.forward(distance)
        tl.left(90)
        tl.forward(distance)
        tl.right(45)
        y_max += int(2 * distance * np.sin((np.pi)/4))
    
def chercher_objet(forme : str = "balle", couleur : str = "rouge"):

    #instructions = recup_infos("instructions.txt")
    formes = recup_infos("forme_couleur.txt")
    coords = []
    for i in range(len(formes)):
        if formes[i] == forme and formes[i+1] == couleur:
            indice_objet = i
            message_console(f"L'image contient bien un objet de type {forme} et de couleur {couleur}",
                            f"The image does contain an object of type {forme} and color {couleur}.")
            for k in range(i+2,i+6):
                coords.append(int(formes[k]))
            dessiner_obstacle(forme, couleur,coords[0],coords[2], coords[1],coords[3])
    revenir()

def reculer(distance : int = config["dist_dep"]):
    demi_tour()
    avancer(distance)

def tracer(forme : str = "cercle", rayon : int = 20, couleur : str = "jaune"):
    x, y = tl.pos()
    tl.color(COULEURS[couleur])
    if forme == "cercle":
        avancer(5)
        tl.setheading(0)
        if -150 <= x <= 150 and -150 <= x + (2*rayon) <= 150 and -150 <= y <= 150 and -150 <= y + (2*rayon) <= 150:
            tl.circle(rayon)
        else:
           message_console("impossible de tracer le cercle à ces coordonnées",
                           "Impossible to draw a circle at these coordinates")
    elif forme == "carré":
        #avancer(5)
        tl.setheading(0)
        if -150 <= x + rayon <= 150 and -150 <= x <= 150 and -150 <= y + rayon <= 150 and -150 <= y <= 150:
            for i in range(4):
                tl.forward(rayon)
                tl.left(90)
        else:
            message_console("impossible de tracer le carré à ces coordonnées",
                            "Impossible to draw a square at these coordinates")

def find ( couleur : str = "rouge",forme : str = "balle"):
    
    #instructions = recup_infos("instructions.txt")
    formes = recup_infos("forme_couleur.txt")
    coords = []
    for i in range(len(formes)):
        if formes[i] == forme and formes[i+1] == couleur:
            indice_objet = i
            message_console(f"L'image contient bien un objet de type {forme} et de couleur {couleur}",
                            f"The image does contain an object of type {forme} and color {couleur}.")
            for k in range(i+2,i+6):
                coords.append(int(formes[k]))
            dessiner_obstacle(forme, couleur,coords[0],coords[2], coords[1],coords[3])
    revenir()

def simulation():
    instructions = recup_infos("instructions.txt")
    if len(instructions)==0:
        ecrire("Aucune instruction !","red")
    objets = recup_infos("forme_couleur.txt")
    nb_commandes = 0

    for i in range(len(instructions)):
        param = []
        if instructions[i] in COMMANDES.keys():
            nb_parametres = COMMANDES[instructions[i]][1]
            commande = COMMANDES[instructions[i]]

            if nb_parametres>0:
                for k in range(i+1,i+1+nb_parametres):

                    if k<len(instructions):

                        if instructions[k].isdigit():
                            param.append(int(instructions[k]))
                        elif commande[2] != "int":
                            param.append(instructions[k])

                    else: break
                commande[0](*param)
                nb_commandes +=1
                #print(f"Commande exécutée : {commande[0]}")
                
            else:
                commande[0]()
                #print(f"Commande exécutée : {commande[0]}")
    if nb_commandes > 1:
        ecrire("Simulation terminée !", "green")

#----------- Commandes/fonctions --------------
COMMANDES = {              # commande : [fonction, nombre d'arguments,""] "int" si seul parametre = entier
    
    "zigzag" : (zigzag,1,""),
    "tourne" : (tourner,1,"int"),
    "revenir" : (revenir,0,""),
    "demi_tour" : (demi_tour,0,""),
    "avance" : (avancer,1,"int"),
    "trouve" : (chercher_objet,2,""),
    "aller" : (aller_a,2,""),
    "eviter" : (eviter_obstacle, 2, ""),
    "recule" : (reculer,1,"int"),
    "tracer" : (tracer,3,""),
    "find" : (find,2,"")
}

#----------------------------- Programme principal --------------------------------

print("")
tl.speed(1)
initialisation(x_start_tl,y_start_tl)
tl.color("green")

print("config : ", config,"\n")
commandes = recup_infos("instructions.txt")

print("commandes : ", commandes)
objets = recup_infos("forme_couleur.txt")
print("\nobjets : ", objets)

simulation()

tl.done()