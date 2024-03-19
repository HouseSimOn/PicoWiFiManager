#!/bin/sh

cd Source

# Do not use makefsdata from lwip/src/apps/http/makefsdata, because of bug - https are not supported
# MAKEFSDATA_PATH="./../../Sdk/pico-sdk/lib/lwip/src/apps/http/makefsdata/makefsdata"

# Use local copy instead with fixed bug
MAKEFSDATA_PATH="./../makefsdata"

perl $MAKEFSDATA_PATH

mv fsdata.c ./../GeneratedFsData.c

cd ..
