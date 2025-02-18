#!/bin/bash

source /home/ajbiffl3/soft/doublecap/build/singularity_setup.sh

export BUILDDIR="/home/ajbiffl3/soft/doublecap/build"
export OUTPUTDIR="/scratch/user/ajbiffl3/doublecap/data/"

INDEX=$(printf "%04d" $1)
PB=$2
PE=$3
KILLGAMMAS=$4
NTHREADS=$5

${BUILDDIR}/doublecap prod.mac -s 1 -r 1 -k $KILLGAMMAS -n $NTHREADS -m $BUILDDIR -i $INDEX -o $OUTPUTDIR -l $PB -p $PE -n $NTHREADS

