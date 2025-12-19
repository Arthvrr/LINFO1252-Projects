import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

#recuperer les données et nettoyer 
def data_loading(filename):
    data = pd.read_csv(filename)
    data = data.dropna()
    data["threads_number"] = pd.to_numeric(data["threads_number"], errors="coerce")
    data["compilation_time"] = pd.to_numeric(data["compilation_time"], errors="coerce")
    data = data.dropna()
    return data

#creation des plots en récupérant les means et stds
def plotting(filename,title_suffix=''):
    """
    Function to generate execution time plots per program based on a CSV file.
    
    :param filename: str - Path to the input CSV file
    :param output_file: str - Path to save the resulting plot
    :param title_suffix: str - Optional suffix for plot titles
    """
    data = data_loading(filename)
    programs = data["program_name"].unique() #creation d'une liste avec les noms de programmes

    #iteration dans le graph en fonction des noms de programmes pour trouver le mean et std afin de créer les subplots
    for i, program in enumerate(programs): 
        ax = axes[i] #attribution de la place du subplot
        p_data = data[data["program_name"] == program]

        threads = p_data["threads_number"].unique()
        means, stds = [], []

        for t in threads:
            t_data = p_data[p_data["threads_number"] == t]["compilation_time"]
            means.append(t_data.mean())
            stds.append(t_data.std())
        
        #ajout de la courbe means et des stds
        ax.plot(threads, means, marker='o', label="Means curve", markersize=8, color='royalblue')
        ax.errorbar(threads, means, yerr=stds, fmt='o', color='r', label="Standard deviation", capsize=5)
        
        #mise en forme du sublot
        ax.set_title(f"{title_suffix} : {program} ")
        ax.set_ylabel("Execution Time (ms)", fontsize=12, fontweight='bold')
        ax.set_xlabel("Threads Number", fontsize=12, fontweight='bold')
        ax.grid(True, linestyle='--', alpha=0.5)
        ax.legend(fontsize=10)

#creation de la figure
fig, axes = plt.subplots(1, 3, figsize=(15, 10), dpi=150)
plt.suptitle('Execution Time per Threads for differents programs',fontsize=16, fontweight='bold')

#ajout des subplots a la figure
plotting('output_tatas.csv',title_suffix='1.4')

plt.tight_layout()
plt.subplots_adjust(hspace=0.5)
plt.savefig('graph.pdf')
plt.close()