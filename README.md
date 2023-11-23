# PDAL

## 依赖

1. GDAL

```bash
> -DGDAL_INCLUDE_DIR=D:\devtools\GDAL.3.7.3\include -DGDAL_LIBRARY=D:\devtools\GDAL.3.7.3\lib\libgdal.dll.a 
```

2. GeoTIFF

```bash
> -DGEOTIFF_LIBRARY=D:\devtools\LIBGEOTIFF.1.7.1\lib\libgeotiff.a -DGEOTIFF_INCLUDE_DIR=D:\devtools\LIBGEOTIFF.1.7.1\include 
```

3. zlib

```bash
> -DZLIB_LIBRARY=D:/devtools/zlib.1.3/lib/libzlib.dll.a -DZLIB_INCLUDE_DIR=D:/devtools/zlib.1.3/include
```

4. curl

```bash
> -DCURL_LIBRARY=D:\devtools\crul.mingw.8.2.1\lib\libcurl.dll.a -DCURL_INCLUDE_DIR=D:\devtools\crul.mingw.8.2.1\include
>
```

## build on Windows use mingw64
```bash
> mkdir build & cd build
> cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release DNANA_CMAKE_NANA_FILESYSTEM_FORCE=1 -DWITH_TESTS=0 -DCMAKE_INSTALL_PREFIX=D:\devtools\PDAL.2.6.0 -DPROJ_LIBRARY=D:\devtools\proj.9.2.1\lib\libproj.dll.a -DPROJ_INCLUDE_DIR=D:\devtools\proj.9.2.1\include -DGDAL_INCLUDE_DIR=D:\devtools\GDAL.3.7.3\include -DGDAL_LIBRARY=D:\devtools\GDAL.3.7.3\lib\libgdal.dll.a -DGEOTIFF_LIBRARY=D:\devtools\LIBGEOTIFF.1.7.1\lib\libgeotiff.a -DGEOTIFF_INCLUDE_DIR=D:\devtools\LIBGEOTIFF.1.7.1\include -DZLIB_LIBRARY=D:/devtools/zlib.1.3/lib/libzlib.dll.a -DZLIB_INCLUDE_DIR=D:/devtools/zlib.1.3/include -DCURL_LIBRARY=D:\devtools\crul.mingw.8.2.1\lib\libcurl.dll.a -DCURL_INCLUDE_DIR=D:\devtools\crul.mingw.8.2.1\include -DTIFF_INCLUDE_DIR=D:\devtools\TIFF.4.6.0\include -DTIFF_LIBRARY=D:/devtools/TIFF.4.6.0/lib/libtiff.dll.a
> 
> ninja
> ninja install
```
