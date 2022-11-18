import os
import math
import numpy as np
import matplotlib.pylab as mplt

R1 = 3.4
R2 = 5.2
R3 = 360
R4 = 60
C1 = 0.24
C2 = 0.26
I_MIN = 1
I_MAX = 2
L2 = 0.42
L_MIN = 1.3
L_MAX = 13
T = 0.000001
U1_A = 10
U1_T = 0.009
H_S3 = (I_MAX - I_MIN)

# x = [i1, i5, UC1]
equations = [
    lambda x, t: (u1(t) - (x[0] * R1) - x[2]) / l1(x[0]),
    lambda x, t: (((x[0] - ((u1(t) - (x[0] * R1) - (x[0] * l1(x[0]))) * C1) - x[1]) * R3) - (x[1] * R2) - (x[1] * R4)) / L2,
    lambda x, t: u1(t) - (x[0] * R1) - (x[0] * l1(t))
]

s3_coeff = [
    lambda x: (2 * (x - I_MIN) + H_S3) * ((I_MAX - x) ** 2),
    lambda x: (2 * (I_MAX - x) + H_S3) * ((x - I_MIN) ** 2),
    lambda x: (x - I_MIN) * (I_MAX - x) ** 2,
    lambda x: (x - I_MAX) * (I_MIN - x) ** 2
]


def u1(t):
    return 0 if t % U1_T > 0 and t % (2*U1_T) > U1_T else U1_A


def s3(i1):
    return (s3_coeff[0](i1) * L_MAX + s3_coeff[1](i1) * L_MIN) / (H_S3 ** 3) + (s3_coeff[2](i1) * 0 + s3_coeff[3](i1) * 0) / H_S3 ** 2


def l1(i1):
    if abs(i1) <= I_MIN: return L_MAX
    if abs(i1) >= I_MAX: return L_MIN
    return s3(i1)


def calculate_coefficients(prev, t):
    coeff = [[0 for _ in range(len(prev))] for _ in range(4)]

    # K1
    for i in range(len(prev)):
        coeff[0][i] = T * equations[i](prev, t)

    # K2
    for i in range(len(prev)):
        coeff[1][i] = T * equations[i]([prev[x] + coeff[0][x] / 2 for x in range(len(prev))], t + T / 2)

    # K3
    for i in range(len(prev)):
        coeff[2][i] = T * equations[i]([prev[x] + coeff[1][x] / 2 for x in range(len(prev))], t + T / 2)

    # K4
    for i in range(len(prev)):
        coeff[3][i] = T * equations[i]([prev[x] + coeff[2][x] for x in range(len(prev))], t + T)

    return coeff


def runge_kutta(prev, t):
    coeff = calculate_coefficients(prev, t)
    args = []
    for j in range(len(prev)):
        k = [coeff[i][j] for i in range(len(coeff))]
        args.append(prev[j] + calculate_y_coefficient(k))

    return args


def calculate_y_coefficient(k):
    return (k[0] + 2 * (k[1] + k[2]) + k[3]) / 6


def build_graph(x_axis, y_axis, y_name, x_name='t'):
    mplt.plot(x_axis, y_axis)
    mplt.ylabel(y_name)
    mplt.xlabel(x_name)
    mplt.savefig(f'graphs/{y_name}.png')
    mplt.clf()


def main():
    if not os.path.isdir('graphs'):
        os.mkdir('graphs')

    times = np.arange(0, 0.2, T)

    x = [0, 0, 0]
    result = {'i1': [], 'i5': [], 'U2': []}
    t = 0

    for i in range(len(times)):
        result['i1'].append(x[0])
        result['i5'].append(x[1])
        result['U2'].append(x[2])
        x = runge_kutta(x, t)
        t += T

    build_graph(times, [u1(x) for x in np.arange(0, 0.2, T)], 'U1')

    for x in result.keys():
        build_graph(times, result[x], x)

    build_graph(times, [x * R4 for x in result['i5']], 'u_2')

    build_graph(np.arange(0, 3, 0.001), [l1(i) for i in np.arange(0, 3, 0.001)], "L1", "i")


if __name__ == '__main__':
    main()
