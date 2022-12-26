# Robust_Interact


This package contains 3 directories under `Robust_Interact/Code`: 
1. Directory `Robust_Interact`, which contains the source code of the proposed algorithms

	(1). *2RI* (only works for 2-dimensional datasets)
  
	(2). *Verify-Point* (can be found in PointPrune.cpp)
  
	(3). *Verify-Space* (can be found in SpacePrune.cpp)
  
      The baseline algorithms can also be found in this directory.

2. Directory `RI_user_study`, which contains the source code for user study

	(1). *Verify-Point* (ours)
  
	(2). *Verify-Space* (ours)
  
	(3). *Preference-Learning*
  
	(4). *HDPI*
  
	(5). *Active-Ranking*
  
	(6). *UH-Simplex*

3. Directory `Var_experiments`, which contains the source code for variants of *Verify-Point* and *Verify-Space*. The following variants are provided:

	(1). Return a set of points with size at most *k* which contains the best point (instead of only returning the best point)
	
	(2). Return the top-*k* points (instead of only returning the best point)
	
	(3). Display *s* points in Conjecture Phase and ask the user to select the favorite point (instead of pairwise comparison)
	
	(4). Display *s* points in Conjecture Phase and ask the user to partition them into *superior* and *inferior* groups (instead of pairwise comparison)



Part of the code of *Verify-Space* is based on `volesti` https://github.com/GeomScale/volesti.

The code requires `glpk` and `lp-solve` to run.

If you are using ubuntu, you can run the following command to install `glpk` and `lp-solve`.

```
sudo apt-get install libglpk-dev
sudo apt-get install lp-solve
```

To compile the code, clone it to your local machine and direct to `Code/Experiments`, `Code/RI_user_study` or `Code/Var_Experiments`, then run the following.

```
mkdir output
mkdir build
cd build
cmake ..
make
```

All the used dataset (after filtering out tuples with missing values and normalization) can be found under the `input` folder. The source of datasets are as follows.

(1). *Diamond* can be found at https://www.kaggle.com/datasets/shivam2503/diamonds.

(2). *NBA* was extracted from https://databasketball.com/.

(3). *Car* was extracted from Yahoo! autos https://autos.yahoo.com/.

(4). *Island* was downloaded from Yufei Tao's homepage.
