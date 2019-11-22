import os
import subprocess
import sys

directory = os.getcwd();
directory = directory + "/metas"

os.system("mkdir -p test")
os.system("mkdir -p test/output")
#os.system("make simulator -s")


c = 0
#TestId , Instruction , Status , Author [, Message]
tests = []

#reads the metas and extracts test location, id, and retcodes
for file in os.listdir(directory):
     filename = os.fsdecode(file)
     if filename.endswith(".txt"):
         name = filename[:-4]
         f = open('metas/' + filename)
         all_lines = f.readlines()[0:7]
         all_lines[1] = all_lines[1][1:]
         testnumber = {
        "path":  "testbin/" + name + ".bin",
        "retcode": all_lines[2][1:].rstrip('\n'),
        "instruction": all_lines[1].rstrip('\n'),
        "testid": all_lines[0][1:].rstrip('\n'),
        "Author": all_lines[3][1:].rstrip('\n'),
        "Message": all_lines[4][1:].rstrip('\n'),
        "stdin": all_lines[5][1:].rstrip('\n'),
        "stdout": all_lines[5][1:].rstrip('\n'),
         }
         tests.append(testnumber)
         f.close()
         continue
     else:
         continue

c = 0
failed = False
testbench = open("test/output/mips_testbench_result.csv", "w")

print('TestId' , "," ,'instruction' , "," , 'Status' , ',' , 'Author' , "," , 'Message' '\n')

for test in tests:
    if len(sys.argv)==1:
        path = "bin/mips_simulator"
    else:
        path = sys.argv[1]    
    path = '.' + os.path.join('/', path)
    p = subprocess.run( [path, test['path']], stdout=subprocess.PIPE, input=test['stdin'].encode('utf-8'), timeout = 1000)
    
    if ((p.stdout.decode('utf-8') == test['stdout']) or str((p.returncode) == test['retcode'])):
        print(test['testid'] , " , " , test['instruction'] , " , " , "PASS",' , ', test['Author'], " , " , test['Message'])
        L = [test['testid'] , "," , test['instruction'] , "," , "PASS",',', test['Author'] , "," , test['Message'],'\n']
        testbench.writelines(L)
    else:
        failed = True
        c = c + 1
        print(test['testid'] , " , " , test['instruction'] , " , " , "FAIL",' , ', test['Author'], " , " , test['Message'])
        L = [test["testid"],",",test['instruction'],",", "FAIL",',', test["Author"], ",",test["Message"],'\n']
        testbench.write(L)

testbench.close()
if (failed):
    c = len(tets) - c
    k = str(c) +" of " + str(len(tests)) + " tests have passed\n"
    sys.exit(k)
else:
    sys.exit("Congradulations! All the tests have passed")    

