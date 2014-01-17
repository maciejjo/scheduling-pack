#!/bin/bash

  fn=$(basename $1)
  echo $fn
  f="${fn%.*}"

  gnuplot << EOF
  set terminal pdf enhanced
  set datafile separator ","
  set output "figures/${f}_smooth.pdf"
  set grid
  set xlabel "Liczba iteracji"
  set ylabel "Cmax"
  set style line 1 lt rgb "orange" lw 3
  plot \
        'plot_input/${f}.csv' using 1:2 title 'n = 5' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:3 title 'n = 10' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:4 title 'n = 20' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:5 title 'n = 40' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:6 ls 1 title 'OPT' smooth csplines with lines

  set output "figures/${f}.pdf"
  plot \
        'plot_input/${f}.csv' using 1:2 title 'n = 5' with lines, \
        'plot_input/${f}.csv' using 1:3 title 'n = 10' with lines, \
        'plot_input/${f}.csv' using 1:4 title 'n = 20' with lines, \
        'plot_input/${f}.csv' using 1:5 title 'n = 40' with lines, \
        'plot_input/${f}.csv' using 1:6 ls 1 title 'OPT' with lines
EOF
