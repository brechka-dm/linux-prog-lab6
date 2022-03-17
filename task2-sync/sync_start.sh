#!/bin/bash
for (( i=1; i <= 10; i++ ))
do
  ./task2-1-sync&
  ./task2-2-sync&
  ./task2-3-sync&
done