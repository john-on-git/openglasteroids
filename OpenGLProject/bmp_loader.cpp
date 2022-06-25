#include <iostream>
#include <fstream>
#include <map>

#include "bmp_loader.hpp"

using namespace std;

/// <summary>
/// create an array of RGB triples from bmp file
/// </summary>
/// <param name="path">the path of the file</param>
/// <returns>an array of the image's rgb values</returns>
bmp* load_bmp(std::string path)
{
    struct dibheader {
        unsigned int size;
        unsigned short int bpp; //bits per pixel
        unsigned int compresionMethod;
        unsigned int imageSize;
        unsigned int horizontalResolution;
        unsigned int verticalResolution;
        unsigned int colorPaletteSize;
        unsigned int numImportantColors;
    };

    ifstream file(path, std::ios::binary);
    bmp* o = NULL;

    if (file.is_open()) {
        //read the file header
        if (file.get() == 0x42 && file.get() == 0x4D) //verify the header field
        {
            file.ignore(sizeof(unsigned int) + sizeof(unsigned int)); //skip file size and reserved bytes

            //read content offset
            unsigned int contentStart = 0;
            file.read(((char*)&contentStart), sizeof(unsigned int));

            //read the DIB header
            dibheader dib = {};
            file.read(((char*)&dib.size), sizeof(unsigned int)); //read the size
            o = new bmp{ 0, 0, NULL };
            switch (dib.size) {
                //not implemented
            case 12:
                break;
            case 64:
                break;
            case 108:
                break;
            case 124: //BITMAPV5HEADER
                break;

            case 40: //BITMAPINFOHEADER
                file.read(((char*)&o->width), sizeof(int)); //read the width
                file.read(((char*)&o->height), sizeof(int)); //read the height

                file.ignore(2); //ignore fixed value (colour planes)
                file.read(((char*)&dib.bpp), sizeof(unsigned short int)); //read the bits per pixel
                file.read(((char*)&dib.compresionMethod), sizeof(unsigned int)); //read the compression method
                file.read(((char*)&dib.imageSize), sizeof(unsigned int)); //read the image size
                file.read(((char*)&dib.horizontalResolution), sizeof(unsigned int)); //read the horizontal resolution
                file.read(((char*)&dib.verticalResolution), sizeof(unsigned int)); //read the vertical resolution
                file.read(((char*)&dib.colorPaletteSize), sizeof(unsigned int)); //read the number of colors in the palette
                file.read(((char*)&dib.numImportantColors), sizeof(unsigned int)); //read the number of important colors
                break;
            default:
                break;
            }
            //read the bit masks

            //read the colour palette

            //"The number of entries in the palette is either 2^n (where n is the number of bits per pixel)
            //or a smaller number specified in the header"
            unsigned char paddingAfterLine = o->width % 4 == 0 ? 0 : 4ll - (o->width % 4);
            if (dib.bpp <= 8) { //1, 4, & 8 bit bmps always contain a color palette
                size_t paletteSize = (dib.colorPaletteSize == 0 ? (size_t)pow(2, dib.bpp) : dib.colorPaletteSize);
                auto palette = new unsigned char* [paletteSize]; //allocate memory
                for (size_t i = 0;i < paletteSize;i++) //for each colour
                {
                    palette[i] = new unsigned char[3]; //allocate memory
                    palette[i][2] = file.get(); //blue
                    palette[i][1] = file.get(); //green
                    palette[i][0] = file.get(); //red
                    file.ignore();              //skip the fourth
                }
                cout << "palette" << endl;
                for (size_t i = 0;i < paletteSize;i++)
                {
                    cout << i << ": " << endl
                        << "\tR: " << (int)palette[i][0] << endl
                        << "\tG: " << (int)palette[i][1] << endl
                        << "\tB: " << (int)palette[i][2] << endl;
                }

                file.seekg(contentStart); //seek to start of content
                switch (dib.bpp) {
                case 1:
                {
                    o->content = new unsigned char[o->width * o->height * PX_SIZE]; //allocate memory for content
                    char byte = file.get();
                    for (char i = 0;i < 8;i++)
                        if (((byte >> i) & 1) == 1) //if the bit at position i is true
                            o->content[0] = 0x00,
                            o->content[1] = 0x00,
                            o->content[2] = 0x00;
                        else
                            o->content[0] = 0xFF,
                            o->content[1] = 0xFF,
                            o->content[2] = 0xFF;
                }
                break;
                case 4:
                    o->content = new unsigned char[(o->width + paddingAfterLine) * o->height * PX_SIZE]; //allocate memory for content

                    for (size_t i = 0;i <= o->height;i++) //copy all values into array
                    {
                        for (size_t j = 0;j <= o->width / 2;j++)
                        {
                            unsigned char byte = file.get(); //get the byte from file

                            //then separate out the two 4-bit values within
                            unsigned char first = (byte >> 4) & 0x0F;
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 0] = palette[first][0];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 1] = palette[first][1];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 2] = palette[first][2];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 3] = 0xFF;

                            unsigned char second = byte & 0x0F;
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 3] = palette[second][0];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 4] = palette[second][1];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 5] = palette[second][2];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 6] = 0xFF;
                        }
                        file.ignore(paddingAfterLine); //move to end of row.
                    }
                    break;
                case 8:
                    o->content = new unsigned char[(o->width + paddingAfterLine) * o->height * PX_SIZE]; //allocate memory for content
                    for (size_t i = 0;i < o->height;i++) //copy all values into array
                    {
                        for (size_t j = 0;j < o->width;j++)
                        {
                            auto color = palette[file.get()];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 0] = color[0];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 1] = color[1];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 2] = color[2];
                            o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * 2 * PX_SIZE) + 3] = 0xFF;
                        }
                        file.ignore(paddingAfterLine); //move to end of row.
                    }
                    break;
                }
            }
            else
            {
                file.seekg(contentStart); //seek to start of content
                switch (dib.bpp)
                {
                case 16:
                    break;
                case 24:
                {
                    o->content = new unsigned char[(o->width + paddingAfterLine) * o->height * PX_SIZE]; //allocate memory for content
                    for (size_t i = 0;i <= o->height;i++) //copy all values into array
                    {
                        for (size_t j = 0;j <= o->width;j++)
                        {
                            file.read((char*)o->content + (i * (o->width + paddingAfterLine) * PX_SIZE) + (j * PX_SIZE) + 2, sizeof(unsigned char)); //blue
                            file.read((char*)o->content + (i * (o->width + paddingAfterLine) * PX_SIZE) + (j * PX_SIZE) + 1, sizeof(unsigned char)); //green
                            file.read((char*)o->content + (i * (o->width + paddingAfterLine) * PX_SIZE) + (j * PX_SIZE) + 0, sizeof(unsigned char)); //red
                            //o->content[(i * (o->width + paddingAfterLine) * PX_SIZE) + (j * PX_SIZE) + 3] = 0xFF;
                        }
                    }
                    file.ignore(paddingAfterLine); //move to end of row.
                }
                break;
                default:
                    break;
                }
            }
        }
        file.close();
    }
    return o;
}