#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

// Include the Ruby headers and goodies
#include "ruby.h"

using namespace std;

typedef VALUE (ruby_method)(...);

static string read_proc_name(string& dirname, string& filename){
	int i=0;
	char data[2048];
	data[2047]=0;
	string sdata;
	string name = dirname;
	name += "/";
	name += filename;
	name += "/stat";
	int fd = open(name.c_str(), O_RDONLY);
	read(fd, data, 2047);
	close(fd);
	sdata = string(data);
	i = sdata.find(" ");
	name = sdata.substr(i+1, sdata.find(" ", i+1)-i);
	return name;
};

static VALUE 
get_list_processes(VALUE self){
	 DIR* dp;	
	 struct dirent *dirp;
	 string dir("/proc");
	 vector<string> files;
	 vector<int> pids;
	 string filename;
	 int pid;
	 if((dp  = opendir(dir.c_str())) == NULL) {
		  cout << "Error(" << errno << ") opening " << dir << endl;
		  return errno;
	 }

	 while ((dirp = readdir(dp)) != NULL ) {
		pid = atoi(dirp->d_name);
		if(dirp->d_type == DT_DIR && pid != 0){
			filename = string(dirp->d_name);
			pids.push_back(pid);
			files.push_back(read_proc_name(dir, filename));
		}
	 }

	 VALUE rb_array_proc = rb_ary_new2(pids.size());
	 for(int i=0; i < pids.size(); i++){
		VALUE rb_processo = rb_ary_new2(2);
		rb_ary_push(rb_processo, INT2FIX(pids[i]));
		rb_ary_push(rb_processo, rb_str_new2(files[i].c_str()));
		rb_ary_push(rb_array_proc, rb_processo);
	 }

	 closedir(dp);
	 return rb_array_proc;
};

static VALUE t_init(VALUE self){
	return self;
};
// The initialization method for this module
extern "C" {
void Init_cproctable() {
	//create a Ruby Module
	VALUE rb_cproctable_module = rb_define_module("CProc");
	//create class inside the module
	//rb_cObject is defined in ruby.h
	VALUE rb_cproctable_class = rb_define_class_under(rb_cproctable_module,
																	"CProcTable", rb_cObject);

	rb_define_method(rb_cproctable_class, "initialize", 
							(ruby_method*) &t_init, 0);
	rb_define_method(rb_cproctable_class, "get_list_processes", 
							(ruby_method*) &get_list_processes, 0);
};
} // extern "C"
