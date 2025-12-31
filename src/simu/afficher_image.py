import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import sys

H, L = 300, 300  

# sys.argv[1] -> fichier texte (image filtrée)
# sys.argv[2] -> fichier jpeg (image originale)
fichier_filtre = sys.argv[1]
fichier_original = sys.argv[2]


with open(fichier_filtre, "r") as f:    # lire le fichier
    values = list(map(int, f.read().split()))

img_filtree = np.array(values).reshape(H, L, 3)


img_originale = mpimg.imread(fichier_original)  # lire l'image originale


fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6)) # Afficher les deux images côte à cote

ax1.imshow(img_originale)
ax1.set_title('Image originale')
ax1.axis('off')

ax2.imshow(img_filtree)
ax2.set_title('Image filtrée')
ax2.axis('off')

plt.tight_layout()
plt.show()