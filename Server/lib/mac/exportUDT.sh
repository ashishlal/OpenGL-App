#!/bin/sh
echo "..Adding $1 to DYLD_LIBRARY_PATH.."
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$1
echo "DYLD_LIBRARY_PATH = $DYLD_LIBRARY_PATH"
source ~/.bash_profile
