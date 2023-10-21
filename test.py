'''import networkx as nx
import matplotlib.pyplot as plt

# Inizializza un grafo vuoto
G = nx.Graph()

# Apre il file CSV
with open('TTT_3^2.csv', 'r') as file:
    # Inizializza una lista di stati all'interno di una mossa
    states_in_move = []
    previous_state = None  # Inizializza lo stato precedente a None
    for line in file:
        if line.startswith('-----------------------------'):
            # Hai raggiunto la fine di una mossa, procedi con la gestione delle mosse
            # Aggiungi nodi e archi per gli stati in questa mossa
            for state in states_in_move:
                G.add_node(state)

                # Collega il nodo corrente al nodo della mossa precedente (se esiste)
                if previous_state is not None:
                    G.add_edge(state, previous_state[])

            # Svuota la lista degli stati all'interno della mossa
            states_in_move = []
        else:
            # Divide la riga in stato e numero
            parts = line.strip().split(';')
            state = parts[0]
            state_num = int(parts[1].strip())  # Estrai il numero di collegamento

            # Aggiungi il vettore di stato alla lista di stati all'interno della mossa
            states_in_move.append(state)

            # Salva lo stato corrente come stato precedente per la prossima iterazione
            previous_state = states_in_move

# Disegna il grafo
pos = nx.spring_layout(G)  # Calcola la posizione dei nodi
nx.draw(G, pos, with_labels=True, node_size=700, node_color='skyblue', font_size=10, font_color='black')
plt.title('Grafo Tic Tac Toe')
plt.show()

'''
#/////////////////////////////////////////////////////////////////////////
import networkx as nx
import matplotlib.pyplot as plt

# Inizializza un grafo vuoto
G = nx.Graph()

# Apre il file CSV
with open('TTT_3^2.csv', 'r') as file:
    # Inizializza una lista di stati all'interno di una mossa
    states_in_move = []
    state_num = []
    previous_state = None  # Inizializza lo stato precedente a None
    for line in file:
        if line.startswith('-----------------------------'):
            lunghezza = len(states_in_move)
            print("La lunghezza di states_in_move è:", lunghezza)

            # Hai raggiunto la fine di una mossa, procedi con la gestione delle mosse
            # Aggiungi nodi e archi per gli stati in questa mossa
            for state in states_in_move:
                G.add_node(state)
                
            # Collega il nodo corrente al nodo della mossa precedente (se esiste)
            if previous_state is not None:
                for index, state in enumerate(states_in_move):
                     for index2, prev in enumerate(previous_state):
                            if index2 == state_num[index]:
                               G.add_edge(state, previous_state[index2])
            previous_state=[]
            previous_state = states_in_move
            
            
            if previous_state is None:
                previous_state = states_in_move
            

            # Svuota la lista degli stati all'interno della mossa
            states_in_move = []
        else:
            # Divide la riga in stato e numero
            parts = line.strip().split(';')
            state = parts[0]
            state_num.append(int(parts[1].strip()))  # Estrai il numero di collegamento

            # Aggiungi il vettore di stato alla lista di stati all'interno della mossa
            states_in_move.append(state)

pos = nx.tree_layout(G)
nx.draw(G, pos, with_labels=False, node_size=700, node_color='skyblue', font_size=10, font_color='black')
plt.title('Grafo Tic Tac Toe')
plt.show()


