
# Structural Rounding
See example.py if you'd like to write code that makes use of our structural rounding algorithms.


## Compiling
- **Python** Simply run ```make python``` to compile.
- **C++** Simply run ```make cpp``` to compile.


## Generating Synthetic Graphs
Compile the generator by running ```make compile```.
Then, running ```make small_graphs``` will create 5 graphs per parameter setting (3600 graphs in total) each with 4 million edges in expectation.
To create smaller graphs, use ```make test_graphs``` which creates graphs with 100 thousand edges in expectation.
To create larger graphs, use ```make medium_graphs``` (40 million edges) or ```make large_graphs``` (200 million edges).
Note that the largest graphs can exceed 600MB in size even with a space efficient encoding.

You can run the generator script directly to create different sizes of graphs using ```./generator/create_graphs_vc.sh <edges> <directory>```.


## Running Experiments
- **Python** Once compiled, run ```python main.py <graphs-directory>```
- **(or) C++** Once compiled, run ```./main <graphs-directory>```

~~Once you have created synthetic graphs, you can reproduce our experimental results by running ```make small_data```.
Use ```make medium_data``` or ```make large_data``` if appropriate.
You can run our experiments on different sizes of graphs using ```python main.py <directory>```.
Note that the ```make``` commands additionally disable Python's random hashing feature so that results are consistent between runs.~~

The variance experiments can also be reproduced using ```make test_data```.
Do the large amount of repetition, it is not recommended to use graphs with more than 100 thousand edges in the variance tests.
To run the variance experiments on graphs of a different size, use ```python variance.py <directory>```.
