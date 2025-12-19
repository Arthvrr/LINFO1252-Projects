#!/bin/bash

echo "program_name,threads_number,compilation_time" > output_test_and_test_and_set.csv

mesures=(2 4 8 16 32)
src_path="$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1/src/test_and_test_and_set"
build_path="$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1"

program="test_and_test_and_set_main"

make -C "$build_path" clean # on nettoie les anciens fichiers
make -C "$build_path" # compile tous les programmes

# Vérification de l'exécutable
exec_path="${src_path}/${program}"
if [ ! -f "${exec_path}" ]; then
    echo "Error: Executable $exec_path not found."
    exit 1
fi

# Lancement des mesures et des essais pour chaque nombre de threads
for threads in "${mesures[@]}"; do
    cnt=0 # Compteur des chronos, se remet à 0 après 5 essais
    for i in {1..5}; do
        # Exécution du programme avec le nombre de threads
        elapsed=$({ time make -C "$build_path" test_and_test_and_set_main N=$(threads) > /dev/null; } 2>&1 | grep real | awk '{print $2}' | sed 's/[^0-9.]//g')

        # Si elapsed contient un temps valide, on l'ajoute
        if [[ "$elapsed" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
            cnt=$(echo "$cnt + $elapsed" | bc)
        else
            echo "Error: Invalid time format: $elapsed"
            continue
        fi
    done
    avg=$(echo "scale=3; $cnt / 5" | bc) # Moyenne des 5 essais
    echo "${program},${threads},${avg}" >> output_test_and_test_and_set.csv # Ajout des infos au CSV
done