set(CXX_CMAT_SRC mat/c_mat.cpp)

# find Matlab header and libs
set(Matlab_LIBRARIES )
include_directories(/usr/local/MATLAB/R2014a/extern/include)
find_library(t_lib0 mat /usr/local/MATLAB/R2014a/bin/glnxa64)
list(APPEND Matlab_LIBRARIES ${t_lib0})
find_library(t_lib1 mx /usr/local/MATLAB/R2014a/bin/glnxa64)
list(APPEND Matlab_LIBRARIES ${t_lib1})
find_library(t_lib2 mex /usr/local/MATLAB/R2014a/bin/glnxa64)
list(APPEND Matlab_LIBRARIES ${t_lib2})
find_library(t_lib3 eng /usr/local/MATLAB/R2014a/bin/glnxa64)
list(APPEND Matlab_LIBRARIES ${t_lib3})
message(">>> ${Matlab_LIBRARIES}")

add_library(c_mat STATIC ${CXX_CMAT_SRC})
target_link_libraries(c_mat ${Matlab_LIBRARIES})