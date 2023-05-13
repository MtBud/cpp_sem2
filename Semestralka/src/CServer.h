#pragma once

class CServer{
public:
    int start();

    void serve( int srvrSocket );

    void shutdown();
};