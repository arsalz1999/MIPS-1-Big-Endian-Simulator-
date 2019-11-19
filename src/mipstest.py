# change code to take in simulator path as argument
# change code to print output as csv

import subprocess
import os
import sys

os.system("make simulator -s")


tests = [
    {
        'path': 'tb/test1.bin',
        'instruction': 'SW/LW',
        'retcode': 0, # expected retcode
        'stdin': 'w',
        'stdout': 'Aw' # expected stdout
    },
    {
        'path': 'tb/ADD-wrap.mips.bin',
        'instruction': 'ADD',
        'retcode': 246,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/ADDU-wrap.mips.bin',
        'instruction': 'ADDU',
        'retcode': 254,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/BNE-loop.mips.bin',
        'instruction': 'BNE',
        'retcode': 10,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/JR-nothing.mips.bin',
        'instruction': 'JR',
        'retcode': 0,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/JR-return-immediate-nodelay.mips.bin',
        'instruction': 'JR',
        'retcode': 10,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/JR-return-immediate.mips.bin',
        'instruction': 'JR',
        'retcode': 5,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/LW-getc.mips.bin',
        'instruction': 'LW',
        'retcode': 88,
        'stdin': 'X',
        'stdout': ''
    },
    {
        'path': 'tb/LW-read-data-section.mips.bin',
        'instruction': 'LW',
        'retcode': 0,
        'stdin': '',
        'stdout': ''
    },
    {
        'path': 'tb/SW-putc.mips.bin',
        'instruction': 'SW',
        'retcode': 0,
        'stdin': '',
        'stdout': 'X'
    },
    {
        'path': 'tb/SW-read-write-data-section.mips.bin',
        'instruction': 'SW',
        'retcode': 5,
        'stdin': '',
        'stdout': ''
    }
]

failed = False

print('#\tinstr\tstatus')
for index, test in enumerate(tests):
    p = subprocess.run(['./bin/mips_simulator', test['path']], stdout=subprocess.PIPE, input=test['stdin'].encode('utf-8')) # not sure if stdin is supposed to be encoded as ASCII or UTF-8

    if ((p.stdout.decode('utf-8') == test['stdout']) and (p.returncode == test['retcode'])):
        print('{}\t{}  \tPass'.format(index, test['instruction']))
    else:
        failed = True
        print('{}\t{}  \tFailed'.format(index, test['instruction']))
        if (p.stdout.decode('utf-8') != test['stdout']):
            print('\t\t\tExpected stdout {} but got {}'.format(test['stdout'], p.stdout.decode('utf-8')))
        if (p.returncode != test['retcode']):
            print('\t\t\tExpected retcode {} but got {}'.format(test['retcode'], p.returncode))

if (failed):
    sys.exit("Error: At least 1 test failed.")
