go build -buildmode=c-archive

or

go build -buildmode=c-archive util.go

then

gcc godll.def goutil.a -shared -lwinmm -lWs2_32 -o goutil.dll