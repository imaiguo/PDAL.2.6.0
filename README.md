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

## Run Time

```bash
>
> set Path=D:\devtools\Qt\Qt5.12.12\Tools\mingw730_64\bin;D:\devtools\proj.9.2.1\bin;D:\devtools\TIFF.4.6.0\bin;D:\devtools\crul.mingw.8.2.1\bin;D:\devtools\GDAL.3.7.3\bin;D:\devtools\zlib.1.3\bin;D:\devtools\lz4.1.9.4\bin;%Path%
>
```

## Tools

### 查看点云数据信息

```bash
>
> pdal.exe info C:\Users\ephraim\Documents\cloudlas\7_8.las -p 0
> D:\devtools\PDAL.2.6.0\bin>pdal.exe info C:\Users\ephraim\Documents\autzen.laz -p 0
{
  "file_size": 56350988,
  "filename": "C:\\Users\\ephraim\\Documents\\autzen.laz",
  "now": "T锟叫癸拷锟斤拷准时锟斤拷",
  "pdal_version": "2.6.0 (git-version: 4ca892)",
  "points":
  {
    "point":
    {
      "Blue": 93,
      "Classification": 1,
      "EdgeOfFlightLine": 0,
      "GpsTime": 245379.3984,
      "Green": 102,
      "Intensity": 4,
      "KeyPoint": 0,
      "NumberOfReturns": 1,
      "Overlap": 0,
      "PointId": 0,
      "PointSourceId": 7326,
      "Red": 84,
      "ReturnNumber": 1,
      "ScanAngleRank": -17,
      "ScanDirectionFlag": 0,
      "Synthetic": 0,
      "UserData": 128,
      "Withheld": 0,
      "X": 637177.98,
      "Y": 849393.95,
      "Z": 411.19
    }
  },
  "reader": "readers.las"
}
>
```


### 点云数据格式转换


```bash
>
> D:\devtools\PDAL.2.6.0\bin>pdal.exe translate C:\Users\ephraim\Documents\autzen.laz C:\Users\ephraim\Documents\autzen.txt
>
> D:\devtools\PDAL.2.6.0\bin>pdal.exe translate C:\Users\ephraim\Documents\autzen.laz C:\Users\ephraim\Documents\autzen.las
>
> D:\devtools\PDAL.2.6.0\bin>pdal.exe info --metadata C:\Users\ephraim\Documents\autzen.laz
>
```

### 点云数据过滤

Ground Filter Tutorial [PMF/SMRF ]

配置文件pipeline.json内容

```json
{
    "type":"filters.range",
    "limits":"Classification[2:2]"
}
```

```bash
>
> pdal.exe translate input.las output.las --json pipeline.json
>
```

识别地面 

a. 普通方法

```bash
>
> pdal.exe translate ./exercises/analysis/ground/CSite1_orig-utm.laz -o ./exercises/analysis/ground/ground.copc.laz smrf -v 4
>
```

b. 使用过滤器

Let us start by removing the non-ground data to just view the ground data

```bash
> pdal.exe translate \
./exercises/analysis/ground/CSite1_orig-utm.laz \
-o ./exercises/analysis/ground/ground.copc.laz \
smrf expression \
--filters.expression.expression="Classification == 2"\
-v 4
```

c. 综合使用

Now we will instead use the translate command to stack the filters.outlier and filters.smrf stages:

```bash
> pdal.exe translate ./exercises/analysis/ground/CSite1_orig-utm.laz \
-o ./exercises/analysis/ground/denoised-ground-only.copc.laz \
outlier smrf expression  \
--filters.outlier.method="statistical" \
--filters.outlier.mean_k=8 --filters.outlier.multiplier=3.0 \
--filters.smrf.ignore="Classification[7:7]"  \
--filters.expression.expression="Classification == 2"
```
