#!/usr/local/bin/python3.6

import sys, os, subprocess

if len(sys.argv) < 2:
    print("usage:  %s studentid  # will create dir using studentid.tar" % (sys.argv[0]) )
    sys.exit(-1)

studentid = sys.argv[1]
if studentid.endswith(".tar"):
    studentid = studentid[0:-4]

PUNTAR = "~rbutler/public/courses/cs/PUNTAR.py"
(status,output) = subprocess.getstatusoutput("%s %s" % (PUNTAR,studentid) )
print(output,"\n\n----\n")

if status != 0:
    print("\n%s FAILED to untar correctly\n" %(studentid) )
    exit(status)

os.chdir(studentid)
os.system("rm -rf  %s" % (studentid) )
os.system("rm -rf  p2")
os.system("rm -f *.o")

print("BEGIN MAKE")
os.system("cp ~rbutler/public/courses/cs/p2testfiles/p2test1.c p2test.c")
os.system("make")
print("END MAKE\n")

(status,output) = subprocess.getstatusoutput("./p2 -hw")
print(output,"\n")
if "hello" not in output  or  "world" not in output:
    print("\n%s FAILED to print  hello world\n" %(studentid) )
    exit(status)

print("%s  SUCCESS" %(studentid) )

exit(0)
