#pragma once

#include <grpc++/grpc++.h>
#include "arduino_protocol.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class ArduinoServiceBasicImpl : public shammam::Arduino::Service {
    Status GetPinState(
        ServerContext* context,
        const shammam::Pin* request,
        shammam::State* reply);

    Status SetPinState(
        ServerContext* context,
        const shammam::PinAndState* request,
        shammam::State* reply);

    Status ResetPins(
        ServerContext* context,
        const shammam::Empty* request,
        shammam::Empty* reply);
};

ArduinoServiceBasicImpl* __get_rpc_service();

void runRPC(string addr);
