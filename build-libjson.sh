#! /bin/bash

LIB_FILE="jsoncpp-1.8.4"

f_log()
{
	echo "==> $1 <=="
}

f_log "install unzip"

sudo apt install -y unzip cmake make g++

f_log "unzip lib source file"

unzip -u ${LIB_FILE}.zip

f_log "build lib"

mkdir -p build_lib
cd build_lib
cmake -DCMAKE_BUILD_TYPE=debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -G "Unix Makefiles" ../${LIB_FILE}
make

cp src/lib_json/libjsoncpp.a ../lib

f_log "delete lib source file"

cd ..
rm -fr ${LIB_FILE}
rm -fr build_lib

ls lib