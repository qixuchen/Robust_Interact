# Robust_Interact


This package contains 2 directories: 
1. Directory "Robust Interaction", which contains the source codes of the proposed algorithms

	(1). *2RI* (only works for 2-dimensional datasets)
  
	(2). *Verify-Point* (can be found in PointPrune.cpp)
  
	(3). *Verify-Space* (can be found in SpacePrune.cpp)
  
      The baseline algorithms can also be found in this directory.

2. Directory "RI_user_study", which contains the source codes for user study

	(1). *Verify-Point* (ours)
  
	(2). *Verify-Space* (ours)
  
	(3). *Preference-Learning*
  
	(4). *HDPI*
  
	(5). *Active-Ranking*
  
	(6). *UH-Simplex*


Part of the code of *Verify-Space* is based on `volesti` https://github.com/GeomScale/volesti.

The code requires `glpk` and `lp-solve` to run.

If you are using ubuntu, you can run the following command to install `glpk` and `lp-solve`.

```
sudo apt-get install libglpk-dev
sudo apt-get install lp-solve
```

To compile the code, clone it to your local machine and direct to `Code/Experiments` or `Code/RI_user_study`, then run the following.

```
mkdir output
mkdir build
cd build
cmake ..
make
```
