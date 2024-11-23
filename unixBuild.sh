CC=g++
CFLAGS="-std=c++14 -O2 -DNDEBUG"
INCLUDES="-I src -I calendar -I qimen -I swtwl -I yixue -I zhcn"
LDFLAGS=""
TARGET="build/release/csp"
OBJ_DIR="build/release"
SRC_DIRS="src calendar qimen swtwl yixue zhcn"
if [ -d "$OBJ_DIR" ]; then
    rm -rf "$OBJ_DIR"
fi
mkdir -p "$OBJ_DIR"
for dir in $SRC_DIRS; do
    for f in $dir/*.cpp; do
        echo "Compiling $f ..."
        $CC $CFLAGS $INCLUDES -c $f -o "$OBJ_DIR/$(basename "$f" .cpp).o"
    done
done
echo "Linking executable ..."
$CC $LDFLAGS -o $TARGET $OBJ_DIR/*.o
echo "Compile done: $TARGET"
echo "Cleaning up object files ..."
rm -f $OBJ_DIR/*.o
echo "Build and cleanup completed."
