#include "tgaimage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>



using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0 , 0, 255, 255);
vector<vector<int>> points;
vector<vector<vector<int>>> triangles;
vector<vector<string>> vlignes;

/*void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool swaped = false;
    if (abs(x0 - x1) < abs(y0 - y1)) {
        swap(x0, y0);
        swap(x1, y1);
        swaped = true;
    }
    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }
    for (int x = x0; x <= x1; x++) {
        float t = (x - x0)/(float)(x1 - x0);
        int y = y0*(1.-t) + y1*t;
        if (swaped) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}*/

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

void drawTriangle(vector<int> p1, vector<int> p2, vector<int> p3, TGAImage &image, TGAColor color){
    /*drawLine(p1[0], p1[1], p2[0], p2[1], image, white);
    drawLine(p2[0], p2[1], p3[0], p3[1], image, white);
    drawLine(p1[0], p1[1], p3[0], p3[1], image, white);
    */if(p1[1]>p2[1]){
        swap(p1,p2);
    }
    if(p1[1]>p3[1]){
        swap(p1,p3);
    }
    if(p2[1]>p3[1]){
        swap(p2,p3);
    }
    float t,t2;
    int x, x2;
    for (int y = p1[1]; y <= p3[1]; y++) {
        t = (y - p1[1])/(float)(p3[1] - p1[1]);
        x = p1[0]*(1.-t) + p3[0]*t;
        //image.set(x, y, color);
        if(y<=p2[1]){
            t2 = (y - p1[1])/(float)(p2[1] - p1[1]);
            x2 = p1[0]*(1.-t2) + p2[0]*t2;
        }else{
            t2 = (y - p2[1])/(float)(p3[1] - p2[1]);
            x2 = p2[0]*(1.-t2) + p3[0]*t2;
        }
        drawLine(x, y, x2, y, image, color);
        //image.set(x2, y, color);
    }
}

int getElemTga(string name){
    ifstream fichier(name.c_str(), ios::in);  // on ouvre le fichier en lecture
    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            stringstream ligneStream(ligne);
            istream_iterator<string> begin(ligneStream);
            istream_iterator<string> end;
            vector<string> vstrings(begin, end);
            if (vstrings.size() > 0 && vstrings[0] == "v"){
                float x =  strtof((vstrings[1]).c_str(),0);
                float y =  strtof((vstrings[2]).c_str(),0);
                vector<int> point{(int)((1+x)*400),(int)((1+y)*400)};
                points.push_back(point);
            }
            if(vstrings.size() > 0 && vstrings[0] == "f"){
                vector<vector<int>> triangle;
                triangle.push_back(points[-1+stoi(vstrings[1].substr(0, vstrings[1].find("/")))]);
                triangle.push_back(points[-1+stoi(vstrings[2].substr(0, vstrings[2].find("/")))]);
                triangle.push_back(points[-1+stoi(vstrings[3].substr(0, vstrings[3].find("/")))]);
                triangles.push_back(triangle);
            }
        }
    }else{  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;
        return 0;
    }
    return 1;
}



void drawPointTga(string name, TGAImage &image){
    if(getElemTga(name)){
        for (vector<int> point : points){
            image.set(point[0], point[1], white);
        }
    }
}


void drawLineTga(string name, TGAImage &image){
    if(getElemTga(name)){
        for (vector<vector<int>> triangle : triangles){
            drawLine(triangle[0][0], triangle[0][1], triangle[1][0], triangle[1][1], image, white);
            drawLine(triangle[1][0], triangle[1][1], triangle[2][0], triangle[2][1], image, white);
            drawLine(triangle[0][0], triangle[0][1], triangle[2][0], triangle[2][1], image, white);
        }
    }
}


int drawTriangleTga(string name, TGAImage &image){
    ifstream fichier(name.c_str(), ios::in);  // on ouvre le fichier en lecture
    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            stringstream ligneStream(ligne);
            istream_iterator<string> begin(ligneStream);
            istream_iterator<string> end;
            vector<string> vstrings(begin, end);
            vlignes.push_back(vstrings);
            //copy(vstrings.begin(), vstrings.end(), ostream_iterator<string>(cout, "\n"));
            if (vstrings.size() > 0 && vstrings[0] == "f"){

                /*vector<string> point1 = vlignes[-1+stoi(vstrings[1].substr(0, vstrings[1].find("/")))];
                vector<string> point2 = vlignes[-1+stoi(vstrings[2].substr(0, vstrings[2].find("/")))];
                vector<string> point3 = vlignes[-1+stoi(vstrings[3].substr(0, vstrings[3].find("/")))];
                vector<int> p1{(1.+(float)strtof((point1[1]).c_str(),0))*400, (1.+(float)strtof((point1[2]).c_str(),0))*400};
                vector<int> p2{(1.+(float)strtof((point2[1]).c_str(),0))*400, (1.+(float)strtof((point2[2]).c_str(),0))*400};
                vector<int> p3{(1.+(float)strtof((point3[1]).c_str(),0))*400, (1.+(float)strtof((point3[2]).c_str(),0))*400};

                drawTriangle(p1, p2, p3, image, red);*/
                //cout << ligne << endl;  // on l'affiche
            }
        }

        //cout << ligne << "\n" << token << endl;  // on l'affiche

        fichier.close();  // on ferme le fichier
    }
    else  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;

    return 0;
}



int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);
    image.set(52, 41, red);

    drawLine(10, 20, 80, 80, image, white);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");


    TGAImage image2(800,800, TGAImage::RGB);
    drawPointTga("african_head.obj" ,image2);
    image2.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image2.write_tga_file("output2.tga");


    TGAImage image3(800,800, TGAImage::RGB);
    drawLineTga("african_head.obj" ,image3);
    image3.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image3.write_tga_file("output3.tga");


    TGAImage image4(800,800, TGAImage::RGB);
    drawTriangle(vector<int>{50,50},vector<int>{400,550},vector<int>{750,650},image4, red);
    drawTriangle(vector<int>{50,50},vector<int>{50,200},vector<int>{200,200},image4, green);
    drawTriangle(vector<int>{125,600},vector<int>{150,750},vector<int>{50,800},image4, blue);
    image4.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image4.write_tga_file("outputTriangle.tga");

    TGAImage image5(800,800, TGAImage::RGB);
    drawTriangleTga("african_head.obj" ,image5);
    image5.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image5.write_tga_file("output5.tga");

    return 0;
}
