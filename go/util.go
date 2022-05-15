package main

/*
#include <stdio.h>
#include <stdlib.h>
*/
import "C"

//export Add
func Add(a, b C.int) C.int {
    return a + b
}

//export Concat
func Concat(a, b *C.char) *C.char {
    aGStr := C.GoString(a)
    bGStr := C.GoString(b)
    result := C.CString(aGStr+bGStr)
    return result
}

func main() {}