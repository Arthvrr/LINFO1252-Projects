#!/bin/bash

pc_path="$HOME/Documents/Syst Info/P1/P1-LINFO1252/src/prod_cons.c"
rw_path="$HOME/Documents/Syst Info/P1/P1-LINFO1252/src/reader_writer.c"
include_path="$HOME/Documents/Syst Info/P1/P1-LINFO1252/include"


un=$({ time make -C "$HOME/Documents/Systèmes Informatiques - LINFO1252/Projets/Projet 1" philosoph N=2 > /dev/null; } 2>&1 | grep real | awk '{print $2}')
#$(/usr/bin/time -f "%e" make -C "$HOME/Documents/Syst Info/P1/P1-LINFO1252" prod_cons PROD=2 CONS=2 2>&1 > /dev/null)
#do=$(/usr/bin/time -f "%e" make -C "$HOME/Documents/Syst Info/P1/P1-LINFO1252" reader_writer WRITER=2 READER=2 2>&1 > /dev/null)

echo " premier temps : ${un},"