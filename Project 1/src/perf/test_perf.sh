#!/bin/bash

echo "number,program_name,threads_number,compilation_time" > output.csv

# Définitions des variables
mesures=(2 4 8 16 32)

src_path="$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1/src"
build_path="$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1"

programs=("philosoph" "prod_cons" "reader_writer")

make -C "$build_path" clean #on nettoie les anciens fichiers
make -C "$build_path" #compile tous les programmes

nb=0

# Exécution des tests de performance
for exec in "${programs[@]}"; do
    exec_path="${src_path}/${exec}"

    # Vérifiez si l'exécutable a été généré
    if [ ! -f "${exec_path}" ]; then
        echo "Error: Executable $exec_path not found."
        continue
    fi

    # Lancer les mesures
    for threads in "${mesures[@]}"; do
        cnt=0 #compteur des chronos, se remet à 0 après chaque programme
        midThread=$((threads / 2))

        for i in {1..5}; do
            if [ "$exec" == "prod_cons" ]; then
                elapsed=$({ time make -C "$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1" prod_cons PROD=${midThread} CONS=${midThread} > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')

            elif [ "$exec" == "reader_writer" ]; then
                elapsed=$({ time make -C "$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1" reader_writer WRITER=${midThread} READER=${midThread} > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')
                
            else 
                elapsed=$({ time make -C "$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1" philosoph N=2 > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')
            fi

            # Vérifiez si elapsed est un nombre valide avant de l'ajouter à cnt
            if [[ "$elapsed" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
                cnt=$(echo "$cnt + $elapsed" | bc)
            else
                echo "Error: Invalid time format: $elapsed"
                continue
            fi
        done

        avg=$(echo "scale=3; $cnt / 5" | bc) # moyenne des 5 essais
        echo "${nb},${exec},${threads},${avg}" >> output.csv # ajout des valeurs au CSV
    done
    nb=$((nb+1))
done