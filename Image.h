#pragma once
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Image {
    vector<unsigned char> blue;
    vector<unsigned char> green;
    vector<unsigned char> red;
    vector<unsigned char> bc;
    vector<unsigned char> gc;
    vector<unsigned char> rc;
    char* pixelData{};
    char idLength{};
    char colorMapType{};
    char dataTypeCode{};
    short colorMapOrigin{};
    short colorMapLength{};
    char colorMapDepth{};
    short xOrigin{};
    short yOrigin{};
    short width{};
    short height{};
    char bitsPerPixel{};
    char imageDescriptor{};

    Image() = default;

    explicit Image(const string& filename) {
        ifstream inFile(filename, ios_base::binary);
        if (inFile.is_open()) {
            inFile.read(&idLength, 1);
            inFile.read(&colorMapType, 1);
            inFile.read(&dataTypeCode, 1);
            inFile.read((char*)(&colorMapOrigin), 2);
            inFile.read((char*)(&colorMapLength), 2);
            inFile.read(&colorMapDepth, 1);
            inFile.read((char*)(&xOrigin), 2);
            inFile.read((char*)(&yOrigin), 2);
            inFile.read((char*)(&width), 2);
            inFile.read((char*)(&height), 2);
            inFile.read(&bitsPerPixel, 1);
            inFile.read(&imageDescriptor, 1);
            pixelData = new char[width * height * 3];
            inFile.read(pixelData, width*height*3);
            for (unsigned int i = 0; i < height; i++) {
                for (unsigned int j = 0; j < width; j++) {
                    unsigned int index = (i*width+j) * 3;
                    blue.push_back(pixelData[index]);
                    green.push_back(pixelData[index+1]);
                    red.push_back(pixelData[index+2]);
                }
            }
        }
        bc = blue;
        gc = green;
        rc = red;
    }

    Image& operator=(const Image& orig) {
        if (this != &orig) {
            this->bc = orig.bc;
            this->gc = orig.gc;
            this->rc = orig.rc;
            this->idLength = orig.idLength;
            this->colorMapType = orig.colorMapType;
            this->dataTypeCode = orig.dataTypeCode;
            this->colorMapOrigin = orig.colorMapOrigin;
            this->xOrigin = orig.xOrigin;
            this->yOrigin = orig.yOrigin;
            this->width = orig.width;
            this->height = orig.height;
            this->bitsPerPixel = orig.bitsPerPixel;
            this->imageDescriptor = orig.imageDescriptor;
            this->blue = orig.blue;
            this->green = orig.green;
            this->red = orig.red;
        }
        return *this;
    }
};

Image multiply(const string& topLayer, const string& botLayer, Image* image);
Image subtract(const string& botLayer, const string& topLayer, Image* image);
Image screen(const string& topLayer, const string& botLayer, Image* image);
Image overlay(const string& topLayer, const string& botLayer, Image* image);
Image addgreen(const string& file, int val, Image* image);
Image addred(const string& file, int val, Image* image);
Image addblue(const string& file, int val, Image* image);
Image scalered(const string& file, int val, Image* image);
Image scalegreen(const string& file, int val, Image* image);
Image scaleblue(const string& file, int val, Image* image);
Image blueonly(const string& file, Image* image);
Image greenonly(const string& file, Image* image);
Image redonly(const string& file, Image* image);
Image combine(const string& bFile, const string& gFile, const string& rFile, Image* image);
Image flip(const string& file, Image* imag);
void write(const string& outFile, const Image& file);

void write(const string& outFile, const Image* file) {
    ofstream oFile(outFile, ios_base::binary);
    if (oFile.is_open()) {
        oFile.write(&file->idLength, 1);
        oFile.write(&file->colorMapType, 1);
        oFile.write(&file->dataTypeCode, 1);
        oFile.write((char*)(&file->colorMapOrigin), 2);
        oFile.write((char*)(&file->colorMapLength), 2);
        oFile.write(&file->colorMapDepth, 1);
        oFile.write((char*)(&file->xOrigin), 2);
        oFile.write((char*)(&file->yOrigin), 2);
        oFile.write((char*)(&file->width), 2);
        oFile.write((char*)(&file->height), 2);
        oFile.write(&file->bitsPerPixel, 1);
        oFile.write(&file->imageDescriptor, 1);
        for (int i = 0; i < file->blue.size(); i++) {
            oFile.write((char*)&file->bc.at(i), 1);
            oFile.write((char*)&file->gc.at(i), 1);
            oFile.write((char*)&file->rc.at(i), 1);
        }
    }
}

Image multiply(const string& topLayer, const string& botLayer, Image* image) {
    if (image == nullptr) {
        Image bot(botLayer);
        Image top(topLayer);
        float tempB1;
        float tempG1;
        float tempR1;
        float tempB2;
        float tempG2;
        float tempR2;
        vector<unsigned char> normB2;
        vector<unsigned char> normG2;
        vector<unsigned char> normR2;
        for (int i = 0; i < top.blue.size(); i++) {
            tempB1 = (float)top.blue.at(i) / 255;
            tempG1 = (float)top.green.at(i) / 255;
            tempR1 = (float)top.red.at(i) / 255;
            tempB2 = (float)bot.blue.at(i) / 255;
            tempG2 = (float)bot.green.at(i) / 255;
            tempR2 = (float)bot.red.at(i) / 255;
            tempB2 = tempB1*tempB2*255+0.5f;
            tempG2 = tempG1*tempG2*255+0.5f;
            tempR2 = tempR1*tempR2*255+0.5f;
            normB2.push_back((unsigned char)tempB2);
            normG2.push_back((unsigned char)tempG2);
            normR2.push_back((unsigned char)tempR2);
        }
        bot.bc = normB2;
        bot.gc = normG2;
        bot.rc = normR2;
        return bot;
    }
    else {
        Image top(topLayer);
        float tempB1;
        float tempG1;
        float tempR1;
        float tempB2;
        float tempG2;
        float tempR2;
        vector<unsigned char> normB2;
        vector<unsigned char> normG2;
        vector<unsigned char> normR2;
        for (int i = 0; i < top.blue.size(); i++) {
            tempB1 = (float)top.blue.at(i) / 255;
            tempG1 = (float)top.green.at(i) / 255;
            tempR1 = (float)top.red.at(i) / 255;
            tempB2 = (float)image->bc.at(i) / 255;
            tempG2 = (float)image->gc.at(i) / 255;
            tempR2 = (float)image->rc.at(i) / 255;
            tempB2 = tempB1*tempB2*255+0.5f;
            tempG2 = tempG1*tempG2*255+0.5f;
            tempR2 = tempR1*tempR2*255+0.5f;
            normB2.push_back((unsigned char)tempB2);
            normG2.push_back((unsigned char)tempG2);
            normR2.push_back((unsigned char)tempR2);
        }
        image->bc = normB2;
        image->gc = normG2;
        image->rc = normR2;
        return *image;
    }

}

Image subtract(const string& botLayer, const string& topLayer, Image* image) {
    if (image == nullptr) {
        Image top(topLayer);
        Image bot(botLayer);
        int bVal;
        int gVal;
        int rVal;
        vector<unsigned char> blueChan;
        vector<unsigned char> greenChan;
        vector<unsigned char> redChan;
        for (int i = 0; i < bot.blue.size(); i++) {
            bVal = top.blue.at(i) - bot.blue.at(i);
            gVal = top.green.at(i) - bot.green.at(i);
            rVal = top.red.at(i) - bot.red.at(i);
            if (bVal < 0) {
                bVal = 0;
            }
            else if (bVal > 255) {
                bVal = 255;
            }
            if (gVal < 0) {
                gVal = 0;
            }
            else if (gVal > 255) {
                gVal = 255;
            }
            if (rVal < 0) {
                rVal = 0;
            }
            else if (rVal > 255) {
                rVal = 255;
            }
            blueChan.push_back((unsigned char)bVal);
            greenChan.push_back((unsigned char)gVal);
            redChan.push_back((unsigned char)rVal);
        }
        top.bc = blueChan;
        top.gc = greenChan;
        top.rc = redChan;
        return top;
    }
    else {
        Image bot(botLayer);
        int bVal;
        int gVal;
        int rVal;
        vector<unsigned char> blueChan;
        vector<unsigned char> greenChan;
        vector<unsigned char> redChan;
        for (int i = 0; i < bot.blue.size(); i++) {
            bVal = image->bc.at(i) - bot.blue.at(i);
            gVal = image->gc.at(i) - bot.green.at(i);
            rVal = image->rc.at(i) - bot.red.at(i);
            if (bVal < 0) {
                bVal = 0;
            }
            if (gVal < 0) {
                gVal = 0;
            }
            if (rVal < 0) {
                rVal = 0;
            }
            blueChan.push_back((unsigned char)bVal);
            greenChan.push_back((unsigned char)gVal);
            redChan.push_back((unsigned char)rVal);
        }
        image->bc = blueChan;
        image->gc = greenChan;
        image->rc = redChan;
        return *image;
    }
}

Image screen(const string& topLayer, const string& botLayer, Image* image) {
    if (image != nullptr) {
        Image top(topLayer);
        float tempB1;
        float tempG1;
        float tempR1;
        float tempB2;
        float tempG2;
        float tempR2;
        vector<unsigned char> normB2;
        vector<unsigned char> normG2;
        vector<unsigned char> normR2;
        for (int i = 0; i < top.blue.size(); i++) {
            tempB1 = (float)top.blue.at(i) / 255;
            tempG1 = (float)top.green.at(i) / 255;
            tempR1 = (float)top.red.at(i) / 255;
            tempB2 = (float)image->bc.at(i) / 255;
            tempG2 = (float)image->gc.at(i) / 255;
            tempR2 = (float)image->rc.at(i) / 255;
            tempB2 = (1 - ((1-tempB1)*(1-tempB2))) * 255 + 0.5f;
            tempG2 = (1 - ((1-tempG1)*(1-tempG2))) * 255 + 0.5f;
            tempR2 = (1 - ((1-tempR1)*(1-tempR2))) * 255 + 0.5f;
            normB2.push_back((unsigned char)tempB2);
            normG2.push_back((unsigned char)tempG2);
            normR2.push_back((unsigned char)tempR2);
        }
        image->bc = normB2;
        image->gc = normG2;
        image->rc = normR2;
        return *image;
    }
    else {
        Image top(topLayer);
        Image bot(botLayer);
        float tempB1;
        float tempG1;
        float tempR1;
        float tempB2;
        float tempG2;
        float tempR2;
        vector<unsigned char> normB2;
        vector<unsigned char> normG2;
        vector<unsigned char> normR2;
        for (int i = 0; i < top.blue.size(); i++) {
            tempB1 = (float)top.blue.at(i) / 255;
            tempG1 = (float)top.green.at(i) / 255;
            tempR1 = (float)top.red.at(i) / 255;
            tempB2 = (float)bot.blue.at(i) / 255;
            tempG2 = (float)bot.green.at(i) / 255;
            tempR2 = (float)bot.red.at(i) / 255;
            tempB2 = (1 - ((1-tempB1)*(1-tempB2))) * 255 + 0.5f;
            tempG2 = (1 - ((1-tempG1)*(1-tempG2))) * 255 + 0.5f;
            tempR2 = (1 - ((1-tempR1)*(1-tempR2))) * 255 + 0.5f;
            normB2.push_back((unsigned char)tempB2);
            normG2.push_back((unsigned char)tempG2);
            normR2.push_back((unsigned char)tempR2);
        }
        bot.bc = normB2;
        bot.gc = normG2;
        bot.rc = normR2;
        return bot;
    }

}

Image overlay(const string& topLayer, const string& botLayer, Image* image) {
    if (image == nullptr) {
        Image bot(botLayer);
        Image top(topLayer);
        float tempB1;
        float tempG1;
        float tempR1;
        float tempB2;
        float tempG2;
        float tempR2;
        vector<unsigned char> normB2;
        vector<unsigned char> normG2;
        vector<unsigned char> normR2;
        for (int i = 0; i < top.blue.size(); i++) {
            tempB1 = (float)top.blue.at(i) / 255;
            tempG1 = (float)top.green.at(i) / 255;
            tempR1 = (float)top.red.at(i) / 255;
            tempB2 = (float)bot.blue.at(i) / 255;
            tempG2 = (float)bot.green.at(i) / 255;
            tempR2 = (float)bot.red.at(i) / 255;
            if (tempB2 <= 0.5) {
                tempB2 = 2*tempB1*tempB2*255+0.5f;
            }
            else {
                tempB2 = (1-(2*(1-tempB1)*(1-tempB2)))*255+0.5f;
            }
            if (tempG2 <= 0.5) {
                tempG2 = 2*tempG1*tempG2*255+0.5f;
            }
            else {
                tempG2 = (1-(2*(1-tempG1)*(1-tempG2)))*255+0.5f;
            }
            if (tempR2 <= 0.5) {
                tempR2 = 2*tempR1*tempR2*255+0.5f;
            }
            else {
                tempR2 = (1-(2*(1-tempR1)*(1-tempR2)))*255+0.5f;
            }
            normB2.push_back((unsigned char)tempB2);
            normG2.push_back((unsigned char)tempG2);
            normR2.push_back((unsigned char)tempR2);
        }
        top.bc = normB2;
        top.gc = normG2;
        top.rc = normR2;
        return top;
    }
    else {
        Image top(topLayer);
        float tempB1;
        float tempG1;
        float tempR1;
        float tempB2;
        float tempG2;
        float tempR2;
        vector<unsigned char> normB2;
        vector<unsigned char> normG2;
        vector<unsigned char> normR2;
        for (int i = 0; i < top.blue.size(); i++) {
            tempB1 = (float)top.blue.at(i) / 255;
            tempG1 = (float)top.green.at(i) / 255;
            tempR1 = (float)top.red.at(i) / 255;
            tempB2 = (float)image->bc.at(i) / 255;
            tempG2 = (float)image->gc.at(i) / 255;
            tempR2 = (float)image->rc.at(i) / 255;
            if (tempB2 <= 0.5) {
                tempB2 = 2*tempB1*tempB2*255+0.5f;
            }
            else {
                tempB2 = (1-(2*(1-tempB1)*(1-tempB2)))*255+0.5f;
            }
            if (tempG2 <= 0.5) {
                tempG2 = 2*tempG1*tempG2*255+0.5f;
            }
            else {
                tempG2 = (1-(2*(1-tempG1)*(1-tempG2)))*255+0.5f;
            }
            if (tempR2 <= 0.5) {
                tempR2 = 2*tempR1*tempR2*255+0.5f;
            }
            else {
                tempR2 = (1-(2*(1-tempR1)*(1-tempR2)))*255+0.5f;
            }
            normB2.push_back((unsigned char)tempB2);
            normG2.push_back((unsigned char)tempG2);
            normR2.push_back((unsigned char)tempR2);
        }
        image->bc = normB2;
        image->gc = normG2;
        image->rc = normR2;
        return *image;
    }

}

Image addgreen(const string& file, int val, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        vector<unsigned char> greenC;
        int gVal = 0;
        for (unsigned char i : iFile.gc) {
            gVal = (int)i + val;
            if (gVal > 255) {
                gVal = 255;
            }
            else if (gVal < 0) {
                gVal = 0;
            }
            greenC.push_back((unsigned char)gVal);
        }
        iFile.gc = greenC;
        return iFile;
    }
    else {
        vector<unsigned char> greenC;
        int gVal = 0;
        for (unsigned char i : image->gc) {
            gVal = (int)i + val;
            if (gVal > 255) {
                gVal = 255;
            }
            else if (gVal < 0) {
                gVal = 0;
            }
            greenC.push_back((unsigned char)gVal);
        }
        image->gc = greenC;
        return *image;
    }
}

Image addred(const string& file, int val, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        vector<unsigned char> redC;
        int rVal = 0;
        for (unsigned char i : iFile.rc) {
            rVal = (int)i + val;
            if (rVal > 255) {
                rVal = 255;
            }
            else if (rVal < 0) {
                rVal = 0;
            }
            redC.push_back((unsigned char)rVal);
        }
        iFile.rc = redC;
        return iFile;
    }
    else {
        vector<unsigned char> redC;
        int rVal = 0;
        for (unsigned char i : image->rc) {
            rVal = (int)i + val;
            if (rVal > 255) {
                rVal = 255;
            }
            else if (rVal < 0) {
                rVal = 0;
            }
            redC.push_back((unsigned char)rVal);
        }
        image->rc = redC;
        return *image;
    }
}

Image addblue(const string& file, int val, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        vector<unsigned char> blueC;
        int bVal = 0;
        for (unsigned char i : iFile.bc) {
            bVal = (int)i + val;
            if (bVal > 255) {
                bVal = 255;
            }
            else if (bVal < 0) {
                bVal = 0;
            }
            blueC.push_back((unsigned char)bVal);
        }
        iFile.bc = blueC;
        return iFile;
    }
    else {
        vector<unsigned char> blueC;
        int bVal = 0;
        for (unsigned char i : image->bc) {
            bVal = (int)i + val;
            if (bVal > 255) {
                bVal = 255;
            }
            else if (bVal < 0) {
                bVal = 0;
            }
            blueC.push_back((unsigned char)bVal);
        }
        image->bc = blueC;
        return *image;
    }
}

Image scalered(const string& file, int val, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        float tempR2;
        vector<unsigned char> normR2;
        for (unsigned char i : iFile.rc) {
            tempR2 = (float) i;
            tempR2 = (tempR2 * (float) val);
            if (tempR2 > 255) {
                tempR2 = 255;
            }
            normR2.push_back((unsigned char) tempR2);
        }
        iFile.rc = normR2;
        return iFile;
    }
    else {
        float tempR2;
        vector<unsigned char> normR2;
        for (unsigned char i : image->rc) {
            tempR2 = (float) i;
            tempR2 = (tempR2 * (float) val);
            if (tempR2 > 255) {
                tempR2 = 255;
            }
            normR2.push_back((unsigned char) tempR2);
        }
        image->rc = normR2;
        return *image;
    }
}

Image scalegreen(const string& file, int val, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        float tempG2;
        vector<unsigned char> normG2;
        for (unsigned char i : iFile.gc) {
            tempG2 = (float) i;
            tempG2 = (tempG2 * (float) val);
            if (tempG2 > 255) {
                tempG2 = 255;
            }
            normG2.push_back((unsigned char) tempG2);
        }
        iFile.gc = normG2;
        return iFile;
    }
    else {
        float tempG2;
        vector<unsigned char> normG2;
        for (unsigned char i : image->gc) {
            tempG2 = (float) i;
            tempG2 = (tempG2 * (float) val);
            if (tempG2 > 255) {
                tempG2 = 255;
            }
            normG2.push_back((unsigned char) tempG2);
        }
        image->gc = normG2;
        return *image;
    }
}

Image scaleblue(const string& file, int val, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        float tempB2;
        vector<unsigned char> normB2;
        for (unsigned char i : iFile.bc) {
            tempB2 = (float) i;
            tempB2 = (tempB2 * (float) val);
            if (tempB2 > 255) {
                tempB2 = 255;
            }
            normB2.push_back((unsigned char) tempB2);
        }
        iFile.bc = normB2;
        return iFile;
    }
    else {
        float tempB1;
        vector<unsigned char> normB1;
        for (unsigned char i : image->bc) {
            tempB1 = (float) i;
            tempB1 = (tempB1 * (float) val);
            if (tempB1 > 255) {
                tempB1 = 255;
            }
            normB1.push_back((unsigned char) tempB1);
        }
        image->bc = normB1;
        return *image;
    }
}

Image blueonly(const string& file, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        iFile.gc = iFile.bc;
        iFile.rc = iFile.bc;
        return iFile;
    }
    else {
        image->gc = image->bc;
        image->rc = image->bc;
        return *image;
    }
}

Image greenonly(const string& file, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        iFile.bc = iFile.gc;
        iFile.rc = iFile.gc;
        return iFile;
    }
    else {
        image->bc = image->gc;
        image->rc = image->gc;
        return *image;
    }

}

Image redonly(const string& file, Image* image) {
    if (image == nullptr) {
        Image iFile(file);
        iFile.bc = iFile.rc;
        iFile.gc = iFile.rc;
        return iFile;
    }
    else {
        image->bc = image->rc;
        image->gc = image->rc;
        return *image;
    }

}

Image combine(const string& bFile, const string& gFile, const string& rFile, Image* image) {
    if (image == nullptr) {
        Image blue(bFile);
        Image green(gFile);
        Image red(rFile);
        blue.gc = green.gc;
        blue.rc = red.rc;
        return blue;
    }
    else {
        Image blue(bFile);
        Image green(gFile);
        image->bc = blue.bc;
        image->gc = green.gc;
        return *image;
    }
}

Image flip(const string& file, Image* imag) {
    if (imag == nullptr) {
        Image image(file);
        vector<unsigned char> blueRev;
        vector<unsigned char> greenRev;
        vector<unsigned char> redRev;
        int len = (int)image.bc.size() - 1;
        for (int i = len; i >= 0; i--) {
            blueRev.push_back(image.bc.at(i));
            greenRev.push_back(image.gc.at(i));
            redRev.push_back(image.rc.at(i));
        }
        image.bc = blueRev;
        image.gc = greenRev;
        image.rc = redRev;
        return image;
    }
    else {
        vector<unsigned char> blueRev;
        vector<unsigned char> greenRev;
        vector<unsigned char> redRev;
        int len = (int)imag->bc.size() - 1;
        for (int i = len; i >= 0; i--) {
            blueRev.push_back(imag->bc.at(i));
            greenRev.push_back(imag->gc.at(i));
            redRev.push_back(imag->rc.at(i));
        }
        imag->bc = blueRev;
        imag->gc = greenRev;
        imag->rc = redRev;
        return *imag;
    }
}
