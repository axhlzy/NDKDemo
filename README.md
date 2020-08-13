# NDKDemo

extern "C" 
void _init(void) { } 

      -------》编译生成后在.init段  
  
__attribute__((constructor)) 
void _init(void) { } 

      -------》编译生成后在.init_array段  
      
__attribute__((destructor))
void final(void) { }

      -------》编译生成后在.init_array段  
      

### RegisterNative效率高于直接调用JNI本地函数（静态注册）

- 1.调用 System. loadlibrarye（）方法，将包含本地方法具体实现的C++运行库加载
到内存中
- 2.Java虚拟机检索加载进来的库函数符号，在其中査找与Java本地方法拥有相同签名的JNI本地函数符号。若找到一致的，则将本地方法映射到具体的JNI本地函数
- 3.Android Framework这类复杂的系统下，拥有大量的包含本地方法的Java类，Java虛机加载相应运行库，再逐一检索，将各个本地方法与相应的函数映射起来，这显然会增加运行时间，降低运行的效率，所以有了RegisterNative()
      
      
### 调用顺序

linker --->  do_dlopen() ---> find_library() 

---> CallConstructors()

---> CallFunction("DT_INIT", init_func)			//调用init

---> CallArray("DT_INIT_ARRAY", init_array, init_array_count, false); 	//调用init_array

#### CallConstructors
```
// so库文件加载完毕以后调用构造函数  
void soinfo::CallConstructors() {  
      
  if (constructors_called) {  
    return;  
  }  
  
  // We set constructors_called before actually calling the constructors, otherwise it doesn't  
  // protect against recursive constructor calls. One simple example of constructor recursion  
  // is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:  
  // 1. The program depends on libc, so libc's constructor is called here.  
  // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.  
  // 3. dlopen() calls the constructors on the newly created  
  //    soinfo for libc_malloc_debug_leak.so.  
  // 4. The debug .so depends on libc, so CallConstructors is  
  //    called again with the libc soinfo. If it doesn't trigger the early-  
  //    out above, the libc constructor will be called again (recursively!).  
  constructors_called = true;  
  
  if ((flags & FLAG_EXE) == 0 && preinit_array != NULL) {  
    // The GNU dynamic linker silently ignores these, but we warn the developer.  
    PRINT("\"%s\": ignoring %d-entry DT_PREINIT_ARRAY in shared library!",  
          name, preinit_array_count);  
  }  
  
  // 调用DT_NEEDED类型段的构造函数  
  if (dynamic != NULL) {  
    for (Elf32_Dyn* d = dynamic; d->d_tag != DT_NULL; ++d) {  
      if (d->d_tag == DT_NEEDED) {  
        const char* library_name = strtab + d->d_un.d_val;  
        TRACE("\"%s\": calling constructors in DT_NEEDED \"%s\"", name, library_name);  
        find_loaded_library(library_name)->CallConstructors();  
      }  
    }  
  }  
  
  TRACE("\"%s\": calling constructors", name);  
  
  // DT_INIT should be called before DT_INIT_ARRAY if both are present.  
  // 先调用.init段的构造函数  
  CallFunction("DT_INIT", init_func);  
  // 再调用.init_array段的构造函数  
  CallArray("DT_INIT_ARRAY", init_array, init_array_count, false);  
}  
```
#### CallFunction
```
// 构造函数调用的实现  
void soinfo::CallFunction(const char* function_name UNUSED, linker_function_t function) {  
  
  // 判断构造函数的调用地址是否符合要求  
  if (function == NULL || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {  
    return;  
  }  
  
  // function_name被调用的函数名称，function为函数的调用地址  
  // [ Calling %s @ %p for '%s' ] 字符串为在 /system/bin/linker 中查找.init和.init_array段调用函数的关键  
  TRACE("[ Calling %s @ %p for '%s' ]", function_name, function, name);  
  // 调用function函数  
  function();  
  TRACE("[ Done calling %s @ %p for '%s' ]", function_name, function, name);  
  
  // The function may have called dlopen(3) or dlclose(3), so we need to ensure our data structures  
  set_soinfo_pool_protection(PROT_READ | PROT_WRITE);  
}  
```
#### CallArray
```
void soinfo::CallArray(const char* array_name UNUSED, linker_function_t* functions, size_t count, bool reverse) {  
  if (functions == NULL) {  
    return;  
  }  
  
  TRACE("[ Calling %s (size %d) @ %p for '%s' ]", array_name, count, functions, name);  
  
  int begin = reverse ? (count - 1) : 0;  
  int end = reverse ? -1 : count;  
  int step = reverse ? -1 : 1;  
  
  // 循环遍历调用.init_arrayt段中每个函数  
  for (int i = begin; i != end; i += step) {  
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);  
      
    // .init_arrayt段中，每个函数指针的调用和上面的.init段的构造函数的实现是一样的  
    CallFunction("function", functions[i]);  
  }  
  
  TRACE("[ Done calling %s for '%s' ]", array_name, name);  
}  
```
