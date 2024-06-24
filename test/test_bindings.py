import numpy as np
import math
import predicates

def test_nearly_collinear():
    xs = np.array(
        [
            [1.0, 1.0, 1.0],
            [0.5, 12.0, 24.0],
            [0.5, 12.0, 24.0],
        ]
    )
    zs = np.copy(xs)

    num_steps = 128
    for row in range(num_steps):
        zs[1, 0] = xs[1, 0]
        zs[2, 0] = math.nextafter(zs[2, 0], math.inf)
        for col in range(num_steps):
            zs[1, 0] = math.nextafter(zs[1, 0], math.inf)
            result = predicates.sign_exact_determinant(zs)
            if row == col:
                assert result == 0.0
            elif row < col:
                assert result < 0.0
            else:
                assert result > 0.0
