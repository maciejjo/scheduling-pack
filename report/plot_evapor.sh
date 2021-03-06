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
        'plot_input/${f}.csv' using 1:2 title '{/Symbol r} = 0.0' smooth csplines with lines, \
        'plot_input/${f}_2.csv' using 1:2 title '{/Symbol r} = 0.005' smooth csplines with lines, \
        'plot_input/${f}_3.csv' using 1:2 title '{/Symbol r} = 0.001' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:3 title '{/Symbol r} = 0.01' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:4 title '{/Symbol r} = 0.02' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:5 title '{/Symbol r} = 0.05' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:6 title '{/Symbol r} = 0.1' smooth csplines with lines, \
        'plot_input/${f}.csv' using 1:7 title 'OPT' smooth csplines with lines

  set output "figures/${f}.pdf"
  plot \
        'plot_input/${f}.csv' using 1:2 title '{/Symbol r} = 0.0' with lines, \
        'plot_input/${f}_2.csv' using 1:2 title '{/Symbol r} = 0.005' with lines, \
        'plot_input/${f}_3.csv' using 1:2 title '{/Symbol r} = 0.001' with lines, \
        'plot_input/${f}.csv' using 1:3 title '{/Symbol r} = 0.01' with lines, \
        'plot_input/${f}.csv' using 1:4 title '{/Symbol r} = 0.02' with lines, \
        'plot_input/${f}.csv' using 1:5 title '{/Symbol r} = 0.05' with lines, \
        'plot_input/${f}.csv' using 1:6 title '{/Symbol r} = 0.1' with lines, \
        'plot_input/${f}.csv' using 1:7 title 'OPT' with lines
EOF
