#go build -buildmode=c-archive

# 可以指定文件

#go build -buildmode=c-archive util.go

# build dll

#gcc godll.def goutil.a -shared -lwinmm -lWs2_32 -o goutil.dll



# 直接输出dll

go build -ldflags "-s -w" -buildmode=c-shared -o "goutil.dll" "util.go"

# 创建c结构体对应的go结构体

 go tool cgo -godefs person.go > new_person.go