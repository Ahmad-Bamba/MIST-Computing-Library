VERSION="1.10.8"
DIRECTORY="asio-$VERSION"
if [ ! -d "$DIRECTORY" ]; then
    wget "https://sourceforge.net/projects/asio/files/latest/download?source=files"
    tar -xvf "download?source=files"
fi
echo g++ -std=c++1y -Iasio-1.10.8/include/ src/*.cpp -o out
g++ -std=c++1y -Iasio-1.10.8/include/ -c -lpthread src/*.cpp -o a.o
