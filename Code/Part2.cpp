#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include<cmath>


using namespace cv;
using namespace std;

struct point {
    int x;
    int y;
};

class node {
public:
    int data;
    node* next;
    node* downPtr;

    point topLeft;
    point bottomRight;

    bool isGrey;

    node* NW;
    node* NE;
    node* SW;
    node* SE;

    int id;

    node(int val = -333) {
        data = val;
        next = NULL;
        downPtr = NULL;

        isGrey = true;

        NW = NULL;
        NE = NULL;
        SW = NULL;
        SE = NULL;

        id = 0;
    }
};

class QuadTree {
public:
    node* root;
    QuadTree() { root = NULL; }
};

//int k = 0;

void readingFiles(node* &root, int fileID) {
    string folderName = "T3";
    string fileName = folderName + "/" + to_string(fileID) + ".txt";

    if (fileID == 1)
        fileName = folderName + "/root.txt";

    fstream myFile;
    myFile.open(fileName, ios::in);
    if (myFile.is_open()) {
        node* newNode = new node;
        string line;
        getline(myFile, line);              // reading the current index/ID of the Node
        newNode->id = fileID;
        //k++;
        getline(myFile, line);              // reading the color of the Node
        if (line[line.length() - 2] == '1')
            newNode->data = -1;
        if (line[line.length() - 2] == '5')
            newNode->data = 255;
        if (line[line.length() - 2] == '0')
            newNode->data = 0;

        getline(myFile, line);
        getline(myFile, line);              // reading the topLeft coordinates of the Image
        for (int i = 0; i < line.length(); i++) {
            if (line[i - 1] == ' ' && line[i - 2] == ':') {
                newNode->topLeft.x = line[i] - 48;
                i++;
                while (line[i] != ',') {
                    newNode->topLeft.x *= 10;
                    newNode->topLeft.x += line[i] - 48;
                    i++;

                }
            }
            if (line[i - 1] == ' ' && line[i - 2] == ',') {
                newNode->topLeft.y = line[i] - 48;
                i++;
                while (line[i] != '\0') {
                    newNode->topLeft.y *= 10;
                    newNode->topLeft.y += line[i] - 48;
                    i++;

                }
            }
        }

        getline(myFile, line);              // reading the bottomRight coordinates of the Image
        for (int i = 9; i < line.length(); i++) {
            if (line[i - 1] == ' ' && line[i - 2] == ':') {
                newNode->bottomRight.x = line[i] - 48;
                i++;
                while (line[i] != ',') {
                    newNode->bottomRight.x *= 10;
                    newNode->bottomRight.x += line[i] - 48;
                    i++;

                }
            }
            if (line[i - 1] == ' ' && line[i - 2] == ',') {
                newNode->bottomRight.y = line[i] - 48;
                i++;
                while (line[i] != '\0') {
                    newNode->bottomRight.y *= 10;
                    newNode->bottomRight.y += line[i] - 48;
                    i++;

                }
            }
        }
        root = newNode;
    }
    else return;

    //cout << root->data << " ";


    readingFiles(root->NW, (fileID * 4));
    readingFiles(root->NE, (fileID * 4) + 1);
    readingFiles(root->SW, (fileID * 4) + 2);
    readingFiles(root->SE, (fileID * 4) + 3);

}

void decodedImageFormation(node* root, int ** &arr, int rows, int cols) {
    if (root == NULL)
        return;

    if (root->NW == NULL && root->NE == NULL && root->SW == NULL && root->SE == NULL) {

        if (root->topLeft.x == root->bottomRight.x) {
            for (int j = root->topLeft.y; j <= root->bottomRight.y; j++)
                arr[root->topLeft.x][j] = root->data;
        }
        else if (root->topLeft.y == root->bottomRight.y) {
            for (int i = root->topLeft.x; i <= root->bottomRight.x; i++)
                arr[i][root->topLeft.y] = root->data;
        }
        else {
            for(int i = root->topLeft.x; i < root->bottomRight.x; i++){
                for (int j = root->topLeft.y; j < root->bottomRight.y; j++) {
                    arr[i][j] = root->data;
                }
            }
        }

    }




    decodedImageFormation(root->NW, arr, rows, cols);
    decodedImageFormation(root->NE, arr, rows, cols);
    decodedImageFormation(root->SW, arr, rows, cols);
    decodedImageFormation(root->SE, arr, rows, cols);
}

void main() {

    QuadTree quad;
    readingFiles(quad.root, 1);

    int rows = quad.root->bottomRight.x;
    int cols = quad.root->bottomRight.y;

    int** decodedArray = new int* [rows];
    for (int i = 0; i < rows; i++)
        *(decodedArray + i) = new int[cols];

    decodedImageFormation(quad.root, decodedArray, rows, cols);

    //for (int i = 0; i < rows; i++) {
    //    for (int j = 0; j < cols; j++) {
    //        cout << decodedArray[i][j] << " ";
    //    }
    //    cout << endl << endl;
    //}


    Mat img(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (decodedArray[i][j] == 0) {
                img.at<Vec3b>(i, j)[0] = 0;
                img.at<Vec3b>(i, j)[1] = 0;
                img.at<Vec3b>(i, j)[2] = 0;
            }
            else if (decodedArray[i][j] == 255) {
                img.at<Vec3b>(i, j)[0] = 255;
                img.at<Vec3b>(i, j)[1] = 255;
                img.at<Vec3b>(i, j)[2] = 255;
            }
        }
    }

    imshow("image", img);
    imwrite("decoded-T3.bmp", img);
    waitKey(0);

    

}