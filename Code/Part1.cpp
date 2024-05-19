#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include<cmath>


using namespace cv;
using namespace std;

struct point {      // A struct to store the starting and ending Coordinates of the Image
    int x;
    int y;
};

class node {
public:
    int data;               // Data to store Color (-1 = Grey), (0 = Black), (255 = White)
    node* next;
    node* downPtr;

    point topLeft;          // struct objects to store Image coordinates
    point bottomRight;

    bool isGrey;        // bool to check if both color Exists or Not (Black & White)

    node* NW;           
    node* NE;
    node* SW;
    node* SE;

    int id;         // Node Id to find and for the Naming of The Node txt Files

    node(int val = -333) {      // Constructor to Initializes the data members os the class
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

class List {
public:
    node* head;

    List() { head = NULL; }

    void insert(int val) {          // Function to insert a Node in the List
        node* newNode = new node(val);
        if (head == NULL) {
            head = newNode;
        }
        else {
            node* temp = head;
            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newNode;
        }
    }

    void display() {                // Function of Displaying the whole Linked LIst
        node* temp = head;
        while (temp != NULL) {
            node* p = temp;
            while (p != NULL) {
                cout << p->data;
                if (p->next != NULL)
                    cout << " > ";
                p = p->next;
            }
            cout << endl;

            temp = temp->downPtr;
        }
    }
};

List LL;
void convertTo2dLL(int** array, int row, int col) {         // Creating a 2dLL

    LL.insert(1);
    node* tempHead = LL.head;

    for (int i = 0; i < row; i++) {

        for (int j = 0; j < col; j++) {         // For White
            int index = j;
            if (array[i][j] == 255) {
                LL.insert(j + 1);

                while (array[i][index] == 255) {
                    //if (index < row - 1)
                    index++;
                    if (index == col)
                        break;
                }

                LL.insert(index);
            }
            j = index;
        }

        LL.insert(-2);

        for (int j = 0; j < col; j++) {         // For Black
            int index = j;
            if (array[i][j] == 0) {
                LL.insert(j + 1);

                while (array[i][index] == 0) {
                    // if (index < row - 1)
                    index++;
                    if (index == col)
                        break;
                }

                LL.insert(index);
            }
            j = index;
        }

        LL.insert(-1);

        if (i < row - 1) {
            node* newNode = new node(i + 2);
            LL.head->downPtr = newNode;
            LL.head = LL.head->downPtr;
        }
    }

    LL.head = tempHead;
}

class QuadTree {
public:
    node* root;
    QuadTree() { root = NULL; }
};

void insertQuad(node*& root, node* newNode) {       // Inserting a Node in the Quad-Tree
    if (root == NULL)
        root = newNode;
}


void image_Tree(node*& root, point& topLeft, point& bottomRight, int **array) {     // Function to create a Quad-Tree from the 2dLL/2D Array
    if (root == NULL)
        return;

    if (((bottomRight.x - topLeft.x) <= 1) && ((bottomRight.y - topLeft.y) <= 1)) {     // Checking if the portion of the image is divisible or Not
        root->data = array[topLeft.x][topLeft.y];
        return;
    }

    bool black = false, white = false;

    //node* temp = LL.head;
    //while (temp->data <= bottomRight.x) {
    //    if (temp == LL.head) {
    //        for (int i = 1; i < topLeft.x; i++)
    //            temp = temp->downPtr;
    //    }
    //    node* ptr = temp->next;
    //    while (ptr->data != -2) {
    //        if (ptr->data > topLeft.y && ptr->data <= bottomRight.y)
    //            white = true;
    //        ptr = ptr->next;
    //    }
    //    while (ptr->data != -1) {
    //        if (ptr->data > topLeft.y && ptr->data <= bottomRight.y)
    //            black = true;
    //        ptr = ptr->next;
    //    }
    //    if (black && white)
    //        break;
    //    temp = temp->downPtr;
    //    if (temp == NULL)
    //        break;
    //}

    for (int i = topLeft.x; i < bottomRight.x; i++) {               // Checking if the image portion have both colors or Not (Black & White)
        for (int j = topLeft.y; j < bottomRight.y; j++) {
            if (array[i][j] == 0)
                black = true;
            if (array[i][j] == 255)                 
                white = true;
            if (black && white)
                break;
        }
        if (black && white)
            break;
    }

    if (black && white) {           // making Bool Grey = true if both Color Exists
        root->isGrey = true;
        root->data = -1;
    }
    else root->isGrey = false;

    if (black && (!white))          // Initializing data = 0 if Only black Color
        root->data = 0;
    else if (white && (!black))          // Initializing data = 255 if Only White Color
        root->data = 255;

    node* newNode_NW = new node;
    node* newNode_NE = new node;
    node* newNode_SW = new node;
    node* newNode_SE = new node;

    if (root->isGrey) {         // If image portion is Grey than Further Dividing it into 4 parts.

        int childRow = (topLeft.x + bottomRight.x) / 2;
        int childCol = (topLeft.y + bottomRight.y) / 2;


        //------------------------------- NW

        newNode_NW->topLeft.x = topLeft.x;
        newNode_NW->topLeft.y = topLeft.y;
        newNode_NW->bottomRight.x = childRow;
        newNode_NW->bottomRight.y = childCol;

        newNode_NW->id = root->id * 4;

        insertQuad(root->NW, newNode_NW);


        //------------------------------- NE

        newNode_NE->topLeft.x = topLeft.x;
        newNode_NE->topLeft.y = childCol;
        newNode_NE->bottomRight.x = childRow;
        newNode_NE->bottomRight.y = bottomRight.y;

        newNode_NE->id = (root->id * 4) + 1;

        insertQuad(root->NE, newNode_NE);

        //------------------------------- SW

        newNode_SW->topLeft.x = childRow;
        newNode_SW->topLeft.y = topLeft.y;
        newNode_SW->bottomRight.x = bottomRight.x;
        newNode_SW->bottomRight.y = childCol;

        newNode_SW->id = (root->id * 4) + 2;

        insertQuad(root->SW, newNode_SW);

        //------------------------------- SE

        newNode_SE->topLeft.x = childRow;
        newNode_SE->topLeft.y = childCol;
        newNode_SE->bottomRight.x = bottomRight.x;
        newNode_SE->bottomRight.y = bottomRight.y;

        newNode_SE->id = (root->id * 4) + 3;

        insertQuad(root->SE, newNode_SE);
    } 

    image_Tree(root->NW, newNode_NW->topLeft, newNode_NW->bottomRight, array);          // Recursive Traversing the Tree and Inserting Childs if Grey
    image_Tree(root->NE, newNode_NE->topLeft, newNode_NE->bottomRight, array);          // Recursive Traversing the Tree and Inserting Childs if Grey
    image_Tree(root->SW, newNode_SW->topLeft, newNode_SW->bottomRight, array);          // Recursive Traversing the Tree and Inserting Childs if Grey
    image_Tree(root->SE, newNode_SE->topLeft, newNode_SE->bottomRight, array);          // Recursive Traversing the Tree and Inserting Childs if Grey

}

void  constructFiles(node* root) {
    if (root == NULL)
        return;
    string folderName = "T1";
    string fileName = folderName + "/" + to_string(root->id) + ".txt";

    if (root->id == 1)
        fileName = folderName + "/" + "root.txt";

    ofstream outFile(fileName);             

    outFile << "\t\tCurrent: ";         // Writing the ID of Node in the txt File
    if (root->id == 1)
        outFile << "ROOT" << endl;
    else
        outFile << root->id << endl;

    outFile << "Color: ";                    // Writing the color of Node in the txt File
    if (root->data == -1)
        outFile << "Grey (" << root->data << ")" << endl;
    else if (root->data == 0)
        outFile << "Black (" << root->data << ")" << endl;
    else if (root->data == 255)
        outFile << "White (" << root->data << ")" << endl;

    // Writing the position/Coordinates of Node in the txt File
    outFile << "Position: \n\tTop Left: " << root->topLeft.x << ", " << root->topLeft.y << "\n\tBottom Right: " << root->bottomRight.x << ", " << root->bottomRight.y << endl;
    
    if (root->id == 1)         // Writing the Parent ID of Node in the txt File
        outFile << "Parent: No Parent, ROOT" << endl;
    else
        outFile << "Parent: " << root->id / 4 << endl;
        

    // Writing the Child Id's of Nodes in the txt File
    if (root->NW != NULL && root->NE != NULL && root->SW != NULL && root->SE != NULL)
        outFile << "Children: \n\t" << root->NW->id << ", " << root->NE->id << ", " << root->SW->id << ", " << root->SE->id << endl;
    if (root->NW == NULL && root->NE == NULL && root->SW == NULL && root->SE == NULL)
        outFile << "Children: \n\tNo Child" << endl;

    constructFiles(root->NW);
    constructFiles(root->NE);
    constructFiles(root->SW);
    constructFiles(root->SE);

}

void main() {
    Mat image = imread("t1.bmp", IMREAD_GRAYSCALE);

    if (image.empty()) {
        cout << "\n\nCould not open or find the image" << endl;
        return;
    }

    int rows = image.rows;
    int cols = image.cols;

    cout << "Rows: " << rows << "\nCols: " << cols << endl;

    int** imgArray = new int* [rows];           //Creating 2D Array
    for (int i = 0; i < cols; i++)
        *(imgArray + i) = new int[rows];

    for (int i = 0; i < rows; i++) {          // copy image data to 2D array
        for (int j = 0; j < cols; j++) {
            *(*(imgArray + i) + j) = (int)image.at<uchar>(i, j);
        }
    }

    convertTo2dLL(imgArray, rows, cols);
    //LL.display();



    QuadTree quadTree;             // Creating Quad Tree Object

    node* newNode = new node;

    newNode->data = -1;
                                        // Inserting the Whole Image as Root in the Quad-tree
    newNode->topLeft.x = 0;
    newNode->topLeft.y = 0;
    newNode->bottomRight.x = rows;
    newNode->bottomRight.y = cols;

    newNode->isGrey = true;

    newNode->id = 1;

    insertQuad(quadTree.root, newNode);
    node* tempRoot = quadTree.root;

    image_Tree(quadTree.root, newNode->topLeft, newNode->bottomRight, imgArray);            // Fuction to create a tree
    quadTree.root = tempRoot;

    //constructFiles(quadTree.root);      // function of writing each node of Tree in txt File..


    /*namedWindow("Image", WINDOW_NORMAL);
    imshow("Image", image);
    waitKey(0);*/


}

