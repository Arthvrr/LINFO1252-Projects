#!/bin/bash

echo "number,program_name,threads_number,compilation_time" > output_tas.csv

# Nombre de threads, chemins utiles et noms des programmes
mesures=(2 4 8 16 32)
 
src_path="$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1/src/test_and_set"
build_path="$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1"

programs=("test_and_set_philosoph" "test_and_set_prod_cons" "test_and_set_reader_writer")

make -C "$build_path" clean #on nettoie les anciens fichiers
make -C "$build_path" #compile tous les programmes

nb=0

# Parcours des programmes
for exec in "${programs[@]}"; do
    exec_path="${src_path}/${exec}"
    # Compteur des chronos, se remet à 0 après chaque programme

    # Vérification de l'exécutable
    if [ ! -f "${exec_path}" ]; then
        echo "Error: Executable $exec_path not found."
        continue
    fi

    # Lancement des mesures et des essais
    for threads in "${mesures[@]}"; do
        cnt=0 # Compteur des chronos, se remet à 0 après 5 essais
        midThread=$((threads / 2))
        for i in {1..5}; do
            if [ "$exec" == "test_and_set_prod_cons" ]; then
                elapsed=$({ time make -C "$build_path" test_and_set_prod_cons PROD=${midThread} CONS=${midThread} > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')
            elif [ "$exec" == "test_and_set_reader_writer" ]; then
                elapsed=$({ time make -C "$build_path" test_and_set_reader_writer WRITER=${midThread} READER=${midThread} > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')
            else
                elapsed=$({ time make -C "$build_path" test_and_set_philosoph N= $(threads) > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')
            fi

            # Si elapsed contient un temps valide, on l'ajoute
            if [[ "$elapsed" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
                cnt=$(echo "$cnt + $elapsed" | bc)
            else
                echo "Error: Invalid time format: $elapsed"
                continue
            fi
        done
        avg=$(echo "scale=3; $cnt / 5" | bc) # Moyenne des 5 essais
        echo "${nb},${exec},${threads},${avg}" >> output_tas.csv # Ajout des infos au CSV
        
    done
    nb=$((nb+1))
done