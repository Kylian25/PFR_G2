from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import sys

def dessiner_rectangle(img, x_min, x_max, y_min, y_max):
    couleur = [0, 0, 0]
    delta=5

    for x in range(x_min-delta,x_max+delta):
        img[y_min-delta][x]=couleur
        img[y_max+delta][x]=couleur
    for y in range(y_min-delta,y_max+delta):
        img[y][x_min-delta]=couleur
        img[y][x_max+delta]=couleur

# main
img = np.array(Image.open(sys.argv[1]).convert("RGB"))

with open(sys.argv[2]) as f:
    for line in f:
        x_min, x_max, y_min, y_max = map(int, line.split())
        dessiner_rectangle(img, x_min, x_max, y_min, y_max)

plt.imshow(img)
plt.axis("off")
plt.savefig("resultat.jpg")
