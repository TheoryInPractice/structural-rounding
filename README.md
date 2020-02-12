
# Structural Rounding

See example.py if you'd like to write code that makes use of our structural rounding algorithms.


## Compiling
- **Python** To compile, first edit ```PYTHON_DIR``` in the Makefile to match the installation directory of Python on your machine.
On linux machines, this should be as simple as changing the version number.
Run ```make python``` to compile the code.
- **(or) C++** Simply run ```make cpp``` to compile the code.


## Generating Synthetic Graphs

Compile the generator by running ```make generator```.
Then, running ```./create_graphs.sh small``` will create 5 graphs per parameter setting (3600 graphs in total) each with 4 million edges in expectation.
To create smaller graphs, use ```./create_graphs.sh test``` which creates graphs with 100 thousand edges in expectation.
To create larger graphs, use ```./create_graphs.sh medium``` (40 million edges) or ```./create_graphs.sh large``` (200 million edges).
Note that the largest graphs can exceed 600MB in size even with a space efficient encoding.

You can also create different sizes of graphs using ```./custom_create_graphs.sh <edges> <directory>```.


## Running Experiments

Once you have created synthetic graphs, you can reproduce our experimental results by running ```make small_data```.
Use ```make medium_data``` or ```make large_data``` if appropriate.
You can run our experiments on different sizes of graphs using ```python main.py <directory>```.
Note that the ```make``` commands additionally disable Python's random hashing feature so that results are consistent between runs.

The variance experiments can also be reproduced using ```make test_data```.
Do the large amount of repetition, it is not recommended to use graphs with more than 100 thousand edges in the variance tests.
To run the variance experiments on graphs of a different size, use ```python variance.py <directory>```.
