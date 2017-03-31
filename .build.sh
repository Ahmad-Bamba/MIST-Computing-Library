VERSION="1.10.8"
DIRECTORY="asio-$VERSION"

if [ ! -f /usr/local/lib/libprotobuf.so ]; then
    ./.protobuf_dl.sh
fi

# this should be false on users, but true on travis
if [ ! -f protobuf_files/MIST.pb.h ]; then
    echo "MIST.pb files not found. Generating from protoc..."
    protoc -I=protobuf_files/ --cpp_out=protobuf_files/ protobuf_files/MIST.proto
    echo "protoc exited with code: $?"
fi

if [ ! -d "$DIRECTORY" ]; then
    wget "http://learn612.000webhostapp.com/asio-1.10.8.tar.gz"
    tar -xf "asio-1.10.8.tar.gz"
fi

mkdir .build/
cd .build/
cmake ..
make

exitval=$?
printf "Exited with code $exitval\n"
exit $exitval
