#!/bin/bash

# Run template files

set -e

if [ ! -f "src/template/array_types.c.j2" ] ; then
    echo "Run this script from the root of the project"
    exit 1
fi

j2 src/template/array_types.c.j2 > src/array_types.c
j2 src/template/array.h.j2 > include/robotraconteurlite/array.h
j2 src/template/message_data.c.j2 > src/message_data.c
j2 src/template/message_data.h.j2 > include/robotraconteurlite/message_data.h

python3 -m pre_commit run --all
