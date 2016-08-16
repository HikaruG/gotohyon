#!/bin/bash

if [ ! -d ".git" ]; then
    echo "Doit être exécuté dans le dossier git"
    exit 2
fi

docker build -t plt-initial -f docker/plt-initial . || exit 2
docker build -t plt-build -f docker/plt-build . || exit 2
./docker/run_docker_x11.sh plt-build || exit 2
