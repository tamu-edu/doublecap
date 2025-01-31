#!/bin/bash

source /home/ajbiffl3/soft/doublecap/build/singularity_setup.sh

export BUILDDIR="/home/ajbiffl3/soft/doublecap/build"

INDEX=$(printf "%04d" $1)

${BUILDDIR}/doublecap ${BUILDDIR}/prod.mac $INDEX

