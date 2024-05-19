#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include<cmath>


using namespace cv;
using namespace std;



int main() {
    string originalImage = "t3.bmp";            // Name of Original Image File
    string decodedImage = "decoded-T3.bmp";             // Decoded Image File Name

    Mat image1 = imread(originalImage, IMREAD_GRAYSCALE);           // Reading Image Using OpenCV

    if (image1.empty())
        cout << "Could not open or find the image" << endl;         // Checking if image Exists or Not

    int rows1 = image1.rows;
    int cols1 = image1.cols;

    //cout << "Rows: " << rows1 << "\nCols: " << cols1 << endl;

    int** originalArray = new int* [rows1];
    for (int i = 0; i < cols1; i++)                     // Creatig 2D Array to store Image Pixels
        *(originalArray + i) = new int[rows1];

    for (int i = 0; i < rows1; i++) {          // copy image data to 2D array
        for (int j = 0; j < cols1; j++) {
            *(*(originalArray + i) + j) = (int)image1.at<uchar>(i, j);
        }
    }

    Mat image2 = imread(decodedImage, IMREAD_GRAYSCALE);           // Reading Image Using OpenCV

    if (image2.empty())
        cout << "Could not open or find the image" << endl;         // Checking if image Exists or Not

    int rows2 = image1.rows;
    int cols2 = image1.cols;

    //cout << "Rows: " << rows2 << "\nCols: " << cols2 << endl;

    int** decodedArray = new int* [rows2];
    for (int i = 0; i < cols2; i++)                     // Creatig 2D Array to store Image Pixels
        *(decodedArray + i) = new int[rows2];

    for (int i = 0; i < rows2; i++) {          // copy image data to 2D array
        for (int j = 0; j < cols2; j++) {
            *(*(decodedArray + i) + j) = (int)image2.at<uchar>(i, j);
        }
    }



    float X = 0;

    for (int i = 0; i < rows1; i++) {               // Checking the Accuracy between the Original and the Decoded Image
        for (int j = 0; j < cols1; j++) {
            X += (originalArray[i][j] - decodedArray[i][j]) * (originalArray[i][j] - decodedArray[i][j]);       // Appling the Formula
        }
    }

    X /= rows1 * cols1;
    int Z = 255;

    float Y = 100 * (1 - (X / (Z * Z)));        // Appling Forumla to check Percent of Accuracy

    cout << "\n\n\t\tAccuraccy (Y) = " << Y << " %" << endl;        // Displaying the Image Accuracy in Percent


   /* namedWindow("Image", WINDOW_NORMAL);
    imshow("Image", image);
    waitKey(0);*/


}


