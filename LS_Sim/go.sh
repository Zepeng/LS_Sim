#!/bin/bash 
usage(){ cat << EOU

~/opticks/examples/UseSysRap/go.sh

EOU
}


cd $(dirname $BASH_SOURCE)
cvd=0
export CUDA_VISIBLE_DEVICES=${CVD:-$cvd}
export CVDLabel="CVD${CUDA_VISIBLE_DEVICES}"
sdir=$(pwd)
bdir=/tmp/$USER/opticks/$(basename $sdir)/build 

rm -rf $bdir && mkdir -p $bdir && cd $bdir && pwd 

vars="0 BASH_SOURCE sdir bdir OPTICKS_PREFIX"
for var in $vars ; do printf "%20s : %s\n" "$var" "${!var}" ; done

cmake $sdir \
     -DCMAKE_BUILD_TYPE=Debug \
     -DCMAKE_INSTALL_PREFIX=$OPTICKS_PREFIX \
     -DCMAKE_PREFIX_PATH=$OPTICKS_PREFIX/externals \
     -DCMAKE_MODULE_PATH=$OPTICKS_PREFIX/cmake/Modules \
     -DOPTICKS_PREFIX=$OPTICKS_PREFIX \
     -DWITH_G4CXOPTICKS=ON \
     -DCMAKE_INSTALL_PREFIX=$PWD/../install \

make
make install   
