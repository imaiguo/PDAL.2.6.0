.. _readers.gdal:

readers.gdal
================================================================================

The `GDAL`_ reader reads `GDAL readable raster`_ data sources as point clouds.

.. _`GDAL`: http://gdal.org
.. _`GDAL readable raster`: http://www.gdal.org/formats_list.html

Each pixel is given an X and Y coordinate (and corresponding PDAL dimensions)
that are center pixel, and each band is represented by "band-1", "band-2", or
"band-n".  Using the 'header' option allows naming the band data to standard
PDAL dimensions.

.. embed::

Basic Example
--------------------------------------------------------------------------------

Simply writing every pixel of a JPEG to a text file is not very useful.

.. code-block:: json

  [
      {
          "type":"readers.gdal",
          "filename":"./pdal/test/data/autzen/autzen.jpg"
      },
      {
          "type":"writers.text",
          "filename":"outputfile.txt"
      }
  ]


LAS Example
--------------------------------------------------------------------------------

The following example assigns the bands from a JPG to the
RGB values of an `ASPRS LAS`_ file using :ref:`writers.las`.

.. _`ASPRS LAS`: http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html

.. code-block:: json

  [
      {
          "type":"readers.gdal",
          "filename":"./pdal/test/data/autzen/autzen.jpg",
          "header": "Red, Green, Blue"
      },
      {
          "type":"writers.las",
          "filename":"outputfile.las"
      }
  ]


.. note::
   :ref:`readers.gdal` is quite sensitive to GDAL's cache settings. See the
   ``GDAL_CACHEMAX`` value at https://gdal.org/user/configoptions.html for
   more information.

Options
--------------------------------------------------------------------------------

filename
  `GDALOpen`_ 'able raster file to read [Required]

.. _`GDALOpen`: https://gdal.org/api/raster_c_api.html#gdal_8h_1aca05455472359964151f9c891d678d5e

.. include:: reader_opts.rst

header
    A comma-separated list of :ref:`dimension <dimensions>` IDs to map
    bands to. The length of the list must match the number
    of bands in the raster.

memorycopy
    Use the `GDAL MEM driver <https://gdal.org/drivers/raster/mem.html>`__
    to copy the entire raster into memory before converting to points. This
    is useful if the raster driver has a lot of per-block overhead or you
    are willing to trade memory for performance.

gdalopts
    A list of key/value options to pass directly to the GDAL driver.  The
    format is name=value,name=value,...  The option may be specified
    any number of times.
