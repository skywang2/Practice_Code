package main

import (
	"context"
	"flag"
	"fmt"
	"log"
	"net"

	"google.golang.org/grpc"
	pb "google.golang.org/grpc/examples/helloworld/helloworld" // protobuf文件
)

var (
	port = flag.Int("port", 8083, "The server port")
)

type server struct {
	pb.UnimplementedGreeterServer
}

func (s *server) DoSomething(ctx context.Context, in *pb.HelloRequest) (*pb.HelloReply, error) {
	log.Printf("Received: %v", in.GetName())
	return &pb.HelloReply{Message: "Hello" + in.GetName()}, nil
}

func main() {
	flag.Parse()

	lis, _ := net.Listen("tcp", fmt.Sprintf(":%d", *port))	
	s := grpc.NewServer()
	pb.RegisterGreeterServer(s, &server{})
	s.Serve(lis)
}
