#!/bin/bash

if [ ! -f "CMakeLists.txt" ]; then
    echo "Fichier CMakeLists.txt" non trouvé
    exit 2
fi
