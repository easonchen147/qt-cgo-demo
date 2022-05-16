package main
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
*/
import "C"
import "unsafe"
import "fmt"

//export Add
func Add(a, b C.int) C.int {
    return a + b
}

//export Concat
func Concat(a, b, out *C.char) int {
    aGStr := C.GoString(a)
    bGStr := C.GoString(b)
    abResult := aGStr+bGStr
    fmt.Println("result: " + abResult)

    tempStr := C.CString(abResult)
    // 离开需要释放内存
    defer C.free(unsafe.Pointer(tempStr))

    count := len(abResult)
    // 拷贝结果到output
    C.strncpy(out, tempStr, C.ulonglong(count))

    return 0
}

func main() {}