

def __down_hill__():

    def _down(lst, n):
        n2 = n / 2.0
        lst.append(n + n2)
        lst.append(n)
        if (n2 >= 1/32):
            _down(lst, n2)

    sets = []
    _down(sets, 8)
    return sets


r8d, r8, r4d, r4, r2d, r2, r1d, r1, r1_2d, r1_2, r1_4d, r1_4, r1_8d, r1_8, r1_16d, r1_16, r1_32d, r1_32 = __down_hill__()

figures = [
    (r1_4, r1_4),
    (r1_4d, r1_8),
    (r1_16, r1_8, r1_16, r1_8, r1_8)
]
