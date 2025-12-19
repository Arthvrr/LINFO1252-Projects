import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

#recuperer les données et nettoyer 
def data_loading(filename):
    data = pd.read_csv(filename)
    data = data.dropna()
    data["threads_number"] = pd.to_numeric(data["threads_number"], errors="coerce")
    data["compilation_time"] = pd.to_numeric(data["compilation_time"], errors="coerce")
    
    # Vérifier les NaN et les valeurs infinies
    if data.isnull().values.any():
        print(f"Attention : le fichier {filename} contient des valeurs NaN après conversion.")
    if not np.isfinite(data["compilation_time"]).all():
        print(f"Attention : le fichier {filename} contient des valeurs infinies.")
    
    return data.dropna()
#creation des plots en récupérant les means et stds
def plotting(filename1,filename2,filename3):
    """
    Function to generate execution time plots per program based on a CSV file.
    
    :param filename: str - Path to the input CSV file
    :param output_file: str - Path to save the resulting plot
    :param title_suffix: str - Optional suffix for plot titles
    """
    data = data_loading(filename1)
    data_tas = data_loading(filename2)
    data_tatas = data_loading(filename3)

    programs = data["program_name"].unique() #creation d'une liste avec les noms de programmes
    
    
    #iteration dans le graph en fonction des noms de programmes pour trouver le mean et std afin de créer les subplots
    for i, program in enumerate(programs): 
        ax = axes[i] #attribution de la place du subplot
        
        p_data = data[data["number"] == i]
        p_data_tas = data_tas[data_tas["number"] == i]
        p_data_tatas = data_tatas[data_tatas["number"] == i]

        threads = p_data["threads_number"].unique()
        

        means1, means2, means3 = [], [], []
        stds1, stds2,stds3 = [], [], []

        for t in threads:
            t_data = p_data[p_data["threads_number"] == t]["compilation_time"]
            u_data = p_data_tas[p_data_tas["threads_number"] == t]["compilation_time"]
            v_data = p_data_tatas[p_data_tatas["threads_number"] == t]["compilation_time"]

            means1.append(t_data.mean())
            means2.append(u_data.mean())
            means3.append(v_data.mean())

            stds1.append(t_data.std())
            stds2.append(u_data.std())
            stds3.append(v_data.std())
        
            #ajout de la courbe means et des stds
        ax.plot(threads, means1, marker='o', label=f"{program}", markersize=8, color='blue')
        ax.plot(threads, means2, marker='o', label="Test and set", markersize=8, color='green')
        ax.plot(threads, means3, marker='o', label="Test and test and set", markersize=8, color='brown')

        ax.errorbar(threads, means1, yerr=stds1, fmt='o', color='skyblue', label="Standard deviation", capsize=5)
        ax.errorbar(threads, means2, yerr=stds2, fmt='o', color='lime', label="Standard deviation", capsize=5)
        ax.errorbar(threads, means3, yerr=stds3, fmt='o', color='gold', label="Standard deviation", capsize=5)
        
            #mise en forme du sublot
        ax.set_title(f"Execution time for {program} ")
        ax.set_ylabel("Execution Time (s)", fontsize=12, fontweight='bold')
        ax.set_xlabel("Threads Number", fontsize=12, fontweight='bold')
        ax.grid(True, linestyle='--', alpha=0.5)
        ax.legend(fontsize=10)

#creation de la figure
fig, axes = plt.subplots(1, 3, figsize=(15, 10), dpi=150)
plt.suptitle('Execution Time per Threads for differents programs',fontsize=16, fontweight='bold')

#ajout des subplots a la figure
plotting('output.csv','output_tas.csv','output_tatas.csv')


plt.tight_layout()
plt.subplots_adjust(hspace=0.5)
plt.savefig('graph3.pdf')
plt.close()