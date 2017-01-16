package homa

import (
	"log"
	"sync"

	"github.com/tarm/serial"
)

// SerialTagReader is a implementation of tag reader interface.
type SerialTagReader struct {
	port *serial.Port
	once sync.Once
}

// Init function initialize a serial connection for tag reading.
func (s *SerialTagReader) init() {
	c := &serial.Config{Name: "/dev/ttyUSB0", Baud: 9600}
	p, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}
	s.port = p
}

// Read function waits until one tag come and reads it's uid.
func (s *SerialTagReader) Read() *Tag {
	s.init()

	buf := make([]byte, 128)
	n, err := s.port.Read(buf)
	if err != nil {
		log.Fatal(err)
	}

	if buf[0] != '@' {
		return nil
	}

	t := new(Tag)
	t.uid = string(buf[2 : n-1])

	return t
}
