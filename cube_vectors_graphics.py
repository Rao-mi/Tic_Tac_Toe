import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np
from itertools import product, combinations

# Leggi il file CSV specificando dtype come int
file_path = 'vectors.csv'
with open(file_path, 'r') as file:
    # Converte i dati del file in una lista di vettori
    vectors = [tuple(map(int, line.strip().split(','))) for line in file]

# Calcola il centro medio dei vettori
center = np.mean(vectors, axis=0)

def plot_scaled_vectors_and_blue_cube(vectors, scale_factor=0.05, cube_size=0.1):
    fig = plt.figure(figsize=(6, 6))
    ax = fig.add_subplot(111, projection='3d')

    # Visualizza i vettori come frecce con origine al centro
    for vector in vectors:
        scaled_vector = np.array(vector) * scale_factor
        ax.quiver(0, 0, 0, scaled_vector[0], scaled_vector[1], scaled_vector[2],
                  color='r', alpha=0.7, arrow_length_ratio=0.1, pivot='tail')

    # Definisci i vertici del cubo
    cube_vertices = np.array(list(product([-cube_size, cube_size], repeat=3)))

    # Costruisci il cubo con colorazione blu e scala
    scaled_cube_vertices = cube_vertices * cube_size
    cube_faces = list(combinations(scaled_cube_vertices, 4))
    ax.add_collection3d(Poly3DCollection(cube_faces, color="b", alpha=0.5, edgecolor="b"))

    # Nascondi assi
    ax.set_axis_off()

    plt.tight_layout()
    plt.show()

# Visualizza i vettori in rosso e un cubo blu nello stesso grafico
plot_scaled_vectors_and_blue_cube(vectors)
