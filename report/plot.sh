#!/bin/bash

  fn=$(basename $1)
  echo $fn
  f="${fn%.*}"

  gnuplot << EOF
  set terminal pdf
  set datafile separator ","
  set output "figures/${f}_smooth.pdf"
  set grid
  set xlabel "Liczba iteracji"
  set ylabel "Cmax"
  plot \
        'plot_input/${f}.csv' using 1:2 title 'ACO' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:3 title 'OPT' smooth csplines with lines

  set output "figures/${f}.pdf"
  plot \
        'plot_input/${f}.csv' using 1:2 title 'ACO' with lines, \
        'plot_input/${f}.csv' using 1:3 title 'OPT' with lines
EOF
