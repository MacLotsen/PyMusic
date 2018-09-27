
__MIDINR_OFFSET__ = 21


class ScaleNotSetException(Exception):
    pass


class Interval:
    def __init__(self, index=None, scale=None, midinr=None, register=None):
        if not index and not scale:
            raise ScaleNotSetException("No index or scale passed to constructor.")

        if type(index) == Interval:
            self.index = index.index
        else:
            self.index = index
        self.scale = scale
        self.midinr = midinr
        self.register = register

        from pymusic.lang.scale.modern import ionian, aeolian
        self.major = ionian
        self.minor = aeolian

        if index and scale and register:
            midinr = 0
            for c in range(index - 1):
                midinr += scale[c]
            self.midinr = (__MIDINR_OFFSET__ + (12 * register)) + midinr
        elif scale and midinr and not index:
            normalized = (midinr - __MIDINR_OFFSET__) % 12
            index = val = 0
            while val < normalized:
                val += scale[index]
                index += 1
            self.index = index

    def is_perfect(self):
        if self.scale is None:
            raise ScaleNotSetException("Cannot determine if the interval is perfect if it hasn't a scale set.")

        i = 0
        step_sum = sum(self.scale[:self.index-1])
        while i <= step_sum:
            for r in [5, 2, 5]:
                if step_sum == i:
                    return True
                i += r
        return False

    def is_minor(self):
        if self.index not in [2, 3, 6, 7]:
            return False
        step_sum1 = sum(self.scale[:self.index-1])
        step_sum2 = sum(self.minor[:self.index-1])
        return step_sum1 == step_sum2

    def is_major(self):
        if self.index not in [2, 3, 6, 7]:
            return False
        step_sum1 = sum(self.scale[:self.index - 1])
        step_sum2 = sum(self.major[:self.index - 1])
        return step_sum1 == step_sum2

    def is_diminished(self):
        step_sum1 = sum(self.scale[:self.index - 1])
        step_sum2 = sum(self.minor[:self.index - 1]) - 1
        return step_sum1 == step_sum2

    def is_augmented(self):
        step_sum1 = sum(self.scale[:self.index - 1])
        step_sum2 = sum(self.major[:self.index - 1]) + 1
        return step_sum1 == step_sum2


def intervals(size=8):
    return [Interval(index=x) for x in range(1, size+1)]


primus, secundo, tertius, quartus, quinto, sexta, septimo, octava = intervals()
