import sys 
import turtle as tl
from PIL import Image

#----------------------------- Fonctions -----------------------------------
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
                
                elem = ligne.split()      # découpe la ligne, espaces comme séparateurs
                
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
     
def message_console(FR,EN):
    if config["langue"] == "FR":
        print(FR)
    else: print(EN)

"""
Récupère les instructions dans le fichier texte et retourne une liste de celles ci,
si le fichier est vide, retourne un message d'erreur.
"""
def recup_instructions(nom_fichier : str):
    instructions = []
    fichier = "temp/"+nom_fichier

    try:
        with open(fichier,"r") as fichier:
            for ligne in fichier:
                ligne=ligne.strip()  # ignore les lignes vides 
                if not ligne:    # si lignes vides
                    message_console("Aucune instruction dans le fichier","Instructions file is empty")
                    continue
                
                elem = ligne.split()

                if len(elem)>1:
                    for i in range(len(elem)):
                        instructions.append(elem[i])
                    message_console("Liste d'instructions : ", "Instructions list : ")
                    print(instructions)
                    return instructions
                else:
                    message_console("Aucune instruction dans le fichier", "Instructions file is empty")

    except FileNotFoundError:
        message_console("Pas de fichier","No file")
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

def aller_a(x,y):
    x,y = coord_image_turtle(x,y)
    tl.goto(x,y)

def avancer(distance):
    tl.forward(distance)

def revenir(x,y):

    tl.goto(x,y)

def tourner(angle):
    tl.right(angle)

def demi_tour():
    tl.right(180)

def dessiner_obstacle(forme : str,couleur : str ,x_HG : int,y_HG : int,x_BD : int,y_BD : int):
    x_robot_img,y_robot_img = coord_turtle_image(tl.pos()[0],tl.pos()[1])

    x_centre_obstacle = (x_HG + x_BD) / 2
    y_centre_obstacle = (y_HG + y_BD) / 2
    
    largeur_obstacle = abs(x_BD - x_HG)
    hauteur_obstacle = abs(y_BD - y_HG)

    if forme == "cube":
        #dessin
        tl.up()
        aller_a(x_HG,y_HG)

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

        tl.up()

        aller_a(x_robot_img,y_robot_img)
    elif forme == "balle": 

        rayon = min(largeur_obstacle,hauteur_obstacle) /2

        tl.up()
        aller_a(x_centre_obstacle,y_centre_obstacle+rayon)

        tl.fillcolor(couleur)
        tl.begin_fill()
        tl.down()
        tl.setheading(0)
        tl.circle(rayon)
        tl.end_fill()

        tl.up()
        aller_a(x_robot_img,y_robot_img)
    else: message_console("Cette forme n'est pas reconnue", "This form is not recognized")

def eviter_obstacle(x_HG,y_HG,x_BD,y_BD):    

    x_robot,y_robot = coord_turtle_image(tl.pos()[0],tl.pos()[1])

    x_centre_obstacle = (x_HG + x_BD) / 2
    y_centre_obstacle = (y_HG + y_BD) / 2
    
    largeur_obstacle = abs(x_BD - x_HG)
    hauteur_obstacle = abs(y_BD - y_HG)
    
    
    if y_BD + 30 < hauteur:   # verifier si on peut aller en dessous
        tl.up()
        delta = 30
        if est_dans_image(x_centre_obstacle,y_BD + delta):
            aller_a(x_centre_obstacle, y_BD + delta)
            tl.setheading(90)  # Orientation vers le haut
            tl.down()
            
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
        if est_dans_image(x_HG-delta, y_centre_obstacle):      #contournement par le coté
            tl.up()
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
        


#----------------------------- Initialisation --------------------------------

chemin_config = "configuration/parametres"
config = charger_config(chemin_config)


image = sys.argv[1]
image = Image.open("donnees/" + image)
image.save("temp/simu.gif", format="GIF")

largeur, hauteur = image.size
x_start_tl = 0
y_start_tl = - hauteur/2

screen = tl.Screen()
screen.setup(width=largeur, height=hauteur)

tl.bgpic("temp/simu.gif")      # image de fond



#----------------------------- Programme principal --------------------------------

print("")
tl.speed(1)
initialisation(x_start_tl,y_start_tl)
tl.color("green")
#dessiner_obstacle(60,200,160,300,"blue","rond")
#eviter_obstacle(60,200,160,300)
#dessiner_obstacle(30,30,70,55,"yellow", "carre")
#eviter_obstacle(30,30,70,55)

commandes = recup_instructions("instructions.txt")
objet = recup_instructions("objet.txt")
if len(objet) == 6:
    dessiner_obstacle(objet[0],objet[1],int(objet[2]),int(objet[3]),int(objet[4]),int(objet[5]))
    eviter_obstacle(int(objet[2]),int(objet[3]),int(objet[4]),int(objet[5]))

print("objet : ", objet)
print("commandes : ", commandes)

tl.done()