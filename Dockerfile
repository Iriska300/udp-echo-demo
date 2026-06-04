FROM gcc:latest

COPY echo-server.cpp /echo-server.cpp
RUN g++ /echo-server.cpp -o /echo-server

EXPOSE 8080/udp

CMD ["/echo-server"]
