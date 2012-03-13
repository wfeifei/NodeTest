#include <v8.h>
#include <iostream>
using namespace v8;
using namespace std;

static Persistent<Object> process;


Handle<Value> print (const Arguments& args) {
    HandleScope scope;

    printf("In print\n");
    for (int i = 0; i< args.Length(); ++i) {
        cout << *String::Utf8Value (args[i]) << " ";
    }

    cout << endl;

    return Undefined ();
}

int main(int argc, char* argv[])
{
  HandleScope handle_scope;
  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);

  Local<FunctionTemplate> process_template = FunctionTemplate::New();
  process = Persistent<Object>::New(process_template->GetFunction()->NewInstance());
  process->Set(String::NewSymbol("version"), String::New("0.0.1"));
  process->Set(String::NewSymbol("log"), FunctionTemplate::New(print)->GetFunction());

  context->Global()->Set(String::New("log"), FunctionTemplate::New(print)->GetFunction());

  Local<String> str = String::New("(function(process){log(arguments.length, process.version); process.log('I', 'Love', 'Node');}) ");
  Local<Value> result1 = Script::Compile(str)->Run();
  Local<Function> f = Local<Function>::Cast(result1);
  Local<Value> args[1] = {Local<Value>::New(process)};
  f->Call(Context::GetCurrent()->Global(), 1, args);

  return 0;
}

