package main

import (
	"fmt"
	"log"

	"github.com/tarm/serial"
)

// working example using simple buffer
func main_working_example() {
	c := &serial.Config{Name: "/dev/cu.usbmodem1234561", Baud: 115200}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal("Cannot open port. ", err)
	}

	fmt.Println("got Port")

	_, err = s.Write([]byte("GENRAND"))
	if err != nil {
		log.Fatal("Cannot write bytes. ", err)
	}

	buf := make([]byte, 256+6)
	n, err := s.Read(buf)
	if err != nil {
		log.Fatal(err)
	}
	log.Print("%q", buf[:n])
}
