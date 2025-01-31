#!/bin/bash

## JOB SPECIFICATIONS
#SBATCH --job-name=doublecap_prod
#SBATCH --time=04:00:00              #Set the wall clock limit to 1hr
#SBATCH --nodes=8                    #Request 1 node
#SBATCH --ntasks=4                   #Request 4 task
#SBATCH --mem=5G                    #Request 30GB per node
#SBATCH --output=/scratch/user/ajbiffl3/doublecap/logs/output.%j

# set up environment for download
cd $SCRATCH
export SINGULARITY_CACHEDIR=$TMPDIR/.singularity

SIMDIR="/scratch/user/ajbiffl3/doublecap"
LOGFILENAME="${SIMDIR}/logs/output.log"
IMAGE="/home/ajbiffl3/soft/images/g4_env.sif"

srun --output=${LOGFILENAME} singularity exec -B /home/ajbiffl3/soft $IMAGE source run_production_job.sh

