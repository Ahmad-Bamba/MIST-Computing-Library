anum="$#"
if [ ! "$#" -eq 1 ]; then
    printf "Not building, expected output name"
else
    link="g++ protobuf_files/MISTpb.o a.o -std=c++1y -L/usr/local/lib/ -lprotobuf -lpthread -Iprotobuf_files/ -o $1"
    eval $link
    rm a.o
fi

printf "\nExited with code $?\n"
