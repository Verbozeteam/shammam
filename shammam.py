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
    if os.system("cp {} tmp/{}.cpp".format(F, filename)):
        print ("File not found: {}".format(filename))
        exit(1)

INSTALL_DIR = "/Users/hasan/verboze/shammam"
CPPFLAGS = "-std=c++11"
BASE_INCLUDE_DIR = "{}/inc".format(INSTALL_DIR)
BASE_SRC_DIR = "{}/src".format(INSTALL_DIR)
INCLUDE_DIRS = "-I{} -Itmp -Iprotocol".format(BASE_INCLUDE_DIR)
SOURCE_FILES = "{}/*.cpp tmp/*.cpp tmp/*.cc".format(BASE_SRC_DIR)
LDFLAGS = "-L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`"
OUT_FILE = "shammam"
PROTOCOL_PATH = "-I{}".format(INSTALL_DIR)
PROTOCOL_FILENAME = "arduino_protocol.proto".format(INSTALL_DIR)

if cmd_args.include != None:
    INCLUDE_DIRS += " " + " ".join(map(lambda i: "-I{}".format(i), cmd_args.include))

protoc_command = "protoc --cpp_out=tmp --grpc_out=tmp {} {} --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin)".format(PROTOCOL_PATH, PROTOCOL_FILENAME)
gcc_cmd = "g++ {} {} {} -include {}/Arduino.h {} -o {}".format(CPPFLAGS, LDFLAGS, INCLUDE_DIRS, BASE_INCLUDE_DIR, SOURCE_FILES, OUT_FILE)

succeeded = False
if not os.system(protoc_command): # compile the protocol
    if not os.system(gcc_cmd):
        succeeded = True

os.system("rm -rf tmp/")
if succeeded:
    os.system("./" + OUT_FILE)

