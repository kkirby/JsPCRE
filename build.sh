PCRE_PATH=`./config.sh`
emcc ./libpcre.dylib pcre.cc -I$PCRE_PATH -Wno-warn-absolute-paths --bind -o pcre.js