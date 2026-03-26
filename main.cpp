// ─────────────────────────────────────────────────────────────────────────────
//  Generates an ASCII art representation of an image
//
//  Dependencies:
//    stb_image.h  –  https://github.com/nothings/stb
//
//  Usage:
//    Ascii.exe image.png [options]
//
//  Options (optional, any order):
//    width   <int>     Art width in characters  (default: 100)
//    --save            Save output to a .txt file 
// ─────────────────────────────────────────────────────────────────────────────

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string asciiChars = " .:-=+*#%@";

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Use: ascii.exe <image> [width] [--save]\n";
        return 1;
    }

    string filename = argv[1];

    int newWidth = 100;
    bool saveToFile = false;

    for (int i = 2; i < argc; i++) {
        string arg = argv[i];

        if (arg == "--save") {
            saveToFile = true;
        }
        else {
            try {
                newWidth = stoi(arg);
            }
            catch (...) {
                cout << "invalid width\n";
                return 1;
            }
        }
    }

    int width, height, channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (!img) {
        cout << "Error loading image\n";
        return 1;
    }

    float aspectRatio = 0.5;
    int newHeight = (height * newWidth / width) * aspectRatio;

    int xScale = width / newWidth;
    int yScale = height / newHeight;

    if (xScale < 1) xScale = 1;
    if (yScale < 1) yScale = 1;

    ofstream outFile;
    if (saveToFile) {
        string outputName = filename;

        size_t dotPos = outputName.find_last_of('.');
        if (dotPos != string::npos) {
            outputName = outputName.substr(0, dotPos);
        }

        outputName += ".txt";

        outFile.open(outputName);
        if (!outFile) {
            cout << "Error loadign file\n";
            return 1;
        }
    }

    for (int y = 0; y < height; y += yScale) {
        for (int x = 0; x < width; x += xScale) {

            int idx = (y * width + x) * channels;

            int r = img[idx];
            int g = img[idx + 1];
            int b = img[idx + 2];

            int gray = 0.299 * r + 0.587 * g + 0.114 * b;

            gray = (gray - 128) * 1.5 + 128;
            if (gray < 0) gray = 0;
            if (gray > 255) gray = 255;

            int index = ((255 - gray) * (asciiChars.size() - 1)) / 255;

            char c = asciiChars[index];

            cout << c;

            if (saveToFile) {
                outFile << c;
            }
        }

        cout << endl;
        if (saveToFile) outFile << endl;
    }

    if (saveToFile) {
        outFile.close();
        cout << "\nFile Saved\n";
    }

    stbi_image_free(img);
    return 0;
}