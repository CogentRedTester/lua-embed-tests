#!/bin/bash
if make
then
    ./a.out "$@"
fi

echo $?
