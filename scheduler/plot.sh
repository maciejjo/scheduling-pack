#!/bin/bash

  f="${1%.*}"

  gnuplot << EOF
  set terminal post enh color
  set datafile separator ","
  set output "${f}_smooth.ps"
  set grid
  set xlabel "Liczba instancji"
  set ylabel "Cmax"
  plot \
        '${f}.csv' using 1:2 title 'ACO' smooth csplines with lines, \
        '${f}.csv' using 1:3 title 'RND' smooth csplines with lines

  set output "${f}.ps"
  plot \
        '${f}.csv' using 1:2 title 'ACO' with lines, \
        '${f}.csv' using 1:3 title 'RND' with lines
EOF
