import numpy as np
import predicates

def test_nearly_collinear():
    xs = np.array(
        [
            [0.5, 12.0, 24.0],
            [0.5, 12.0, 24.0],
        ]
    )
    zs = np.copy(xs)

    num_steps = 128
    for row in range(num_steps):
        zs[0, 0] = xs[0, 0]
        zs[1, 0] = np.nextafter(zs[1, 0], np.inf)
        for col in range(num_steps):
            zs[0, 0] = np.nextafter(zs[0, 0], np.inf)
            result = predicates.volume(zs)
            if row == col:
                assert result == 0.0
            elif row < col:
                assert result < 0.0
            else:
                assert result > 0.0


def test_nearly_cocircular():
    xs = np.array(
        [
            [-1.0, 1.0, 1.0, -1.0],
            [-1.0, -1.0, 1.0, 1.0],
        ]
    )
    zs = np.copy(xs)

    num_steps = 128
    for row in range(num_steps):
        zs[0, 0] = xs[0, 0]
        zs[1, 0] = np.nextafter(zs[1, 0], np.inf)
        for col in range(num_steps):
            zs[0, 0] = np.nextafter(zs[0, 0], np.inf)
            result = predicates.insphere(zs)
            # TODO: How do we actually test this...
