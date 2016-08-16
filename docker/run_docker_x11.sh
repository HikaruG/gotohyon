#!/bin/bash

if [ -z "$1" ]; then
    echo "Utilisation:"
    echo "  $0 <nom de l'image docker>"
    exit 2
fi
IMAGE=$1

docker run -it \
    --user=$USER \
    --env="DISPLAY" \
    --volume="/etc/group:/etc/group:ro" \
    --volume="/etc/passwd:/etc/passwd:ro" \
    --volume="/etc/shadow:/etc/shadow:ro" \
    --volume="/etc/sudoers.d:/etc/sudoers.d:ro" \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    $IMAGE
    /bin/bash

