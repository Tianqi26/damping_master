# damping_master
Add isotropic Damping from OpenFoam 8 to OpenFoam 6
## copy files from openfoam 8
src/fvOptions/cellSetOption
src/fvOptions/interRegionOption
src/fvOptions/sources/derived/damping/damping
src/fvOptions/sources/derived/damping/isotropicDamping
src/fvOptions/Make
## changes in the files

### 1. change 1 to 0, not output damping coefficient
defineTypeNameAndDebug(damping, 1); 
### 2. For version 6
change
dimensionedScalar(lambda_.dimensions(), scale_.valid() ? 0 : 1)
to
dimensionedScalar("zero",lambda_.dimensions(), scale_.valid() ? 0 : 1)
### 3. Make/files and Make/options
#### files
cellSetOption/cellSetOption.C
cellSetOption/cellSetOptionIO.C
interRegionOption/interRegionOption.C
interRegionOption/interRegionOptionIO.C
damping/damping.C
isotropicDamping/isotropicDamping.C
LIB = $(FOAM_USER_LIBBIN)/libfvOptionsDamping
#### make options
EXE_INC = \
    -I$(LIB_SRC)/finiteVolume/lnInclude \
    -I$(LIB_SRC)/meshTools/lnInclude \
    -I$(LIB_SRC)/sampling/lnInclude \
    -I$(LIB_SRC)/thermophysicalModels/solidThermo/lnInclude \
    -I$(LIB_SRC)/thermophysicalModels/basic/lnInclude \
    -I$(LIB_SRC)/thermophysicalModels/specie/lnInclude \

LIB_LIBS = \
    -lfiniteVolume \
    -lsampling \
    -lmeshTools \
    -lturbulenceModels \
    -lcompressibleTurbulenceModels
   
## usage
add fvOptions file to the constant
add 
libs
(
    
    "libfvOptionsDamping.so"

);
to system/controlDict file

