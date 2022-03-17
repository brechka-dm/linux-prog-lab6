#!/bin/bash
for (( i=1; i <= 10; i++ ))
do
  ./task2-1-async&
  ./task2-2-async&
  ./task2-3-async&
done