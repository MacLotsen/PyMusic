

def __down_hill__(minimum=1.0 / 32.0, maximum=8.0):
    def _down(lst, weight):
        half_weight = weight / 2.0
        lst.append(weight + half_weight)
        lst.append(weight)
        if half_weight >= minimum:
            _down(lst, half_weight)

    sets = []
    _down(sets, maximum)
    return sets


r8d, r8, r4d, r4, \
r2d, r2, r1d, r1, \
r1_2d, r1_2, r1_4d, r1_4, \
r1_8d, r1_8, r1_16d, r1_16, \
r1_32d, r1_32 = __down_hill__()
