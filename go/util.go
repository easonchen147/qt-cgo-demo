package main
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person
{
    int Age;
    char* Name;
};

*/
import "C"
import "unsafe"
import "fmt"
import "github.com/axgle/mahonia"
import "net/http"
import "strings"
import "io/ioutil"
import "encoding/json"

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

//export ToNewGBKCStr
func ToNewGBKCStr(str *C.char, c **C.char) { //生成gbk字符串
	enc := mahonia.NewEncoder("UTF-8")
	output := enc.ConvertString(C.GoString(str))

	tempStr := C.CString(output)
	defer C.free(unsafe.Pointer(tempStr))

	C.strncpy(*c, tempStr, C.ulonglong(len(output)))
}

//export HttpGet
func HttpGet(path *C.char, c **C.char) {
    url:= C.GoString(path)
	//生成client 参数为默认
	client := &http.Client{}

	//提交请求
	req, err := http.NewRequest("GET", url, strings.NewReader(""))
	if err != nil {
		fmt.Println(err)
		return
	}
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	//处理返回结果
	response, err := client.Do(req)
	if err != nil {
		fmt.Println(err)
		return
	}

	buf, err := ioutil.ReadAll(response.Body)
	if err != nil {
		fmt.Println(err)
		return
	}

	cStr := C.CString(string(buf))
	defer C.free(unsafe.Pointer(cStr))
	ToNewGBKCStr(cStr, c)
}

//export SetPerson
func SetPerson(p *C.struct_Person) {
	p.Name = C.CString("SB" + C.GoString(p.Name))
	p.Age = 20 + p.Age
}

type Person struct {
    Age int
    Name string
}

//export JsonToPerson
func JsonToPerson(d *C.char, p *C.struct_Person) {
    str:= C.GoString(d)
    gp := Person{}
	err := json.Unmarshal([]byte(str), &gp)
	if err != nil {
		fmt.Println(err)
	}

	tempStr := C.CString(gp.Name)
    defer C.free(unsafe.Pointer(tempStr))

	C.strncpy(p.Name, tempStr, C.ulonglong(len(gp.Name)))
	p.Age = C.int(gp.Age)
}

func main() {}