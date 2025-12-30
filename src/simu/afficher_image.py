import matplotlib.pyplot as plt
import numpy as np

import sys

H, L = 300, 300  
# sys.argv[1] -> premier argument (ici le fichier)
fichier = sys.argv[1]

# Lire le fichier texte
with open(fichier, "r") as f:
    values = list(map(int, f.read().split()))

img = np.array(values).reshape(H, L, 3)

plt.imshow(img)
plt.axis('off')
plt.show()
