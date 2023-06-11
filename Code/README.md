# Robust_Interact


This package contains 3 directories: 
1. Directory `Experiments`, which contains the source code of the proposed algorithms.

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

2. Directory `RI_user_study`, which contains the source code for user study.

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

See the readme files in these directories for further detail.

Part of the code of *Verify-Point* and *Verify-Space* is based on https://github.com/Weicheng1996/SIGMOD2021.
Part of the code of *Verify-Space* is based on `volesti` https://github.com/GeomScale/volesti.

All datasets used in our paper can be found under `Experiments/input`.