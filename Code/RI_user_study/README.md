This Directory contains the code for user study on the follwing algorithms.

(1). *Verify-Point* (ours)

(2). *Verify-Space* (ours)

(3). *Preference-Learning*

(4). *HDPI*

(5). *Active-Ranking*

(6). *UH-Simplex*

This package also provides real datasets `car` under directory `input` used in the user study.
Links to the original dataset can be found in our paper.

Make sure there is a folder called `input` and a folder called `output` under the directory `RI_user_study`.
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
Under directory `RI_user_study`, run the following: 

    mkdir output
    mkdir build
    cd build
    cmake ..
    make
    
	
### Execution
Under directory `RI_user_study/build`, run

	./User_study 

### Input
The datasets `car` can be found under directory `input`.
It has the following format:

    num_of_record dim
    car_1
    car_2
    car_3
    ...
where `num_of_record` is the total number of record in the dataset, and `dim` is the dimensionality (4 in `car`).

Each car record consist of 4 numbers representing attributes `price`, `year`, `power` and `used KM`. 
	
### Output
The output is the content of the user study. The instruction in the user study is self-explanatory.

There are in total 8 algorithms in the user study (6 mentioned above, 1 variation of `Verify-Point`, and 1 algorithm `STMD` designed by us that was not included in the paper)

Their performances are recorded under directory `RI_user_study/output`.

`result.txt` records the car recommended by each algorithm. One example output:

    --------------------------------------------------------
    |                           Alg | num_question |
    --------------------------------------------------------
    Recommended car:
    --------------------------------------------------------
    |          |Price(USD)|      Year| Power(PS)|   Used KM|
    ---------------------------------------------------------
    |       Car|     17500|      2015|       182|     10000|
    ---------------------------------------------------------
where `Alg` is the name of the algorithm and `num_question` is the number of questions used by `Alg`.

`question.txt`, `hit.txt` and `dissat.txt` each record the number of question, hit and dissatisfaction score (see our paper for the definition of hit and dissatisfaction score) of the tested algorithms, respectively.
