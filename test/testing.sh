#!/bin/bash

echo Testing has started!
echo 
echo "End-to-end tests for Gauss algorithm:"
for file in e2e/*.in; do
    echo $file
    ../build/maind < $file
    echo 
done

echo "End-to-end tests for Bareiss algorithm:"
for file in e2e/test{1..4}.in e2e/test7.in e2e/test8.in e2e/test{11..12}.in e2e/test{14..17}.in; do
    echo $file
    ../build/mainll < $file
    echo 
done
