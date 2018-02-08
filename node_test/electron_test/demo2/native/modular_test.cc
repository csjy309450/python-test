#include <node.h>
#include <v8.h>
#include <string>
#include "core_test.h"

using namespace v8;
using namespace std;

void Method_add(const v8::FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	double arg0 = args[0]->NumberValue();
  	double arg1 = args[1]->NumberValue();
	double res = core_add(arg0, arg1);
	string str_num = std::to_string(res);;
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, str_num.c_str()));
}

void Method_min(const v8::FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	double arg0 = args[0]->NumberValue();
  	double arg1 = args[1]->NumberValue();
	double res = core_min(arg0, arg1);
	string str_num = std::to_string(res);;
	args.GetReturnValue().Set(String::NewFromUtf8(isolate, str_num.c_str()));
}

void Init(Handle<Object> exports) {
	Isolate* isolate = Isolate::GetCurrent();
	exports->Set(String::NewFromUtf8(isolate, "add"),
	FunctionTemplate::New(isolate, Method_add)->GetFunction());
	exports->Set(String::NewFromUtf8(isolate, "min"),
	FunctionTemplate::New(isolate, Method_min)->GetFunction());
}

NODE_MODULE(modular_test, Init)
