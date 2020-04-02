
# Structural Rounding
See example.py if you'd like to write code that makes use of our structural rounding algorithms.


## Compiling
- **Python** Simply run ```make python``` to compile.
- **C++** Simply run ```make cpp``` to compile.


## Generating Synthetic Graphs
Compile the generator by running ```make generator```.
Then, running ```./create_graphs.sh small``` will create 5 graphs per parameter setting (3600 graphs in total) each with 4 million edges in expectation.
To create smaller graphs, use ```./create_graphs.sh test``` which creates graphs with 100 thousand edges in expectation.
To create larger graphs, use ```./create_graphs.sh medium``` (40 million edges) or ```./create_graphs.sh large``` (200 million edges).

Note that the largest graphs can exceed 600MB in size even with a space efficient encoding.

You can also create different sizes of graphs using ```./create_graphs.sh custom <edges> <directory>```.


## Running Experiments
- **Python** Once compiled, run ```python main.py <graphs-directory/>```
- **C++** Once compiled, run ```./main <graphs-directory/>``` or ```./main <graph.s6>```

Once you have created synthetic graphs, you can reproduce our experimental results by running ```main.py``` on ```graphs/small/```, ```graphs/medium/```, and ```graphs/large/```.

## Citation and License
**Important**: Structural-Rounding is *research software*, so you should cite us when you use it in scientific publications!
Please see the [CITATION](./CITATION) file for citation information.
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3401541.svg)](https://doi.org/10.5281/zenodo.3401541)

Structural-Rounding is released under the BSD license; see the [LICENSE](./LICENSE) file.
Distribution, modification and redistribution, and incorporation into other software is allowed.
