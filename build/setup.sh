#! /bin/bash

conda activate main

export G4BASE=/Users/ajbiffl/Documents/Geant4-10/geant_install
export G4INSTALL=$G4BASE/share/Geant4/geant4make

source $G4BASE/bin/geant4.sh
source $G4INSTALL/geant4make.sh


# thing
# /opt/anaconda3/envs/main/etc/system.rootrc -- change Browser.Name
