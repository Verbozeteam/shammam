#include <Arduino.h>
#include <thread>
#include <iostream>

#include <grpc++/grpc++.h>
#include "arduino_protocol.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Logic and data behind the RPC server's behavior.
class ArduinoServiceImpl : public shammam::Arduino::Service {
        Status GetPinState(
            ServerContext* context,
            const shammam::Pin* request,
            shammam::State* reply) override {

            std::cout << "GetPinState()\n";

            reply->set_state(__get_pin_state(request->type(), request->index()));
            return Status::OK;
        }

        Status SetPinState(
            ServerContext* context,
            const shammam::PinAndState* request,
            shammam::State* reply) override {

            std::cout << "SetPinState()\n";

            __set_pin_state(request->type(), request->index(), request->state());
            reply->set_state(__get_pin_state(request->type(), request->index()));
            return Status::OK;
        }

        Status ResetPins(
            ServerContext* context,
            const shammam::Empty* request,
            shammam::Empty* reply) override {

            std::cout << "ResetPins()\n";

            __reset_pins();
            return Status::OK;
        }
};

void rpc_thread(string addr) {
    ArduinoServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << addr << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

void runRPC(string addr) {
    static std::thread t (rpc_thread, addr);
}
