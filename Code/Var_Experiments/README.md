The Directory contains the source code for variants of *Verify-Point* and *Verify-Space*. The following variants are provided:

(1). Return a set of points with size at most *k* which contains the best point (instead of only returning the best point)

(2). Return the top-*k* points (instead of only returning the best point)

(3). Display *s* points in Conjecture Phase and ask the user to select the favorite point (instead of pairwise comparison)

(4). Display *s* points in Conjecture Phase and ask the user to partition them into *superior* and *inferior* groups (instead of pairwise comparison)

This package also provides the synthetic datasets used for testing under directory `input`.

Make sure there is a folder called `input` and a folder called `output` under the directory `Var_Experiments`.
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
Under directory `Var_Experiments`, run the following: 

    mkdir output
    mkdir build
    cd build
    cmake ..
    make
    
	
### Execution
Under directory `Var_Experiments/build`, run

	./VAR

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
The output consists of several parts, we give an example for each part of the output.

In the following examples we assume that `k=5`.

Further, we use the following notations:

Notation | Meaning |
-----|-----|
Alg | name of the algorithm (`Verify-Point` or `Verify-Space`)|
PID | ID of the point |
ret_size | size of the returned set |
num_ques | number of questions used by the algorithm |


1, ground truth 

    ---------------------------------------------------------
    |                  Ground Truth |        -- |        -- |
    |                         Point |   top - 1 |     PID_1 |
    |                         Point |   top - 2 |     PID_2 |
    |                         Point |   top - 3 |     PID_3 |
    |                         Point |   top - 4 |     PID_4 |
    |                         Point |   top - 5 |     PID_5 |
    ---------------------------------------------------------

which is the ground truth of the top-5 points.

2, Return a set of points with size at most *k* which contains the best point (instead of only returning the best point)

    ---------------------------------------------------------
    |        Alg (contain best) |      num_ques |        -- |
    |               Return size |            -- |   ret_size|
    ---------------------------------------------------------

3, Return the top-*k* points (instead of only returning the best point)

    ---------------------------------------------------------
    |                    Alg (topk) |  num_ques |        -- |
    |                         Point |   top - 1 |     PID_1 |
    |                         Point |   top - 2 |     PID_2 |
    |                         Point |   top - 3 |     PID_3 |
    |                         Point |   top - 4 |     PID_4 |
    |                         Point |   top - 5 |     PID_5 |
    ---------------------------------------------------------

which are the top-5 found by `Alg`.


4, Display *s* points in Conjecture Phase and ask the user to select the favorite point (instead of pairwise comparison)

    ---------------------------------------------------------
    |                Alg (favorite) |  num_ques |        -- |
    |                         Point |        -- |      PID_1|
    ---------------------------------------------------------

where `PID_1` is the best point found by `Alg`.

5, Display *s* points in Conjecture Phase and ask the user to partition them into *superior* and *inferior* groups (instead of pairwise comparison)

    ---------------------------------------------------------
    |                 Alg (sup/inf) |  num_ques |        -- |
    |                         Point |        -- |      PID_1|
    ---------------------------------------------------------

where `PID_1` is the best point found by `Alg`.