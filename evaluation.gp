#!/usr/bin/gnuplot -p

f(x) = m*x+n
fit f(x) "/tmp/output.txt" using 1:4 via m,n
plot "/tmp/output.txt" using 1:4, f(x)
