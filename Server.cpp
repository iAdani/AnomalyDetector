

#include "Server.h"

// ---------- SocketIO ----------

// reads from socket
string SocketIO::read(){
    string s = "";
    char input = 0;

    while(input != '\n') {
        recv(ID, &input, sizeof(char), 0);
        s += input;
    }

    return s;
}

// writes to output
void SocketIO::write(string text){
    const char *output = text.c_str();
    send(ID, output, strlen(output), 0);
}

// writes to output
void SocketIO::write(float f){
    ostringstream stream;
    stream << f;
    string output(stream.str());
    write(output);
}

// sadly this is useless
void SocketIO::read(float* f) { }

// ---------- Server ----------

// setting up the server
Server::Server(int port) throw (const char*) {
    struct sockaddr_in address;

    // creating socket file description
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        throw "socket failed!";
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port);

    // attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw "bind failed!";
    }

    // listening
    if (listen(server_fd, 3) < 0) {
        throw "listen failed!";
    }

    isListening = true;
}

// used to handle the signal alarm
void alarm_handler(int a) { cout << "handling..." << endl; }

void Server::start(ClientHandler& ch) throw(const char*) {
    // making a thread to interact with the client, using a lambda expression
    t = new thread([&, this]() {
        signal(SIGALRM, alarm_handler);
        while (isListening) {
            alarm(3);
            socklen_t size = sizeof(client);
            int curClient = accept(server_fd, (struct sockaddr *)&client, &size);
            if (curClient > 0) {
                ch.handle(curClient);
                close(curClient);
            }
            alarm(0);
        }
        close(server_fd);
    });
}

void Server::stop(){
    isListening = false;
	t->join(); // do not delete this!
}

Server::~Server() {
}

