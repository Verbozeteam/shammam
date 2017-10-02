import argparse
import os, sys

# Parse command line arguments
parser = argparse.ArgumentParser(description='Ardunio test suite')
parser.add_argument('-s', '--source', nargs='+', help='<Required> List of source files (comma separated)', required=True)
parser.add_argument('-i', '--include', nargs='+', help='List of directories to look for header files (comma separated)')
cmd_args = parser.parse_args()

os.system("mkdir -p tmp")
os.system("rm -rf inc/*.gch tmp/*")

for F in cmd_args.source:
    filename = F
    if "/" in F:
        filename = F[F.rindex("/")+1:]
    os.system("cp {} tmp/{}.cpp".format(F, filename))

CPPFLAGS = "-std=c++11"
BASE_INCLUDE_DIR = "inc"
BASE_SRC_DIR = "src"
INCLUDE_DIRS = "-I{} -Itmp -Iprotocol".format(BASE_INCLUDE_DIR)
SOURCE_FILES = "{}/*.cpp tmp/*.cpp tmp/*.cc".format(BASE_SRC_DIR)
LDFLAGS = "-L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`"
OUT_FILE = "shammam"

if cmd_args.include != None:
    INCLUDE_DIRS += " " + " ".join(map(lambda i: "-I{}".format(i), cmd_args.include))

protoc_command = "protoc --cpp_out=tmp --grpc_out=tmp arduino_protocol.proto --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin)"
gcc_cmd = "g++ {} {} {} -include {}/Arduino.h {} -o {}".format(CPPFLAGS, LDFLAGS, INCLUDE_DIRS, BASE_INCLUDE_DIR, SOURCE_FILES, OUT_FILE)

if not os.system(protoc_command): # compile the protocol
    if not os.system(gcc_cmd):
        os.system("rm -rf tmp/")
        os.system("./" + OUT_FILE)

