import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression

# Leggi il file CSV
file_path = '4x4_datas_bin_0.01_games_100.csv'
df = pd.read_csv(file_path)

# Estrai i valori delle colonne 'bias' e ' X'
x_values = df['bias'].values.reshape(-1, 1)
y_values = df[' X'].values

# Estrai i valori di "grid", "bin", e "number of games" dal nome del file
file_name = file_path.split('/')[-1]  # Estrai solo il nome del file dalla path
grid, _, _, bin_value, _, num_games = file_name.replace('.csv', '').split('_')

# Chiedi all'utente il grado del fit polinomiale
degree = int(input("Inserisci il grado del fit polinomiale desiderato: "))

# Trasforma le features in polinomi del grado specificato
poly = PolynomialFeatures(degree)
x_poly = poly.fit_transform(x_values)

# Crea il modello di regressione lineare
model = LinearRegression()

# Addestra il modello
model.fit(x_poly, y_values)

# Effettua le predizioni
y_pred = model.predict(x_poly)

# Trova il minimo della curva
min_index = np.argmin(y_pred)
min_x = x_values[min_index]
min_y = y_values[min_index]

# Trova gli indici dove x >= 0.4
indices_greater_than_04 = np.where(x_values >= 0.4)[0]

# Calcola la media dei dati con x >= 0.4
mean_y_greater_than_04 = np.mean(y_values[indices_greater_than_04])


# Crea il grafico
plt.scatter(x_values, y_values, marker='o', color='blue', label='Dati originali', s=10)
plt.plot(x_values, y_pred, color='red', linewidth=2, label='Fit polinomiale')
plt.scatter(min_x, min_y, color='green', marker='o', label=f'Minimo: {min_y} at {min_x}')

# Aggiungi la retta nera parallela all'asse x
#plt.axhline(y=mean_y_greater_than_04, color='black', linestyle='--', label=f'Media con x >= 0.4: {mean_y_greater_than_04:.2f}')

# Personalizza il grafico
plt.title(f'Polynomial Fit of Degree {degree} - Grid: {grid}, Bin: {bin_value}, Games: {num_games}')
plt.xlabel('Values of b (bias)')
plt.ylabel('Games Lost')
plt.legend(loc='upper right')
plt.grid(True)
# Visualizza il grafico
plt.show()
