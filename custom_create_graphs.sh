#!/bin/bash
edges=$1
dir=$2

if [ $# -lt 2 ]
  then
    echo "usage: create_graphs_custom <edges> <directory>"
    exit 1
fi

./generator/create_graphs_vc.sh $edges $dir
