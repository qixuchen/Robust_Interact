cmake_minimum_required(VERSION 3.15)
project(VAR)


enable_testing()

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

if(COMMAND cmake_policy)
       cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#if(NOT DEFINED BOOST)
#  message(FATAL_ERROR "This program requires the boost library, and will not be compiled.")
#else()
#  message(STATUS "BOOST is DEFINED")
#endif()

find_library(LP_SOLVE NAMES liblpsolve55.so PATHS /usr/lib/lp_solve)

if (NOT LP_SOLVE)

  message(FATAL_ERROR "This program requires the lp_solve library, and will not be compiled.")

else ()

  message(STATUS "Library lp_solve found: ${LP_SOLVE}")

  set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")


  set(CMAKE_CXX_STANDARD 14)
  set(INC_DIR /usr/local/Cellar/glpk/4.65/include)
  set(LINK_DIR /usr/local/Cellar/glpk/4.65/lib)




  include_directories(.)
  include_directories(2DPI)
  include_directories(Others)
  include_directories(Qhull)

  include_directories(${INC_DIR})
  link_directories(${LINK_DIR})
  link_libraries(glpk)


  include_directories (BEFORE ./external/Eigen)
  include_directories (BEFORE ./external)
  include_directories (BEFORE ./external/minimum_ellipsoid)
  #include_directories (BEFORE ../include/cartesian_geom)
  #include_directories (BEFORE ../include/convex_bodies)
  include_directories (BEFORE ./external/LPsolve_src/run_headers)
  include_directories (BEFORE ./external/boost)
  #include_directories (BEFORE BOOST)
  include_directories (BEFORE ./include/generators)
  include_directories (BEFORE ./include/volume)
  include_directories (BEFORE ./include)
  include_directories (BEFORE ./include/convex_bodies)
  include_directories (BEFORE ./include/annealing)
  include_directories (BEFORE ./include/samplers)
  include_directories (BEFORE ./include/lp_oracles)
  include_directories (BEFORE ./include/misc)

  #for Eigen
  if (${CMAKE_VERSION} VERSION_LESS "3.12.0") 
    add_compile_options(-D "EIGEN_NO_DEBUG")
  else ()
    add_compile_definitions("EIGEN_NO_DEBUG")
  endif ()


  add_definitions(${CMAKE_CXX_FLAGS} "-std=c++11")  # enable C++11 standard
  add_definitions(${CMAKE_CXX_FLAGS} "-g")  # enable debuger
  #add_definitions(${CMAKE_CXX_FLAGS} "-Wint-in-bool-context")
  #add_definitions(${CMAKE_CXX_FLAGS} "-Wall")
 
  #add_definitions(${CMAKE_CXX_FLAGS} "-O3")  # optimization of the compiler
  #add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgsl")
  add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lm")
  add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-ldl")
  add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-DBOOST_NO_AUTO_PTR")
  #add_definitions(${CXX_COVERAGE_COMPILE_FLAGS} "-lgslcblas")
  #add_definitions( "-O3 -lgsl -lm -ldl -lgslcblas" )


  add_definitions(${CMAKE_CXX_FLAGS} "-g")

  add_executable(VAR
        ST2D/ST2D.h
        ST2D/ST2D.cpp
        Median_Hull/medianhull.cpp
        Median_Hull/medianhull.h
        2DPI/2dPI.cpp
        2DPI/2dPI.h

        HDPI/HDPI.cpp
        HDPI/HDPI.h
        HDPI/partition_func.cpp
        HDPI/partition_func.h
        PointPrune.cpp
        PointPrune.h
        SamplePrune.cpp
        SamplePrune.h

        PointPrune_qtype.cpp
        PointPrune_qtype.h
        SamplePrune_qtype.cpp
        SamplePrune_qtype.h
        variant_func.h
        variant_func.cpp

        UtilityApprox/UtilityApprox.cpp
        UtilityApprox/UtilityApprox.h
        Preference_Learning/preferLearn.cpp
        Preference_Learning/preferLearn.h
        Active_Ranking/active_ranking.cpp
        Active_Ranking/active_ranking.h
        UH/maxUtility.cpp
        UH/maxUtility.h
        RH/alg_one.cpp
        RH/alg_one.h

        Others/user_mode.cpp
        Others/user_mode.h
        Others/data_struct.h
        Others/data_utility.cpp
        Others/data_utility.h
        Others/frame.cpp
        Others/frame.h
        Others/operation.cpp
        Others/operation.h
        Others/pruning.cpp
        Others/pruning.h
        Others/rtree.cpp
        Others/rtree.h
        Others/Array.cc
        Others/Array.hh
        Others/QuadProg++.cc
        Others/QuadProg++.hh
        Others/qhull_build.cpp
        Others/qhull_build.h
        Others/read_write.cpp
        Others/read_write.h
        Others/used_function.cpp
        Others/used_function.h
        Qhull/geom.cpp
        Qhull/geom.h
        Qhull/geom2.cpp
        Qhull/global.cpp
        Qhull/io.cpp
        Qhull/io.h
        Qhull/libqhull.cpp
        Qhull/libqhull.h
        Qhull/mem.cpp
        Qhull/mem.h
        Qhull/merge.cpp
        Qhull/merge.h
        Qhull/poly.cpp
        Qhull/poly.h
        Qhull/poly2.cpp
        Qhull/qhull_a.h
        Qhull/qset.cpp
        Qhull/qset.h
        Qhull/random.cpp
        Qhull/random.h
        Qhull/rboxlib.cpp
        Qhull/stat.cpp
        Qhull/stat.h
        Qhull/user.cpp
        Qhull/user.h
        Qhull/usermem.cpp
        Qhull/userprintf.cpp
        Qhull/userprintf_rbox.cpp
        lp.cpp
        lp.h
        main.cpp)


  target_link_libraries(VAR glpk)

  TARGET_LINK_LIBRARIES(VAR ${LP_SOLVE})
endif()