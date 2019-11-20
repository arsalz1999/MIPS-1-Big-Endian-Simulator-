# change code to take in simulator path as argument
# change code to print output as csv

import subprocess
import os
import sys

os.system("make simulator -s")


tests = [
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
c = 0
failed = False
file1 = open("test_results.txt", "w")

print('#\tinstr\tstatus')
for index, test in enumerate(tests):
    p = subprocess.run(['./bin/mips_simulator', test['path']], stdout=subprocess.PIPE, input=test['stdin'].encode('utf-8'))

    if ((p.stdout.decode('utf-8') == test['stdout']) or (p.returncode == test['retcode'])):
        print('{}\t{}  \tPass'.format(index, test['instruction']))
        L = [str(index),"\t", test['instruction'],"\t", "|||||\n"]
        file1.writelines(L)
    else:
        failed = True
        c = c + 1
        print('{}\t{}  \tFailed'.format(index, test['instruction']))
        L = [str(index),"\t", test['instruction'],"\t", "-----\n"]
        file1.writelines(L)
        if (p.stdout.decode('utf-8') != test['stdout']):
            print('\t\t\tExpected stdout {} but got {}'.format(test['stdout'], p.stdout.decode('utf-8')))
        if (p.returncode != test['retcode']):
            print('\t\t\tExpected retcode {} but got {}'.format(test['retcode'], p.returncode))
file1.close()
if (failed):
    c = 10 - c
    k = str(c) +" of 10 tests have passed\n"
    print(k)
    sys.exit("Some tests have failed")
