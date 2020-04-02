#!/bin/bash

if [ ! -f generator/generator.out ]; then
    make generator
fi

if [ "$1" = "small" ]; then
    mkdir -p graphs/small
    ./generator/create_graphs_vc.sh 4000000 graphs/small
elif [ "$1" = "medium" ]; then
    mkdir -p graphs/medium
    ./generator/create_graphs_vc.sh 40000000 graphs/medium
elif [ "$1" = "large" ]; then
    mkdir -p graphs/large
    ./generator/create_graphs_vc.sh 200000000 graphs/large
elif [ "$1" = "test" ]; then
    mkdir -p graphs/test
    ./generator/create_graphs_vc.sh 100000 graphs/test
elif [ "$1" = "custom" ] && [ $# -gt 2 ]; then
    ./generator/create_graphs_vc.sh $2 $3
else
    echo "Provide 'small', 'medium', or 'large' as an argument or use 'create_graphs custom <edges> <directory>'"
fi
