package main

import (
	"fmt"
	"log"
	"strings"

	"go.bug.st/serial"
)

func main() {

	// Retrieve the port list
	ports, err := serial.GetPortsList()
	if err != nil {
		log.Fatal(err)
	}
	if len(ports) == 0 {
		log.Fatal("No serial ports found!")
	}

	// Open the first serial port detected at 9600bps N81
	mode := &serial.Mode{
		BaudRate: 115200,
		Parity:   serial.NoParity,
		DataBits: 8,
		StopBits: serial.OneStopBit,
	}
	port, err := serial.Open("/dev/cu.usbmodem14201", mode)
	if err != nil {
		log.Fatal(err)
	}

	// Send the string "10,20,30\n\r" to the serial port
	n, err := port.Write([]byte("random_number\n\r"))
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("Sent %v bytes\n", n)

	// Read and print the response

	buff := make([]byte, 100)
	for {
		// Reads up to 100 bytes
		n, err := port.Read(buff)
		if err != nil {
			log.Fatal(err)
		}
		if n == 0 {
			fmt.Println("\nEOF")
			break
		}

		fmt.Printf("%s", string(buff[:n]))

		// If we receive a newline stop reading
		if strings.Contains(string(buff[:n]), "\n") {
			break
		}
	}
}
