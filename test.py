
import networkx as nx
from networkx.drawing.nx_pydot import pydot_layout
import matplotlib.pyplot as plt
# Inizializza un grafo vuoto
G = nx.DiGraph()

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
                #print ("la lunghezza di state num è")
                #print (len(state_num))
                #print ("state num è")
                #print (state_num)
                for index2, prev in enumerate(previous_state):
                    for index, state in enumerate(states_in_move):
                               #print("indice dello stato precedente è:") 
                               #print(index2)
                               #print("indice associato allo stato ")
                               #print(state)
                               #print("è:")
                               #print(state_num[index])
                               if index2 == state_num[index]:
                                #print("VERO")
                                G.add_edge(previous_state[index2],states_in_move[index])
            previous_state=[]
            state_num=[]
            previous_state = states_in_move
            
            
            if previous_state is None:
                previous_state = states_in_move
            

            # Svuota la lista degli stati all'interno della mossa
            states_in_move = []
        else:
            # Divide la riga in stato e numero
            parts = line.strip().split(';')
            state = parts[0]
            #print("int(parts[1].strip()) è")
            #print (int(parts[1].strip()))
            state_num.append(int(parts[1].strip()))  # Estrai il numero di collegamento

            # Aggiungi il vettore di stato alla lista di stati all'interno della mossa
            states_in_move.append(state)
countX=0
countO=0
countN=0
scores = {}
end_states = [] 
'''with open('TTT_ENDS.csv', 'r') as file:
    for line in file:
        parts = line.strip().split(';')
        state = parts[0]
        win_type = int(parts[1].strip())
        if win_type ==1:
            countX=countX+1
        if win_type ==2:    
            countO=countO+1
        if win_type ==0:
            countN=countN+1
        end_states.append(state)
        scores[state] = win_type

print("count X è:",countX)
print("count O è:",countO)
print("count N è:",countN)
'''
bolla = 1
for state in end_states:
    if state in [node for node in G.nodes if not list(G.successors(node))]:
        bolla=bolla*1
    else:
        bolla=bolla*0
if bolla==1:
    print("VERO")    


root_state = "[ ], [ ], [ ], [ ], [ ], [ ], [ ], [ ], [ ]"  # Stato radice
node_attributes = {}
node_prob = {}
def minimax(node, depth, maximizing_player, node_attributes,G):
    if depth == 0 or node in end_states:
        if (maximizing_player) :
            if scores[node] ==1:
                value = 1000
            if scores[node] ==2:
                value = -1000
            if scores[node] ==0:
                value = 0
        else :
            if scores[node] ==1:
                value = -1000
            if scores[node] ==2:
                value = 1000
            if scores[node] ==0:
                value = 0
        node_attributes[node] = {'value': value}        
        return value

    if maximizing_player:
        best_value = -1000
        successors = list(G.successors(node))
        for child in successors:
            value = minimax(child, depth - 1, False,node_attributes,G)
            best_value = max(best_value, value)
            node_attributes[child] = {'value': best_value}  

        return best_value
    else:
        best_value = 1000
        successors = list(G.successors(node))
        for child in successors:
            value = minimax(child, depth - 1, True,node_attributes,G)
            best_value = min(best_value, value)
            node_attributes[child] = {'value': best_value}  

        return best_value

def final_correction(maximizing_player, node_attributes,G):
    # Definisci l'attributo che vuoi controllare
    attributo_da_verificare = "value"

    #    Crea una lista per memorizzare i nodi senza l'attributo specificato
    nodi_senza_attributo = []

    for nodo in G.nodes:
    # Controlla se l'attributo specificato non è presente nel nodo
        if attributo_da_verificare not in G.nodes[nodo]:
            nodi_senza_attributo.append(nodo)
    value=0
    for node in nodi_senza_attributo:
        if node in end_states:
            if (maximizing_player) :
                if scores[node] ==1:
                    value = 1000
                if scores[node] ==2:
                    value = -1000
                if scores[node] ==0:
                    value = 0
            else :
                if scores[node] ==1:
                    value = -1000
                if scores[node] ==2:
                    value = 1000
                if scores[node] ==0:
                    value = 0
            node_attributes[node] = {'value':value}

def prob_ass(node,node_prob,maximizing_player,G):
    if node in end_states:
        node_prob[node] = {'prob': 0}
    
    successors = list(G.successors(node))
    count=0
    if maximizing_player:
        confront=-1000
    else :
        confront =1000

    for child in successors:
        if G.nodes[child]["value"]== confront or G.nodes[child]["value"]== 0:
            count=count+1
    if count==0:
        prob=0
    else:
        prob = 1/count
    for child in successors:
        if G.nodes[child]["value"]== confront:
            node_prob[child] = {'prob': prob}
    for child in successors:
        prob_ass(child,node_prob,not maximizing_player,G)



'''
minimax(root_state,9,True,node_attributes,G)
final_correction(True,node_attributes,G)
#print(node_attributes)
nx.set_node_attributes(G, node_attributes)
# Ottenere gli attributi dei nodi
attributes = nx.get_node_attributes(G, 'value')

# Definisci l'attributo che vuoi controllare
attributo_da_verificare = "value"

# Crea una lista per memorizzare i nodi senza l'attributo specificato
nodi_senza_attributo = []

for nodo in G.nodes:
    # Controlla se l'attributo specificato non è presente nel nodo
    if attributo_da_verificare not in G.nodes[nodo]:
        nodi_senza_attributo.append(nodo)

# Ora la lista `nodi_senza_attributo` contiene i nodi che non hanno l'attributo "value"
print("Nodi senza l'attributo '{}':".format(attributo_da_verificare))
print(nodi_senza_attributo)
print("i nodi senza attributo in end_state sono:")
for node in nodi_senza_attributo:
    if node in end_states:
        print(node)
# Stampa gli attributi
#for node, value in attributes.items():
#    print(f"Nodo {node}: value={value}")

prob_ass(root_state,node_prob,True,G)
print (node_attributes)
print(node_prob)

'''

# Calcola la posizione dei nodi in un layout ad albero
pos = pydot_layout(G, prog='dot')

# Disegna il grafo come un albero
nx.draw(G, pos, with_labels=True, node_size=700, node_color='skyblue', font_size=10, font_color='black')

plt.title('Grafo Tic Tac Toe')
plt.show()
