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
        'plot_input/${f}.csv' using 1:2 title '{/Symbol a} = 0.0' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:3 title '{/Symbol a} = 0.1' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:4 title '{/Symbol a} = 0.25' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:5 title '{/Symbol a} = 0.5' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:6 title '{/Symbol a} = 0.75' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:7 ls 1 title 'OPT' smooth csplines with lines

  set output "figures/${f}.pdf"
  plot \
        'plot_input/${f}.csv' using 1:2 title '{/Symbol a} = 0.0' with lines, \
        'plot_input/${f}.csv' using 1:3 title '{/Symbol a} = 0.1' with lines, \
        'plot_input/${f}.csv' using 1:4 title '{/Symbol a} = 0.25' with lines, \
        'plot_input/${f}.csv' using 1:5 title '{/Symbol a} = 0.5' with lines, \
        'plot_input/${f}.csv' using 1:6 title '{/Symbol a} = 0.75' with lines, \
        'plot_input/${f}.csv' using 1:7 ls 1 title 'OPT' with lines
EOF
