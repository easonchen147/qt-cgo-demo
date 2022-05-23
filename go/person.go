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

typedef struct PersonT
{
    int Age;
    char* Name;
} PersonT;
*/
import "C"

// 定义结构体有以下两种方式

// type Person C.struct_Person
// 这个使用C.struct_Person
type Person struct {
        Age     int32
        Name    *int8
}

// type PersonT C.PersonT
// 这个使用C.PersonT
type PersonT struct {
        Age     int32
        Name    *int8
}

