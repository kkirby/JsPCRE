PCRE_PATH=`./config.sh`
CURRENT=`pwd`
cd $PCRE_PATH
emconfigure ./configure --disable-cpp --prefix $PCRE_PATH
emmake make
emmake make install
cd $CURRENT
mv $PCRE_PATH/lib/libpcre.1.dylib ./libpcre.dylib