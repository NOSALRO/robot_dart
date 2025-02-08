

# File stb\_image\_write.h



[**FileList**](files.md) **>** [**gui**](dir_6a9d4b7ec29c938d1d9a486c655cfc8a.md) **>** [**stb\_image\_write.h**](stb__image__write_8h.md)

[Go to the source code of this file](stb__image__write_8h_source.md)



* `#include <stdlib.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef void | [**stbi\_write\_func**](#typedef-stbi_write_func)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  int | [**stbi\_write\_force\_png\_filter**](#variable-stbi_write_force_png_filter)  <br> |
|  int | [**stbi\_write\_png\_compression\_level**](#variable-stbi_write_png_compression_level)  <br> |
|  int | [**stbi\_write\_tga\_with\_rle**](#variable-stbi_write_tga_with_rle)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  STBIWDEF void | [**stbi\_flip\_vertically\_on\_write**](#function-stbi_flip_vertically_on_write) (int flip\_boolean) <br> |
|  STBIWDEF int | [**stbi\_write\_bmp**](#function-stbi_write_bmp) (char const \* filename, int w, int h, int comp, const void \* data) <br> |
|  STBIWDEF int | [**stbi\_write\_bmp\_to\_func**](#function-stbi_write_bmp_to_func) (stbi\_write\_func \* func, void \* context, int w, int h, int comp, const void \* data) <br> |
|  STBIWDEF int | [**stbi\_write\_hdr**](#function-stbi_write_hdr) (char const \* filename, int w, int h, int comp, const float \* data) <br> |
|  STBIWDEF int | [**stbi\_write\_hdr\_to\_func**](#function-stbi_write_hdr_to_func) (stbi\_write\_func \* func, void \* context, int w, int h, int comp, const float \* data) <br> |
|  STBIWDEF int | [**stbi\_write\_jpg**](#function-stbi_write_jpg) (char const \* filename, int x, int y, int comp, const void \* data, int quality) <br> |
|  STBIWDEF int | [**stbi\_write\_jpg\_to\_func**](#function-stbi_write_jpg_to_func) (stbi\_write\_func \* func, void \* context, int x, int y, int comp, const void \* data, int quality) <br> |
|  STBIWDEF int | [**stbi\_write\_png**](#function-stbi_write_png) (char const \* filename, int w, int h, int comp, const void \* data, int stride\_in\_bytes) <br> |
|  STBIWDEF int | [**stbi\_write\_png\_to\_func**](#function-stbi_write_png_to_func) (stbi\_write\_func \* func, void \* context, int w, int h, int comp, const void \* data, int stride\_in\_bytes) <br> |
|  STBIWDEF int | [**stbi\_write\_tga**](#function-stbi_write_tga) (char const \* filename, int w, int h, int comp, const void \* data) <br> |
|  STBIWDEF int | [**stbi\_write\_tga\_to\_func**](#function-stbi_write_tga_to_func) (stbi\_write\_func \* func, void \* context, int w, int h, int comp, const void \* data) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**STBIWDEF**](stb__image__write_8h.md#define-stbiwdef)  extern<br> |

## Public Types Documentation




### typedef stbi\_write\_func 

```C++
typedef void stbi_write_func(void *context, void *data, int size);
```




<hr>
## Public Attributes Documentation




### variable stbi\_write\_force\_png\_filter 

```C++
int stbi_write_force_png_filter;
```




<hr>



### variable stbi\_write\_png\_compression\_level 

```C++
int stbi_write_png_compression_level;
```




<hr>



### variable stbi\_write\_tga\_with\_rle 

```C++
int stbi_write_tga_with_rle;
```




<hr>
## Public Functions Documentation




### function stbi\_flip\_vertically\_on\_write 

```C++
STBIWDEF void stbi_flip_vertically_on_write (
    int flip_boolean
) 
```




<hr>



### function stbi\_write\_bmp 

```C++
STBIWDEF int stbi_write_bmp (
    char const * filename,
    int w,
    int h,
    int comp,
    const void * data
) 
```




<hr>



### function stbi\_write\_bmp\_to\_func 

```C++
STBIWDEF int stbi_write_bmp_to_func (
    stbi_write_func * func,
    void * context,
    int w,
    int h,
    int comp,
    const void * data
) 
```




<hr>



### function stbi\_write\_hdr 

```C++
STBIWDEF int stbi_write_hdr (
    char const * filename,
    int w,
    int h,
    int comp,
    const float * data
) 
```




<hr>



### function stbi\_write\_hdr\_to\_func 

```C++
STBIWDEF int stbi_write_hdr_to_func (
    stbi_write_func * func,
    void * context,
    int w,
    int h,
    int comp,
    const float * data
) 
```




<hr>



### function stbi\_write\_jpg 

```C++
STBIWDEF int stbi_write_jpg (
    char const * filename,
    int x,
    int y,
    int comp,
    const void * data,
    int quality
) 
```




<hr>



### function stbi\_write\_jpg\_to\_func 

```C++
STBIWDEF int stbi_write_jpg_to_func (
    stbi_write_func * func,
    void * context,
    int x,
    int y,
    int comp,
    const void * data,
    int quality
) 
```




<hr>



### function stbi\_write\_png 

```C++
STBIWDEF int stbi_write_png (
    char const * filename,
    int w,
    int h,
    int comp,
    const void * data,
    int stride_in_bytes
) 
```




<hr>



### function stbi\_write\_png\_to\_func 

```C++
STBIWDEF int stbi_write_png_to_func (
    stbi_write_func * func,
    void * context,
    int w,
    int h,
    int comp,
    const void * data,
    int stride_in_bytes
) 
```




<hr>



### function stbi\_write\_tga 

```C++
STBIWDEF int stbi_write_tga (
    char const * filename,
    int w,
    int h,
    int comp,
    const void * data
) 
```




<hr>



### function stbi\_write\_tga\_to\_func 

```C++
STBIWDEF int stbi_write_tga_to_func (
    stbi_write_func * func,
    void * context,
    int w,
    int h,
    int comp,
    const void * data
) 
```




<hr>
## Macro Definition Documentation





### define STBIWDEF 

```C++
#define STBIWDEF extern
```




<hr>

------------------------------
The documentation for this class was generated from the following file `robot_dart/gui/stb_image_write.h`

