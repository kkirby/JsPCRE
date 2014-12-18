/*************************************************
*           PCRE DEMONSTRATION PROGRAM           *
*************************************************/

/* This is a demonstration program to illustrate the most straightforward ways
of calling the PCRE regular expression library from a C program. See the
pcresample documentation for a short discussion ("man pcresample" if you have
the PCRE man pages installed).

In Unix-like environments, if PCRE is installed in your standard system
libraries, you should be able to compile this program using this command:

gcc -Wall pcredemo.c -lpcre -o pcredemo

If PCRE is not installed in a standard place, it is likely to be installed with
support for the pkg-config mechanism. If you have pkg-config, you can compile
this program using this command:

gcc -Wall pcredemo.c `pkg-config --cflags --libs libpcre` -o pcredemo

If you do not have pkg-config, you may have to use this:

gcc -Wall pcredemo.c -I/usr/local/include -L/usr/local/lib \
  -R/usr/local/lib -lpcre -o pcredemo

Replace "/usr/local/include" and "/usr/local/lib" with wherever the include and
library files for PCRE are installed on your system. Only some operating
systems (e.g. Solaris) use the -R option.

Building under Windows:

If you want to statically link this program against a non-dll .a file, you must
define PCRE_STATIC before including pcre.h, otherwise the pcre_malloc() and
pcre_free() exported functions will be declared __declspec(dllimport), with
unwanted results. So in this environment, uncomment the following line. */

/* #define PCRE_STATIC */

#include <stdio.h>
#include <string.h>
#include "pcre.h"
#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>

using namespace emscripten;

class RegExp {
	protected:
		pcre* _compiled;
		int * _ovector;
		size_t _captureCount;
		unsigned char * _nameTable;
		int _nameTableSize;
		int _nameCount;
	public:
		RegExp(std::string regex,int modifiers = 0){
			const char *error;
			int erroffset;
			this->_compiled = pcre_compile(regex.c_str(),modifiers,&error,&erroffset,NULL);
			if(this->_compiled == NULL){
				printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
				EM_ASM( throw 'Invalid RegEx.'; );
				return;
			}
			pcre_fullinfo(
				this->_compiled,
				NULL,
				PCRE_INFO_CAPTURECOUNT,
				&this->_captureCount
			);
			this->_captureCount += 1;
			this->_ovector = new int[this->_captureCount * 3];
			pcre_fullinfo(
				this->_compiled,
				NULL,
				PCRE_INFO_NAMECOUNT,
				&this->_nameCount
			);
			pcre_fullinfo(
				this->_compiled,
				NULL,
				PCRE_INFO_NAMETABLE,
				&this->_nameTable
			);
			pcre_fullinfo(
				this->_compiled,
				NULL,
				PCRE_INFO_NAMEENTRYSIZE,
				&this->_nameTableSize
			);
		}
		
		~RegExp(){
			pcre_free(this->_compiled);
			delete[] this->_ovector;
			delete[] this->_nameTable;
		}
		
		val match(std::string str){
			const char * subject = str.c_str();
			int rc = pcre_exec(this->_compiled,NULL,str.c_str(),str.length(),0,0,this->_ovector,this->_captureCount * 3);
			if(rc < 0 && rc == PCRE_ERROR_NOMATCH){
				return val(false);
			}
			val array = val::global("Array");
			val context = array.new_();
			for(int i = 0; i < rc; i++){
				const char *substring_start = subject + this->_ovector[2*i];
				int substring_length = this->_ovector[2*i+1] - this->_ovector[2*i];
				char * buff = new char[substring_length+1];
				sprintf(buff,"%.*s",substring_length, substring_start);
				context.call<void>("push",val(std::string(buff)));
			}
			if(this->_nameCount > 0){
				unsigned char *tabptr = this->_nameTable;
				for(int i = 0;i < this->_nameCount;i++){
					int n = (tabptr[0] << 8) | tabptr[1];
					size_t keyLen = this->_nameTableSize - 3;
					size_t valueLen = this->_ovector[2 * n + 1] - this->_ovector[2 * n];
					char * key = new char[keyLen];
					char * value = new char[valueLen];
					sprintf(key,"%s",tabptr + 2);
					sprintf(value,"%.*s",this->_ovector[2*n+1] - this->_ovector[2*n], subject + this->_ovector[2*n]);
					context.set(val(std::string(key)),val(std::string(value)));
					tabptr += this->_nameTableSize;
				}
			}
			return context;
		}
};






EMSCRIPTEN_BINDINGS(regexp) {
	constant("PCRE_EXTENDED",PCRE_EXTENDED);
	constant("PCRE_CASELESS",PCRE_CASELESS);
	constant("PCRE_MULTILINE",PCRE_MULTILINE);
	constant("PCRE_DOTALL",PCRE_DOTALL);
	constant("PCRE_EXTENDED",PCRE_EXTENDED);
	constant("PCRE_ANCHORED",PCRE_ANCHORED);
	constant("PCRE_DOLLAR_ENDONLY",PCRE_DOLLAR_ENDONLY);
	constant("PCRE_EXTRA",PCRE_EXTRA);
	constant("PCRE_NOTBOL",PCRE_NOTBOL);
	constant("PCRE_NOTEOL",PCRE_NOTEOL);
	constant("PCRE_UNGREEDY",PCRE_UNGREEDY);
	constant("PCRE_NOTEMPTY",PCRE_NOTEMPTY);
	constant("PCRE_UTF8",PCRE_UTF8);
	constant("PCRE_UTF16",PCRE_UTF16);
	constant("PCRE_UTF32",PCRE_UTF32);
	constant("PCRE_NO_AUTO_CAPTURE",PCRE_NO_AUTO_CAPTURE);
	constant("PCRE_NO_UTF8_CHECK",PCRE_NO_UTF8_CHECK);
	constant("PCRE_NO_UTF16_CHECK",PCRE_NO_UTF16_CHECK);
	constant("PCRE_NO_UTF32_CHECK",PCRE_NO_UTF32_CHECK);
	constant("PCRE_AUTO_CALLOUT",PCRE_AUTO_CALLOUT);
	constant("PCRE_PARTIAL_SOFT",PCRE_PARTIAL_SOFT);
	class_<RegExp>("RegExp")
		.constructor<std::string,int>()
		.function("match", &RegExp::match);
}
