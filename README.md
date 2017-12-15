# analysis-main 

This c++ library is used to perform analysis of CLAS (E1-F) data, and depends on the CERN ROOT libraries.  It is still under active development. Although all are welcome to use this library, it was not designed with other users in mind.  The collection of routines is largely written by myself, but in some places I have taken code from previous students and scientists.

- Nathan Harrison - I inherited all of his code and used some pieces of particle identification, as well as timing corrections.  His thesis can be [here] (https://www.jlab.org/Hall-B/general/thesis/Harrison_thesis.pdf).
- Marco Mirazita - I used momentum corrections developed by Marco.  A small note can be provided with details upon request.
- All other contributions - There are inline comments which detail contributions from other physicists, but I am sure I have missed some.  If you feel that I have wrongfully used your code in this repo please contact me directly at david.riser@uconn.edu.        

## Installation 
This set of classes was checked with **ROOT 5.34.36** and compiled with **g++ 5.3.0** (c++11).  This project uses the build system SConstruct, install that before running the included install script.

- Install root and SConstruct 
- Clone the repository 
- In the top directory, run the install script with `source install.csh` (default shell on ifarm).

## Use of Classes 
Most of the `main` routines which use these base classes have been moved to [this projects repo] (https://github.com/dmriser/analysis-projects.git).  

## To Do
- Add a simple test file to this repo for users to run after installation.
- Add a `Configure(Parameters *)` method to all derived classes from the base class `DataEventCut`
- Cleanup particle filter 
  - Convert stray selectors into a map indexed by the pid




