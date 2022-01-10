/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sstream>

#include "commands.h"
#include "CLI.h"


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class SocketIO : public DefaultIO {
    int ID;

public:
    SocketIO(int id) {
        ID = id;
    }
    virtual string read(); // reads from socket
    virtual void write(string text); // writes to output
    virtual void write(float f); // writes to output
    virtual void read(float* f);// reads from socket
    virtual ~SocketIO() { };
};



// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:

    // "handle" the client once he connects
    virtual void handle(int clientID){
        SocketIO io(clientID);
        CLI cli(&io);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    bool isListening;
    int server_fd;
    sockaddr_in client;
	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
