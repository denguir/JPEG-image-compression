# JPEG-image-compression

Encode an image (lena_256x256.raw) and compress it into a bitStream (bitStream.txt) using the JPEG algorithm, which consists of:

--> input: lena_256x256.raw  
1- Applying Blockwise DCT (with blockSize=8)  
2- Applying block quantization (The quantization used is the 50% JPEG by default)  
3- Encoding DC coefficients using Delta Encoding  
4- Encoding AC coefficients using Run Length Encoding   
5- Creating a bitStream from (3) and (4), using Exp-Golomb encoding  
--> output: bitstream  
  
The bitStream can then be decompressed and decoded to retrieve the original image (lena_out.raw):  
--> input: bitstream  
1- Read and decode the bitStream using Exp-Golomb decoding  
2- Decode AC coefficients using Run Length Decoding  
3- Decode DC coefficients using Delta Decoding  
4- Apply inverse quantization (loss of information here)  
5- Apply Blockwise inverse DCT  
--> output: lena_out.raw  
  
To compile the code, type:  
g++ main.cpp image_lab6.cpp image_lab5.cpp image_lab4.cpp image_lab3.cpp image_lab2.cpp image_lab1.cpp -std=gnu++11 -o image  

You can also visualize intermediate results by running the appropriate main. For example, to compile image_lab3_main.cpp, type:  
g++ image_lab3_main.cpp image_lab3.cpp image_lab2.cpp image_lab1.cpp -std=gnu++11 -o image3  
