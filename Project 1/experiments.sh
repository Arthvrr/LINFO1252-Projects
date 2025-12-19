#!/bin/bash

to_execute=(
    "test_perf.sh"
    "test_and_set_perf.sh"
    "test_and_test_and_set_perf.sh"
    "tas_perf.sh"
    "tatas_perf.sh"
)
for script in "${to_execute[@]}"; do
  script_path="src/perf/$script"
  if [ -f "$script_path" ]; then
    echo "Exécution de $script_path..."
    bash "$script_path"
  fi
done

echo "Tous les tests ont été exécutés."