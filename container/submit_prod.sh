#!/bin/bash

## JOB SPECIFICATIONS
#SBATCH --job-name=doublecap_prod
#SBATCH --time=02:00:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=50G
#SBATCH --array=1-1%1
#SBATCH --output=/scratch/user/ajbiffl3/doublecap/logs/slurm_output_%a.%j


# set up environment for download
cd $SCRATCH
export SINGULARITY_CACHEDIR=$TMPDIR/.singularity

SIMDIR="/scratch/user/ajbiffl3/doublecap"
LOGFILENAME="${SIMDIR}/logs/output_${SLURM_ARRAY_TASK_ID}.log"
IMAGE="/home/ajbiffl3/soft/images/g4_env.sif"

srun --output=${LOGFILENAME} singularity exec -B /home/ajbiffl3/soft,/scratch/user/ajbiffl3 $IMAGE bash /home/ajbiffl3/soft/doublecap/build/run_production_job.sh ${SLURM_ARRAY_TASK_ID}

