//New Readme - See old readme below

The code is fully functional and runs on Orbit. To compile, you only need sqrt.cpp, sqrt.ispc, tasksys.cpp, and the Makefile.

When in the directory that contains these, run "make"

Make requires the following compilers to be installed: ISPC, clang++-3.5

Once make is run, execute with ./sqrt





//OLD README

We ran into complications with getting sudo access on orbit, so we were not allowed to install much. 
We tried out best to transfer over binaries to install ispc and clang, but the compilation just doesn't work on orbit.

It worked on our macbook, so we ran the tests on there.

The results and graphs are in our report.

The ISPC implementations works correctly on the macbook, and we do get speedup for up to 4 cores, which is great!
However we could not get it to run on Orbit.

We believe the AVX implementations works according to the logic, but we were not able to run it because none of our cpus of our laptops can run AVX.

We tried installing ISPC on CentOS on the desktops in the EE lab, but we do not have root access there either, so there was no luck.

