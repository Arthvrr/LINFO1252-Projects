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

def plotting(filename1,filename2,ax):

    data1= data_loading(filename1)
    data2=data_loading(filename2
                       )
    threads = data1["threads_number"].unique()
    means1, means2 = [], []
    stds1, stds2 = [], []
    
    for t in threads:
        t_data = data1[data1["threads_number"] == t]["compilation_time"]
        y_data = data2[data2["threads_number"] == t]["compilation_time"]
        means1.append(t_data.mean())
        means2.append(y_data.mean())
        stds1.append(t_data.std())
        stds2.append(y_data.std())
        
    #ajout de la courbe means et des stds
    ax.plot(threads, means1, marker='o', label="TAS", markersize=8, color='royalblue')
    ax.plot(threads, means2, marker='o', label="TATAS", markersize=8, color='yellow')
    ax.errorbar(threads, means1, yerr=stds1, fmt='o', color='r', label="Standard deviation", capsize=5)
    ax.errorbar(threads, means2, yerr=stds2, fmt='o', color='g', label="Standard deviation", capsize=5)
        
    #mise en forme du sublot
    ax.set_ylabel("Execution Time (s)", fontsize=12, fontweight='bold')
    ax.set_xlabel("Threads Number", fontsize=12, fontweight='bold')
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.legend(fontsize=10)

#creation de la figure
fig, axes = plt.subplots(1, figsize=(15, 10), dpi=150)
plt.suptitle("Comparaison des temps d'éxécution de TAS et TATAS",fontsize=16, fontweight='bold')

#ajout des subplots a la figure
plotting('output_test_and_set.csv','output_test_and_test_and_set.csv',axes)

plt.tight_layout()
plt.subplots_adjust(hspace=0.5)
plt.savefig('graph2.png')
plt.close()