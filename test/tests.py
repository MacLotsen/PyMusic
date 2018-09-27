import unittest

from test import interval, parser, scale, score, util


def load_tests(loader, tests, pattern):
    suite = unittest.TestSuite()
    suite.addTests(loader.loadTestsFromModule(interval))
    suite.addTests(loader.loadTestsFromModule(parser))
    suite.addTests(loader.loadTestsFromModule(scale))
    suite.addTests(loader.loadTestsFromModule(score))
    suite.addTests(loader.loadTestsFromModule(util))

    return suite
