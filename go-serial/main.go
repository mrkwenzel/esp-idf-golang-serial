package main

import (
	"bufio"
	"fmt"
	"log"
	"time"

	"github.com/tarm/serial"
)

func scannSerialRX(s *serial.Port) {
	fmt.Println("got Port")

	scanner := bufio.NewScanner(s)
	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}

	if scanner.Err() != nil {
		log.Fatal(scanner.Err())
	}
}

func main() {
	c := &serial.Config{Name: "/dev/cu.usbmodem1234561", Baud: 115200}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal("Cannot open port. ", err)
	}

	go scannSerialRX(s)

	for i := 0; i < 10; i++ {
		time.Sleep(time.Second * 2)
		_, err = s.Write([]byte("GENJOKE"))
		time.Sleep(time.Second * 1)
		fmt.Println("sent genjoke")
		if err != nil {
			log.Fatal("Cannot write bytes. ", err)
		}
	}

	for i := 0; i < 10; i++ {
		time.Sleep(time.Second * 2)
		_, err = s.Write([]byte("GENRAND"))
		time.Sleep(time.Second * 1)
		fmt.Println("sent genrand")
		if err != nil {
			log.Fatal("Cannot write bytes. ", err)
		}
	}
}
