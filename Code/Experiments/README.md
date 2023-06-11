This Directory contains the source code of the proposed algorithms.

	(1). *2RI* (only works for 2-dimensional datasets)
  
	(2). *Verify-Point* (can be found in PointPrune.cpp)
  
	(3). *Verify-Space* (can be found in SpacePrune.cpp)
  
    The following baseline algorithms can also be found in this directory.
	
	(a). Algorithms for 2-d:

	(1). *2DPI* 

	(2). *Median*

	(3). *Hull*

	(b). Algorithms for multi-d:

	(1). *HD-PI*

	(2). *UtilApprox*

	(3). *Active-Ranking*

	(4). *Pref-Learning*

	(5). *UH-Simplex*

	(6). *RH*

This package also provides the synthetic and real datasets under directory `input`.
Links to the original dataset can be found in our paper.

Make sure there is a folder called `input` and a folder called `output` under the directory `Experiments`.
They will be used for storing the input/output files and some intermediate results.

## Dependencies

The code requires `glpk` and `lp-solve` to run. The following shows how to install these packages if you are using Linux.

### Install glpk

Run the following command to install `glpk`.

    sudo apt-get install libglpk-dev

Then update the following lines in file `CMakeLists.txt`:

    set(INC_DIR /usr/local/Cellar/glpk/4.65/include)
    set(LINK_DIR /usr/local/Cellar/glpk/4.65/lib)

Update the path `/usr/local/Cellar/glpk/4.65` to the path where you installed the `glpk` package.

### Install lp_solve

Run the following command to install `lp_solve`.

    sudo apt-get install lp-solve

Then update the following line in file `CMakeLists.txt`:

    find_library(LP_SOLVE NAMES liblpsolve55.so PATHS /usr/lib/lp_solve)

Update the path `/usr/lib/lp_solve` to the path where you installed the `lp_solve` package.

## Usage Steps

### Compilation
Under directory `Experiments`, run the following: 

    mkdir output
    mkdir build
    cd build
    cmake ..
    make
    
	
### Execution
Under directory `Experiments/build`, run

	./RI

### Input
The datasets tested in our paper can be found under directory `input`.
A dataset has the following format:

    num_of_record dim
    record_1
    record_2
    record_3
    ...
where `num_of_record` is the total number of record in the dataset, and `dim` is the dimensionality.

A toy 3-d dataset with 4 records:

    4 3
    1   0   0
    0   1   0
    0   0   1
    0.5 0.5 0
	
### Output
The output will be shown on the console. It runs an algorithm with name `Alg` for a number of times (e.g., 100 times). 

In each round, it displays the ground truth best point and the point returned by `Alg`.

The format of output for one round is as follows:

    round i
    ---------------------------------------------------------
    |                  Ground Truth |        -- |        -- |
    |                         Point |   top - 1 |     PID_1 |
    ---------------------------------------------------------
    |                           Alg |num_of_ques|        -- |
    |                         Point |        -- |     PID_2 |
    ---------------------------------------------------------

where `PID_1` is the id of the ground truth best point, `PID_2` is the id of the point returned by `Alg`. If `PID_1 == PID_2`, the best point is found by `Alg`.

`num_of_ques` is the number of questions used by `Alg` to return the answer. 

At the end of the output, it displays the `accuracy`, `avg question num` and `avg time` of the tested algorithm `Alg`.


