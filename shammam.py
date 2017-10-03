import argparse
import os, sys

# Parse command line arguments
parser = argparse.ArgumentParser(description='Ardunio test suite')
parser.add_argument('-s', '--source', nargs='+', help='<Required> List of source files (comma separated)', required=True)
parser.add_argument('-i', '--include', nargs='+', help='List of directories to look for header files (comma separated)')
parser.add_argument('-p', '--protocol', help='Custom protocol for the emulation RPC server')
parser.add_argument('-pt', '--port', default=5001, help='Port to host the emulator RPC on (default is 5001)')
parser.add_argument('-spt', '--serial_port', default=9911, help='Port to host the serial communication on (default is 991)')
parser.add_argument('-n', '--name', default='shammam', help='Name of the emulator instance (an executable with that name will be generated) (default is "shammam")')
cmd_args = parser.parse_args()

tmp_folder = ".{}_tmp".format(cmd_args.name)

os.system("mkdir -p {}".format(tmp_folder))
os.system("rm -rf inc/*.gch {}/*".format(tmp_folder))

for F in cmd_args.source:
    filename = F
    if filename.strip() == "":
        continue
    if "/" in F:
        filename = F[F.rindex("/")+1:]
    if os.system("cp {} {}/{}.cpp".format(F, tmp_folder, filename)):
        print ("File not found: {}".format(filename))
        exit(1)

INSTALL_DIR = "/Users/hasan/verboze/shammam"
CPPFLAGS = "-std=c++11"
BASE_INCLUDE_DIR = "{}/inc".format(INSTALL_DIR)
BASE_SRC_DIR = "{}/src".format(INSTALL_DIR)
INCLUDE_DIRS = "-I{} -I{} -Iprotocol".format(BASE_INCLUDE_DIR, tmp_folder)
SOURCE_FILES = "{}/*.cpp {}/*.cpp {}/*.cc".format(BASE_SRC_DIR, tmp_folder, tmp_folder)
LDFLAGS = "-L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`"
OUT_FILE = cmd_args.name
RPC_PORT = cmd_args.port
SERIAL_PORT = cmd_args.serial_port
PROTOCOL_FILENAME = "arduino_protocol.proto"

if cmd_args.include != None:
    INCLUDE_DIRS += " " + " ".join(map(lambda i: "-I{}".format(i), cmd_args.include))

if cmd_args.protocol and cmd_args.protocol != "":
    if os.system("cp {} {}/{}".format(cmd_args.protocol, tmp_folder, PROTOCOL_FILENAME)):
        print ("File not found: {}".format(cmd_args.protocol))
        exit(1)
    CPPFLAGS += " -D _CUSTOM_RPC_" # tell the code that there is a custom RPC declaration
else:
    if os.system("cp {} {}/{}".format(PROTOCOL_FILENAME, tmp_folder, PROTOCOL_FILENAME)):
        print ("File not found: {}".format(PROTOCOL_FILENAME))
        exit(1)

protoc_command = "protoc --cpp_out={} --grpc_out={} -I{} {} --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin)".format(tmp_folder, tmp_folder, tmp_folder, PROTOCOL_FILENAME)
gcc_cmd = "g++ {} {} {} -include {}/Arduino.h {} -o {}".format(CPPFLAGS, LDFLAGS, INCLUDE_DIRS, BASE_INCLUDE_DIR, SOURCE_FILES, OUT_FILE)

succeeded = False
if not os.system(protoc_command): # compile the protocol
    if not os.system(gcc_cmd):
        succeeded = True

os.system("rm -rf {}/".format(tmp_folder))
if succeeded:
    os.system("./{} {} {}".format(OUT_FILE, RPC_PORT, SERIAL_PORT))

