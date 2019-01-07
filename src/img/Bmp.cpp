#include "Bmp.h"

/*
 * dig = Binärisierung des Bildes
 * erod = erodiertes Bild
 */

//constructor: open bmp file and get width and height from geader
Bmp::Bmp(const char* filename) { 
    this->filename = filename;
    f = fopen(filename, "rb");
    unsigned char info[54]; // read the 54-byte header
    fread(info, sizeof(unsigned char), 54, f); 

    // extract image height and width from header
    width = *(int*)&info[18];
    height = *(int*)&info[22];

    // extract start index from image
    startindex = *(int*)&info[10];

    int* trash[startindex - 54];
    // read rest of data until image begins
    fread(trash, sizeof(unsigned char), startindex - 54, f); 
}

//read bmp file and save rgb values in 1D int-array
unsigned char* Bmp::readRGB() {
    printf("Read .bmp file... ");
    fflush(stdout);
    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
    printf(" done!\n");
    fflush(stdout);

    return data; 
}

//convert rgb (1D unsigned char*-array) to grayscale (1D int-array)
int* Bmp::grayscaleFromRGB(unsigned char* rgb) {
    int* grayscaledImage = new int[width * height]; 
    
    printf("Converting .bmp file in grayscale...");
    fflush(stdout);
    unsigned char r, g, b;

    //begin at 24
    for(int i = 0; i < width * height; i++) grayscaledImage[i] = grayscale(rgb[3*i], rgb[3*i+1], rgb[3*i+2]);

    printf(" done!\n");
    fflush(stdout);
    
    return grayscaledImage;
}

//Convert a grayscale picture (1D int-array) to a binary picture (1D int-array)
int* Bmp::digitalize(int* gray) {
    int* dig = new int[width * height];

    printf("Converting grayscale to binary...");
    fflush(stdout);

    for(int i = 0; i < width * height; i++) gray[i] > THRESHOLD ? dig[i] = 1 : dig[i] = 0; 
    
    printf(" done!\n");
    fflush(stdout);

    return dig;
}

//calculate grayscale from rgb values
unsigned char Bmp::grayscale(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char gray;
    
    gray = 0.299 * r + 0.587 * g + 0.114 * b;
        
    return gray;
}

int* Bmp::erodeBinaryPic(int* dig) {
    //erode pic
    int* erod = new int[width * height];

    //fill border with 0
    //top
    for(int i = 0; i < height; i++) dig[i] = 0;
    //bottom
    for(int i = 0; i < height; i++) dig[width * (height-1) + i] = 0;
    //left
    for(int i = 0; i < height; i++) dig[i*width] = 0;
    //right
    for(int i = 0; i < height; i++) dig[i*width-1] = 0;

    //iterate over picture with 3x3; a_ij is element in the middle
    for(int i = width + 1; i < width * (height - 1) - 2; i++) {
        int ones = 0;
        if(dig[i - width - 1] == 1) ones++;
        if(dig[i - width] == 1) ones++;
        if(dig[i - width + 1] == 1) ones++;
        if(dig[i - 1] == 1) ones++;
        if(dig[i] == 1) ones++;
        if(dig[i + 1] == 1) ones++;
        if(dig[i + width - 1] == 1) ones++;
        if(dig[i + width] == 1) ones++;
        if(dig[i + width + 1] == 1) ones++;

        ones <= m ? erod[i] = 0 : erod[i] = 1;      
    }

    return erod;
}

//Calculate the subtraction picture
int* Bmp::buildEarthContour(int* dig, int* erod) {
    int* diff = new int[width * height];

    for(int i = 0; i < width * height; i++) diff[i] = dig[i] - erod[i];

    return diff;
}

//Calculate the circle middle point (Least-squares circle fit)
Matrix Bmp::calcCircleMiddlePoint(int* diff) {
    int ones = 0; //numbers of needed x,y pairs = numbers of ones
    int i = 0; // counter for all loops
    int s_x = 0; 
    int s_y = 0;
    long s_uu = 0; 
    long s_uv = 0;
    long s_vv = 0; 
    long s_uuu = 0;
    long s_uvv = 0;
    long s_vvv = 0; 
    long s_vuu = 0;

    for(;i < width * height; i++) 
        if(diff[i] == 1) ones++;

    int* xyuv = new int[4 * ones];

    //calculate sums
    for(int y = 0, i = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if(diff[y * width + x] == 1) {
                xyuv[i] = x;
                xyuv[i + 1] = y;
                s_x += x;
                s_y += y;
                i += 4;
            }
        }
    }

    //calculate average values of x and y
    double x_avg = s_x / ones;
    double y_avg = s_y / ones;

    for(i = 2; i < 4 * ones; i+=4) {
        xyuv[i] = xyuv[i - 2] - x_avg; // u_i = x_i - x_avg
        xyuv[i+1] = xyuv[i - 1] - y_avg; // v_i = y_i - y_avg
    }

    //calculate sums
    for(i = 2; i < 4 * ones; i+=4) {
        s_uu += pow(xyuv[i], 2);
        s_uv += xyuv[i] * xyuv[i+1];
        s_vv += pow(xyuv[i+1], 2);
        s_uuu += pow(xyuv[i], 3);
        s_uvv += xyuv[i] * pow(xyuv[i+1], 2);
        s_vvv += pow(xyuv[i+1], 3);
        s_vuu += xyuv[i+1] * pow(xyuv[i], 2);
    }

    //u_c * s_uu + v_c * s_uv = 0.5 * (s_uuu + s_uvv)
    //u_c * s_uv + v_c * s_vv = 0.5 * (s_vvv + s_vuu)

    //solve linar equation system with matrix

    Matrix a(2,2);
    Matrix b(2,1);

    a(0,0) = s_uu;
    a(0,1) = s_uv;
    a(1,0) = s_uv;
    a(1,1) = s_vv;

    b(0,0) = 0.5 * (s_uuu + s_uvv);
    b(1,0) = 0.5 * (s_vvv + s_vuu);

    Matrix c = a.inv() * b;

    c(0,0) = (c(0,0) + x_avg); //x_center
    c(1,0) = (c(1,0) + y_avg); //y_center

    return c;
}

//Calculate normed earth vector
Vec3D Bmp::calcEarthVec(Matrix a, double f) {
    //-512 to shift ccordinate system in the middle of the sensor 
    return (new Vec3D((a(0,0) - 512) * p_length / f * L, (a(1,0) - 512) * p_length / f * L, L))->normVec();
}

//Calculate filed of view and focal of length
double Bmp::calcFieldOfViewAndFocalOfLength(int* diff) {
    int firstLine, lastLine;
    bool keeper = true;

    //get top and bottom pixel of earth
    for(int i = 0; i < width * height; i++) {
        if(keeper && diff[i] == 1) {
            keeper = false;
            firstLine = i / width;
        }
        lastLine = i;
    }
    lastLine = lastLine / width;

    //theorem of intersecting lines
    double d = p_length * (lastLine - firstLine);
    double f = d * L / D;

    printf("d = %f\nf = %f\n", d, f);

    return f;
}

//Create txt-file and write binary array in it
void Bmp::writeToTxt(int* pic, string name) {
    printf("Create %s...", name.c_str());
    ofstream file;
    file.open(name);

    for(int i = 0; i < width * height; i++) {
        if(i != 0 && i % width == 0) file << endl;
        file << pic[i];
    }

    file.close();
    printf(" done\n!");
}

//create bmp-file and write data of subtraction picture in it
void Bmp::writeToBmp(int* pic, string name) {
    printf("Create %s...", name.c_str());

    FILE *f;
    unsigned char *img = new unsigned char[3 * width * height];
    memset(img,0,3*width*height);
    int filesize = 54 + 3*width*height;  

    int x, y;
    for(int i=0; i<width*height; i++) {
        x=i%width; y=(height-1)-i/width;
        int mono = pic[i]*255;
        if (mono > 255) mono=255;
        img[(x+y*width)*3+2] = (unsigned char)(mono);
        img[(x+y*width)*3+1] = (unsigned char)(mono);
        img[(x+y*width)*3+0] = (unsigned char)(mono);
    }

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       width     );
    bmpinfoheader[ 5] = (unsigned char)(       width>> 8 );
    bmpinfoheader[ 6] = (unsigned char)(       width>>16 );
    bmpinfoheader[ 7] = (unsigned char)(       width>>24 );
    bmpinfoheader[ 8] = (unsigned char)(       height    );
    bmpinfoheader[ 9] = (unsigned char)(       height>> 8);
    bmpinfoheader[10] = (unsigned char)(       height>>16);
    bmpinfoheader[11] = (unsigned char)(       height>>24);

    f = fopen(name.c_str(),"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<height; i++) {
        fwrite(img+(width*(height-i-1)*3),3,width,f);
        fwrite(bmppad,1,(4-(width*3)%4)%4,f);
    }

    delete(img);
    fclose(f);

    printf(" done!\n");
}

int Bmp::getWidth() {
    return width;
}

int Bmp::getHeight() {
    return height;
}