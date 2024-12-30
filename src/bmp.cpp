/**
 @file
 @author Kavin Parakh <kkp56@drexel.edu>
 @date 10/25/2023
 @section Description

 Implementation of the Bitmap Class

 @note Most of the code has been reused from Professor's Boady implementation of bmp.cpp.
*/
#include <fstream>
#include <string>
#include "pixel.h"
#include "bmp.h"

/**
  @class bitMap
  @brief A class representing a bitmap image.
*/

/**
  @brief Constructor that creates a new bitmap object.
  @param w Width of the image.
  @param h Height of the image.
  @param filename Name of the file to save the bitmap image.
*/
bitMap::bitMap(int w, int h, const std::string& filename) {
    //Set Width and Height
    width = w;
    height = h;
    //Make the Header
    makeHeader();
    //Make the DIB
    makeDIB();
    //Color Array
    imageData = new pixel[numPixels()];
}

/**
  @brief Get the image data.
  @return Pointer to the array of pixel data.
*/
pixel* bitMap::getImageData() {
    return imageData;
}

/**
  @brief Get the width of the image.
  @return The width of the image.
*/
int bitMap::getWidth() const {
    return width;
}

/**
  @brief Get the height of the image.
  @return The height of the image.
*/
int bitMap::getHeight() const {
    return height;
}

/**
  @brief Destructor to delete the bitmap object.
  Frees memory allocated for header, DIB, and image data.
*/
bitMap::~bitMap(){
    delete[] header;
    delete[] DIB;
    delete[] imageData;
}

/**
  @brief Determine the number of pixels in the image.
  @return The total number of pixels in the image.
*/
int bitMap::numPixels(){
    return width*height;
}

/**
  @brief Set a pixel in the image to a specific color.
  @param pos Position of the pixel in the image data array.
  @param color The color to set the pixel to.
*/
void bitMap::setColor(int pos, pixel color){
    //Check Bounds
    if(pos < 0 || pos > numPixels()){
        return;//Can't set
    }
    imageData[pos] = color;
}

/**
  @brief Save the image to a file.
  @param filename Name of the file to save the image to.
*/
void bitMap::saveImage(std::string filename) {
    // Open File
    std::ofstream image(filename, std::ios::binary);

    // Write header and DIB
    image.write(header, headerSize);
    image.write(DIB, sizeDIB);

    // Calculate padding for each row
    // Source: https://stackoverflow.com/questions/21969456/how-to-calculate-struct-padding-in-c11-during-compile-time
    int paddingAmount = (4 - (width * 3) % 4) % 4;

    // Write all pixels with padding
    char* myPixel = new char[3];
    for (int i = height - 1; i >= 0; i--) {
    	for (int j = 0; j < width; j++) {
        	int idx = (i * width) + j;
            myPixel[2] = imageData[idx].getRed();
            myPixel[1] = imageData[idx].getGreen();
            myPixel[0] = imageData[idx].getBlue();
            image.write(myPixel, 3);
        }
        for (int k = 0; k < paddingAmount; k++) {
        	image.put(0);
    	}
    }

    // Close the image stream
    image.close();

    // Delete the allocated memory for pixel
    delete[] myPixel;

    // Exit
    return;
}

/* File Format Helpers*/

/**
  @brief Make the file header.
*/
void bitMap::makeHeader(){
    //Array to store the header
    header = new char[headerSize];
    //The header is 14 Bytes
    //The DIB is 40 bytes
    int offset = headerSize + sizeDIB;
    
    //Compute Values we will need
    //How many pixel does the image have
    int pixels = numPixels();
    //Each Pixel is another 3 bytes
    // Source: https://en.wikipedia.org/wiki/BMP_file_format
    int paddingAmount = (4 - (width * 3) % 4) % 4;
	int rowSize = width * 3 + paddingAmount;
	int totalPixelArraySize = rowSize * height;
	int totalBits = totalPixelArraySize + offset;
    
    //Make the Header
    //First 2 Bytes are BM for bitmap
    header[0] = 'B';
    header[1] = 'M';
    //Next 4 bytes are the total size of the file
    header[2] = getByte(totalBits,0);
    header[3] = getByte(totalBits,1);
    header[4] = getByte(totalBits,2);
    header[5] = getByte(totalBits,3);
    //Next for bits are 0 (reserved for other uses)
    header[6] = 0;
    header[7] = 0;
    header[8] = 0;
    header[9] = 0;
    //Last 4 bytes are offset
    //Where do the pixels start
    header[10] = getByte(offset,0);
    header[11] = getByte(offset,2);
    header[12] = getByte(offset,2);
    header[13] = getByte(offset,3);
    //Exit
    return;
}

/**
  @brief Make the file DIB.
*/
void bitMap::makeDIB()
{
    //Convert DPI to Pixels Per Meter
    int resolution = dpiToPPM(dpi);
    //Make array of bytes
    DIB = new char[sizeDIB];
    //Set Values
    //First 4 bytes are header size of this header (40)
    DIB[0] = getByte(40,0);
    DIB[1] = getByte(40,1);
    DIB[2] = getByte(40,2);
    DIB[3] = getByte(40,3);
    //Bitmap Width (4 bytes)
    DIB[4] = getByte(width,0);
    DIB[5] = getByte(width,1);
    DIB[6] = getByte(width,2);
    DIB[7] = getByte(width,3);
    //Height (4 bytes)
    DIB[8] = getByte(height,0);
    DIB[9] = getByte(height,1);
    DIB[10] = getByte(height,2);
    DIB[11] = getByte(height,3);
    //Color Plane (2 bytes) is always 1
    DIB[12] = 1;
    DIB[13] = 0;
    //Color Depth (2 bytes) we are using 24 (three 8 bit colors)
    DIB[14] = getByte(24,0);
    DIB[15] = getByte(24,1);
    //Compression (4 bytes) 0 means none
    DIB[16] = 0;
    DIB[17] = 0;
    DIB[18] = 0;
    DIB[19] = 0;
    //Uncompressed Size (4 bytes)
    //0 because we aren't using compression
    DIB[20] = 0;
    DIB[21] = 0;
    DIB[22] = 0;
    DIB[23] = 0;
    //Horizontal Resolution (4 bytes)
    //Pixel per meter
    DIB[24] = getByte(resolution, 0);
    DIB[25] = getByte(resolution, 1);
    DIB[26] = getByte(resolution, 2);
    DIB[27] = getByte(resolution, 3);
    //Vertical Resolution (4 bytes)
    //Pixel per meter
    DIB[28] = getByte(resolution, 0);
    DIB[29] = getByte(resolution, 1);
    DIB[30] = getByte(resolution, 2);
    DIB[31] = getByte(resolution, 3);
    //Color Pallet (4 bytes)
    //0 means all
    DIB[32] = 0;
    DIB[33] = 0;
    DIB[34] = 0;
    DIB[35] = 0;
    //Number of important colors
    //0 mean all equal
    DIB[36] = 0;
    DIB[37] = 0;
    DIB[38] = 0;
    DIB[39] = 0;
    //Exit the Function
    return;
}

/**
  @brief Get a specific byte out of an integer for making the header array.
  @param value Integer value to extract the byte from.
  @param byte The byte position to extract.
  @return The extracted byte.
*/
char bitMap::getByte(int value, int byte)
{
    int newValue = value;
    
    unsigned char rem;
    for(int i=0; i <= byte; i++)
    {
        rem = static_cast<unsigned char>( newValue%256 );
        newValue = newValue/256;
    }
    return rem;
}

/**
  @brief Convert DPI to Pixels Per Meter.
  @param dpi Dots Per Inch value to convert.
  @return Pixels Per Meter.
*/
int bitMap::dpiToPPM(int dpi)
{
    float inchesPerMeter = 39.3701/1;
    float convert = dpi*inchesPerMeter;
    return static_cast<int>(convert);
}
