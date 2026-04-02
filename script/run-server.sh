#!/bin/bash

source script/setup-server.sh

./server/bin/app --help


###  COPY THIS  ###

# run this directly in terminal
# running from the script using bash/source,...
# the argument won't be recognized for some reason

./server/bin/app --host localhost \
         --port 8080 \
         --semantic at-most-once \
         --max-retry 3 \
         --error 50