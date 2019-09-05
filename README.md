# vc_lifting

All data and results can be recreated using make commands.

make compile: Compiles the generator (written in c++) so that synthetic graphs can be created.

make <--->_graphs: Replacing <---> with small, medium, large, or test will create synthetic graphs with 4 million, 40 million, 200 million, and 100 thousand edges (in expectation respectively).  Note that not all parameter settings we kept in the 40 million and 200 million experiments. Currently, 5 random graphs are created per parameter setting.  This can be changed in generator/create_graphs_vc.sh.  The graphs are placed in the graphs/<---> directory.

make <--->_data: Replace <---> with small, medium, large or test.  Runs experiments on all graphs in the graphs/<---> directory.  For small, medium, and large, a different set of experiments are run from main.py while for test, variance.py is run.

make clean: Deletes the compiled generator and all synthetic graphs.


See example.py if you'd like to write code that makes use of our structural rounding algorithms.
