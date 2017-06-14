```
enum { FALSE, TRUE };

// If defined, called once before using plugin
// dataDir is used as root for all required files
void init(const char *dataDir);



enum { NO, YES_FROM_EXTENSION, YES_FROM_CONTENT };
int can_handle(const char* file);

void* load(const char *file);
void* load_ptr(const void* ptr, int size);
void* load_stream(int type);


void put_stream(void*, const unsigned char* bytes, int size);

int get_samples(void*, short* target, int targetSize);

void set_meta_callback(void (*fn)(int n, const char** changed));
void* get_meta(const char* what);

void seek(void*, int tune, int msecs);

void set_parameter(const char* what, void* value);


```



