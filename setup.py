from setuptools import setup

setup(
    name='pymusic',
    version='0.1',
    packages={'pymusic', 'test'},
    install_requires=['python-rtmidi', 'mido'],
    entry_points={
        'console_scripts': [
            'pymusic = pymusic.main'
        ]
    },
    test_suite='test.tests'
)
