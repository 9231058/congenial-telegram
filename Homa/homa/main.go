/*
* +===============================================
* | Author:        Parham Alvani (parham.alvani@gmail.com)
* |
* | Creation Date: 14-01-2017
* |
* | File Name:     main.go
* +===============================================
 */

package homa

import (
	"log"
	"net/http"
)

// Main function initiates the homa server and routes.
func Main() {
	fs := http.FileServer(http.Dir("homa-ui/dist"))
	http.Handle("/", fs)

	log.Println("Listening...")
	if err := http.ListenAndServe(":1373", nil); err != nil {
		log.Fatal("ListenAndServe:", err)
	}
}
