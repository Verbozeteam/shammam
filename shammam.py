import argparse
import os, sys

# Parse command line arguments
parser = argparse.ArgumentParser(description='Ardunio test suite')
parser.add_argument('-s', '--source', nargs='+', help='<Required> List of source files (comma separated)', required=True)
parser.add_argument('-i', '--include', nargs='+', help='List of directories to look for header files (comma separated)')
cmd_args = parser.parse_args()

for F in cmd_args.source:
    filename = F
    if "/" in F:
        filename = F[F.rindex("/")+1:]
    os.system("cp {} tmp/{}.cpp".format(F, filename))

include_directories = " ".join(map(lambda i: "-I{}".format(i), cmd_args.include)) if cmd_args.include != None else ""

os.system("mkdir -p tmp")
os.system("rm -rf inc/*.gch")
gcc_cmd = "g++ -include inc/Arduino.h -Iinc {} src/*.cpp tmp/*.cpp -o tmp/shammam".format(include_directories)
print (gcc_cmd)
if not os.system(gcc_cmd):
    os.system("./tmp/shammam")

