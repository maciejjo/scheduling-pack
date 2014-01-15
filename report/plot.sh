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
        'plot_input/${f}.csv' using 1:2 title '{/Symbol b} = 0.0' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:2 title '{/Symbol b} = 2.0' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:2 title '{/Symbol b} = 4.0' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:2 title '{/Symbol b} = 6.0' smooth csplines with lines, \

  set output "figures/${f}.pdf"
  plot \
        'plot_input/${f}.csv' using 1:2 title 'ACO' with lines, \
        'plot_input/${f}.csv' using 1:3 title 'RND' with lines, \
        'plot_input/${f}.csv' using 1:4 title 'OPT' with lines
EOF
