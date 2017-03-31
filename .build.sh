VERSION="1.10.8"
DIRECTORY="asio-$VERSION"

if [ ! -f /usr/local/lib/libprotobuf.so ]; then
    ./.protobuf_dl.sh
fi

if [ ! -d "$DIRECTORY" ]; then
    wget "http://learn612.000webhostapp.com/asio-1.10.8.tar.gz"
    tar -xvf "asio-1.10.8.tar.gz"
fi

mkdir .build/
cd .build/
cmake ..
make

printf "Exited with code $?\n"

c='

anum="$#"

if [ "$anum" -gt 0 ]; then
    printf "\nBuilding with $1\n"
    build="g++ $1 -std=c++1y -Iasio-1.10.8/include/ -Iinclude/ -Iprotobuf_files/ -lpthread -lprotobuf -c -o a.o"
    eval $build
else
    printf "\nBuilding...\n"
    g++ -std=c++1y -Iasio-1.10.8/include/ -Iinclude/ -Iprotobuf_files -c -lpthread src/MIST.cpp -lprotobuf -o a.o
fi
'
