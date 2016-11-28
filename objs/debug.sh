#!/bin/bash

projectDir=`pwd`
projectDir=$projectDir/../

rm -rf ${projectDir}server/chat
rm -rf ${projectDir}client/client
gcc -g -std=c99 ${projectDir}/server/chat.c -o server
gcc -g -std=c99 ${projectDir}client/client.c -o client
