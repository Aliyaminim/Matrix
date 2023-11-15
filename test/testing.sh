#!/bin/bash

echo Testing has started!
echo 
echo "End-to-end tests:"
for file in e2e/*.in; do
    echo $file
    ../build/main < $file
    echo 
done