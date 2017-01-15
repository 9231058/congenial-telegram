package homa

// Tag structure stores information about tags.
type Tag struct {
	uid []byte
}

// TagReader interface provides a way to read the tags.
type TagReader interface {
	Init()
	Read() *Tag
}
