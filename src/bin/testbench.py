import os
import subprocess
import sys

directory = os.getcwd();
directory = directory + "/metas"
print(directory)
#os.system("make simulator -s")

c = 0
tests = []

#reads the metas and extracts test location, id, and retcodes
for file in os.listdir(directory):
     filename = os.fsdecode(file)
     print(filename)
     if filename.endswith(".meta"):
         name = filename[:-5]
         f = open('metas/' + filename)
         all_lines = f.readlines()
         all_lines[1] = all_lines[1][1:]
         testnumber = {
        "path":  "testbin/" + name + ".bin",
        "retcode": all_lines[2][1:].rstrip('\n'),
        "instruction": all_lines[1].rstrip('\n'),
         }
         tests.append(testnumber)
         f.close()
         continue
     else:
         continue

print(tests)

c = 0
failed = False
file1 = open("test_results_mybench.txt", "w")

print('#\tinstr\tstatus')
for index, test in enumerate(tests):
    p = subprocess.run(['./mips_simulator', test['path']])

    if (str((p.returncode) == test['retcode'])):
        print('{}\t{}  \tPass'.format(index, test['instruction']))
        L = [str(index),"\t", test['instruction'],"\t", "|||||\n"]
        file1.writelines(L)
    else:
        failed = True
        c = c + 1
        print('{}\t{}  \tFailed'.format(index, test['instruction']))
        L = [str(index),"\t", test['instruction'],"\t", "-----\n"]
        file1.writelines(L)
        #if (p.stdout.decode('utf-8') != test['stdout']):
        #    print('\t\t\tExpected stdout {} but got {}'.format(test['stdout'], p.stdout.decode('utf-8')))
        if (p.returncode != test['retcode']):
            print('\t\t\tExpected retcode {} but got {}'.format(test['retcode'], p.returncode))

    #p.wait(timeout =400)
    #p.terminate()

file1.close()
if (failed):
    c = len(tets) - c
    k = str(c) +" of " + str(len(tests)) + " tests have passed\n"
    print(k)
    sys.exit("Some tests have failed")

