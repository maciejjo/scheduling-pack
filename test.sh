#!/bin/bash

for i in instances/inst*;
do
  f=$(basename $i);
  echo "./scheduler/scheduler -i $i -o report/plot_input/${f}.csv";
  ./scheduler/scheduler -i $i -o report/plot_input/${f}.csv;
done
