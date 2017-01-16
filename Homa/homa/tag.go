package homa

// Tag structure stores information about tags.
type Tag struct {
	uid string
}

// TagReader interface provides a way to read the tags.
type TagReader interface {
	Read() *Tag
}
