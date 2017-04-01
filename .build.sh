VERSION="1.10.8"
DIRECTORY="asio-$VERSION"

if [ ! -f /usr/local/lib/libprotobuf.so ]; then
    echo "Downloading and installing protobuf and protoc (silent)..."
    ./.protobuf_dl.sh 1>/dev/null 2>&1
    echo "Finished setting up protobuf with code $?"
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
export cpp_vers=11
cmake ..
make

exitval=$?
printf "Exited with code $exitval\n"
exit $exitval
