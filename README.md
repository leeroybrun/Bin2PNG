# Bin2PNG

Convert a binary file to a PNG image and then decrypt it back to binary.

## Binary to PNG :

Each bytes of the binary file are converted to numbers (0-255), which will then define a pixel color.
For each bytes will be a grayscale pixel in the final PNG image. 
The PNG image can have more pixels than the number of bytes of the binary file. 
The excess pixels will have a RGB(255, 0, 0) color (red) and will be ignored when decrypting the file.

## PNG to Binary :

When decrypting the PNG file to create the corresponding binary file, we take each pixels of the image and get their color.
If the R & G color are different, we skip this pixel. It is not at grayscale, and so is an excess pixel.
It it is grayscale, we take the color number for R and then convert it to it's binary representation.
We construct an unsigned char array with all decrypted binary data of pixels and then save it back to a binary file.

## Usage :

You can download the exe file directly from here : https://github.com/leeroybrun/Bin2PNG/releases/download/v1.0/Bin2PNG.exe

Or you can build it with Visual Studio or any other C compiler.

Then place your binary file to transform in the same folder as the `Bin2PNG.exe` file, and rename it to `in.exe`.

You can then encrypt it to a PNG image :

```shell
Bin2PNG.exe encrypt
```

If successfull, it will then generate an `image.png` file.

You can then decrypt this file back to a binary like this :

```shell
Bin2PNG.exe decrypt
```

It will decrypt the `image.png` file and create a new binary `out.exe`.

## Todo :

- Add parameters to define the name of the binary and image files
- Maybe find a better way to convert byte to number
- Find why the decrypted exe is a bit larger than the original

Licence
======================
(The MIT License)

Copyright (C) 2013 Leeroy Brun, www.leeroy.me

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.