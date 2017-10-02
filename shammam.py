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

CPPFLAGS = "-std=c++11"
LDFLAGS = "-L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`"
OUT_FILE = "tmp/shammam"

protoc_command = "protoc --cpp_out=protocol --grpc_out=protocol arduino_protocol.proto --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin)"
gcc_cmd = "g++ {} {} -include inc/Arduino.h -Iinc -Iprotocol {} protocol/*.cc src/*.cpp tmp/*.cpp -o {}".format(CPPFLAGS, LDFLAGS, include_directories, OUT_FILE)

os.system("mkdir -p tmp")
os.system("rm -rf inc/*.gch")
if not os.system(protoc_command): # compile the protocol
    if not os.system(gcc_cmd):
        os.system(OUT_FILE)

