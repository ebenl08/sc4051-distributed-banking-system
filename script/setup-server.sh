#!/bin/bash/


if [ ! -d "server/bin" ]; then
    mkdir server/bin
fi

g++ -I. -o server/bin/app server/*.cpp server/net/*.cpp

chmod +x server/bin/app
