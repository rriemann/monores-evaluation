# Purpose of this project

This project solves the transport equation d_t u + v d_x u = 0.
The time evolution is realised by a explicit euler approach using the closed
form of the Lax-Wendroff method [^1].

The evolution is done with different grid sizes to examine the discretization error.
The results are written to /tmp/output.txt (hardcoded).

[^1]: http://www.exp.univie.ac.at/cp1/cp1-6/node72.html

# Results

Using the gnuplot script `evaluation.gp` a linear regression fit is done to
determine the accurate error decrease.

While one doubles the grid points, the error is reduced by exp(-0.3452(4)) which
is approximately 0.7.