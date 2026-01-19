from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import sys

def dessiner_rectangle(img, x_min, x_max, y_min, y_max):
    couleur = [0, 0, 0]

    for x in range(x_min, x_max + 1):
        img[y_min, x] = couleur
        img[y_max, x] = couleur

    for y in range(y_min, y_max + 1):
        img[y, x_min] = couleur
        img[y, x_max] = couleur


# main
img = np.array(Image.open(sys.argv[1]).convert("RGB"))

with open(sys.argv[2]) as f:
    for line in f:
        x_min, x_max, y_min, y_max = map(int, line.split())
        dessiner_rectangle(img, x_min, x_max, y_min, y_max)

plt.imshow(img)
plt.axis("off")
plt.savefig("resultat.jpg")
