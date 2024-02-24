# predicates

This is my attempt at implementing sign-exact geometric predicates -- specifically, the signed volume of a simplex.
Jonathan Richard Shewchuk figured out how to do this in the 90s and everyone has been using [his code](https://www.cs.cmu.edu/~quake/robust.html) since.
JRS's predicates code is ingenious; the paper that accompanies it is 50 pages long and the approach required a pretty deep understanding of floating-point arithmetic.
I'm not that smart, so this package implements sign-exact geometric predicates in the most blunt way possible:
1. Evaluate the matrix determinant using interval arithmetic.
If the resulting interval does not contain 0, we're done -- return the midpoint.
2. If the resulting interval does contain 0, convert all the matrix entries to rational numbers exactly.
Evaluate the matrix determinant using bignum rational arithmetic.
Convert the result back to floating-point.

I used Eigen for linear algebra and boost for both interval and bignum rational arithmetic.
I suspect this code is slower than JRS's predicates.
