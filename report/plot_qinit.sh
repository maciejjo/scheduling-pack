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
  plot \
        'plot_input/${f}.csv' using 1:2 title 'q_0 = 0.0' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:3 title 'q_0 = 0.25' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:4 title 'q_0 = 0.5' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:5 title 'q_0 = 0.75' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:6 title 'q_0 = 0.9' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:7 title 'OPT' smooth csplines with lines

  set output "figures/${f}.pdf"
  plot \
        'plot_input/${f}.csv' using 1:2 title 'q_0 = 0.0' with lines, \
        'plot_input/${f}.csv' using 1:3 title 'q_0 = 0.25' with lines, \
        'plot_input/${f}.csv' using 1:4 title 'q_0 = 0.5' with lines, \
        'plot_input/${f}.csv' using 1:5 title 'q_0 = 0.75' with lines, \
        'plot_input/${f}.csv' using 1:6 title 'q_0 = 0.9' with lines, \
        'plot_input/${f}.csv' using 1:7 title 'OPT' with lines
EOF
