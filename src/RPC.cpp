#include <Arduino.h>
#include <RPC.h>
#include <thread>
#include <iostream>

Status ArduinoServiceBasicImpl::GetPinState(
    ServerContext* context,
    const shammam::Pin* request,
    shammam::State* reply) {

    reply->set_state(__get_pin_state(request->type(), request->index()));
    return Status::OK;
}

Status ArduinoServiceBasicImpl::SetPinState(
    ServerContext* context,
    const shammam::PinAndState* request,
    shammam::State* reply) {

    __set_pin_state(request->type(), request->index(), request->state());
    reply->set_state(__get_pin_state(request->type(), request->index()));
    return Status::OK;
}

Status ArduinoServiceBasicImpl::ResetPins(
    ServerContext* context,
    const shammam::Empty* request,
    shammam::Empty* reply) {

    __reset_pins();
    return Status::OK;
}

void rpc_thread(string addr) {
#ifndef _CUSTOM_RPC_
    shammam::Arduino::Service* service = new ArduinoServiceBasicImpl();
#else
    shammam::Arduino::Service* service = __get_rpc_service();
#endif

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "RPC server listening on " << addr << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();

    delete service;
}

void runRPC(string addr) {
    static std::thread t (rpc_thread, addr);
}
