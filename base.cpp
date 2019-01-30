#include "tgaimage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
#include <cstdlib>
#include <math.h>
#include <limits>
#include <algorithm>



using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0 , 0, 255, 255);
vector<vector<int>> points;
vector<vector<float>> texPoints;
vector<vector<vector<int>>> triangles;
vector<vector<vector<float>>> texTriangles;
vector<vector<string>> vlignes;
vector<int> light {0,0,1};
//ee

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

int getElemTga(string name){
    ifstream fichier(name.c_str(), ios::in);
    if(fichier)
    {
        points.clear();
        triangles.clear();
        string ligne;
        while(getline(fichier, ligne))
        {
            stringstream ligneStream(ligne);
            istream_iterator<string> begin(ligneStream);
            istream_iterator<string> end;
            vector<string> vstrings(begin, end);

            //Points
            if (vstrings.size() > 0 && vstrings[0] == "v"){
                float x =  strtof((vstrings[1]).c_str(),0);
                float y =  strtof((vstrings[2]).c_str(),0);
                float z =  strtof((vstrings[3]).c_str(),0);
                vector<int> point{(int)((1+x)*400),(int)((1+y)*400),(int)((1+z)*400)};
                points.push_back(point);
            }

            //Texture Points
            if (vstrings.size() > 0 && vstrings[0] == "vt"){
                float x =  strtof((vstrings[1]).c_str(),0);
                float y =  strtof((vstrings[2]).c_str(),0);
                float z =  strtof((vstrings[3]).c_str(),0);
                vector<float> texPoint{((x)),((y)),((z))};
                texPoints.push_back(texPoint);
            }

            if(vstrings.size() > 0 && vstrings[0] == "f"){
                //Triangles
                vector<vector<int>> triangle;
                for (int i = 1; i < 4; i++){
                    triangle.push_back(points[-1+stoi(vstrings[i].substr(0, vstrings[i].find("/")))]);
                    vstrings[i] = vstrings[1].substr(vstrings[1].find("/")+1, vstrings[1].size()-vstrings[1].find("/")+1);
                }
                triangles.push_back(triangle);

                //Texture Triangle
                vector<vector<float>> texTriangle;
                for (int i = 1; i < 4; i++){
                    texTriangle.push_back(texPoints[-1+stoi(vstrings[i].substr(0, vstrings[i].find("/")))]);
                    vstrings[i] = vstrings[1].substr(vstrings[1].find("/")+1, vstrings[1].size()-vstrings[1].find("/")+1);
                }
                texTriangles.push_back(texTriangle);
                for (int i = 0; i < 3; i++){
                    for (int j = 0; j < 3; j++){
                        cout << texTriangle[i][j] << " ";
                    }
                    cout << "  :  ";
                }
                cout << endl ;
            }
        }
    }else{  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;
        return 0;
    }
    return 1;
}


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
        //cout << "drawLine : " << x << "   " << x0 << "   " << x1 << endl;
        //cout << "drawLine : "<< "compteur : " << compt << endl;
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
        std::swap(p1,p2);
    }
    if(p1[1]>p3[1]){
        std::swap(p1,p3);
    }
    if(p2[1]>p3[1]){
        std::swap(p2,p3);
    }
    float t,t2;
    int x, x2;
    for (int y = p1[1]; y <= p3[1]; y++) {
        //cout << "drawTriangle: "<< "compteur : " << compt << endl;

        t = (y - p1[1])/(float)(p3[1] - p1[1]);
        if (isnan(t)){
                t = 0;
        }
        x = p1[0]*(1.-t) + p3[0]*t;
        //image.set(x, y, color);
        if(y<=p2[1]){
            t2 = (y - p1[1])/(float)(p2[1] - p1[1]);
            if (isnan(t2)){
                t2 = 0;
            }
            x2 = p1[0]*(1.-t2) + p2[0]*t2;
            //cout << t2 << "   " << x2 << "   " << endl;
        }else{
            t2 = (y - p2[1])/(float)(p3[1] - p2[1]);
            if (isnan(t2)){
                t2 = 0;
            }
            x2 = p2[0]*(1.-t2) + p3[0]*t2;
        }
        //cout << y << "   " << x << "   " << x2 << endl;
        //TGAColor rand = TGAColor(rand1, rand2, rand3, 255);
        /*float test = 255*(((float)(p3[1])-(float)(y))/((float)(p3[1])-(float)(p1[1])));
        TGAColor rand = TGAColor(test, 0, 0, 255);*/
        drawLine(x, y, x2, y, image, color);
        //image.set(x2, y, color);
    }
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


void drawTriangleTga(string name, TGAImage &image){
    if(getElemTga(name)){
        for (vector<vector<int>> triangle : triangles){
            int rand1 = rand()%255;
            int rand2 = rand()%255;
            int rand3 = rand()%255;
            TGAColor rand = TGAColor(rand1, rand2, rand3, 255);
            drawTriangle(triangle[0], triangle[1], triangle[2], image, rand);
        }
    }
}



vector<float> barycentric(vector<float> p1, vector<float> p2, vector<float> p3, vector<float> P) {
    float s[2][3];
    for (int i=2; i--; ) {
        s[i][0] = p3[i]-p1[i];
        s[i][1] = p2[i]-p1[i];
        s[i][2] = p1[i]-P[i];
    }
    vector<float> u = {s[0][1]*s[1][2] - s[0][2]*s[1][1], s[0][2]*s[1][0] - s[0][0]*s[1][2], s[0][0]*s[1][1] - s[0][1]*s[1][0]};

    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return vector<float> {1.f-(u[0]+u[1])/u[2], u[1]/u[2], u[0]/u[2]};
    return vector<float>{-1,1,1}; // in this case generate negative coordinates, it will be thrown away by the rasterizator
}


void drawTriangleBuffer(vector<float> p1, vector<float> p2, vector<float> p3, float *zbuffer, TGAImage &image, TGAColor color){
    vector<float> bboxmin{ std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
    vector<float> bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    vector<float> clamp{800-1, 800-1};

    for (int j=0; j<2; j++) {
        bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], p1[j]));
        bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], p1[j]));
    }
    for (int j=0; j<2; j++) {
        bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], p2[j]));
        bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], p2[j]));
    }
    for (int j=0; j<2; j++) {
        bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], p3[j]));
        bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], p3[j]));
    }

    for (float x=bboxmin[0]; x<=bboxmax[0]; x++) {
        for (float y=bboxmin[1]; y<=bboxmax[1]; y++) {
            vector<float> bc_screen  = barycentric(p1, p2, p3, vector<float> {x, y});
            if (bc_screen[0]<0 || bc_screen[1]<0 || bc_screen[2]<0) continue;
            float z = p1[2]*bc_screen[0] + p2[2]*bc_screen[1] + p3[2]*bc_screen[2];
            if (zbuffer[int(x+y*800)] < z) {
                zbuffer[int(x+y*800)] = z;
                image.set(x, y, color);
            }
        }
    }
}


void backFaceCulling(string name, TGAImage &image){
    if(getElemTga(name)){

        float div = 400;
        float moins = 1;
        float *zbuffer = new float[800*800];
        for (vector<vector<int>> triangle : triangles){

            //Recuperation des points dans leurs coordonnees dorigine
            vector<float> point1 {(float)(triangle[0][0])/div-moins, (float)(triangle[0][1])/div-moins, (float)(triangle[0][2])/div-moins};
            vector<float> point2 {(float)(triangle[1][0])/div-moins, (float)(triangle[1][1])/div-moins, (float)(triangle[1][2])/div-moins};
            vector<float> point3 {(float)(triangle[2][0])/div-moins, (float)(triangle[2][1])/div-moins, (float)(triangle[2][2])/div-moins};
            //for(vector<int> point : triangle){
              //  cout << point[0] << "     " << point[1] << "     " << point[2] << endl;
            //}


            vector<float> U {point2[0]-point1[0], point2[1]-point1[1], point2[2]-point1[2]};
            vector<float> V {point3[0]-point1[0], point3[1]-point1[1], point3[2]-point1[2]};

            float normX = U[1]*V[2] - U[2]*V[1];
            float normY = U[2]*V[0] - U[0]*V[2];
            float normZ = U[0]*V[1] - U[1]*V[0];


            vector<float> norm2 {normX, normY, normZ};

            float normale = sqrt(norm2[0]*norm2[0]+norm2[1]*norm2[1]+norm2[2]*norm2[2]);
            vector<float> norm {normX*(1/normale), normY*(1/normale), normZ*(1/normale)};

            //cout << "normale : " << normale << endl;

            //Calcul de lintensite lumineuse
            float intensity = (light[0]*norm[0] + light[1]*norm[1] + light[2]*norm[2])/(light[0]+light[1]+light[2]);
            //cout << "norm : " << norm[0] << "     " << norm[1] << "     " << norm[2] << endl;


            if (intensity > 0) {
                float intensityColor = (intensity*255);
                TGAColor colorTest = TGAColor(intensityColor,intensityColor,intensityColor,255);
                //drawTriangle(triangle[0], triangle[1], triangle[2], image, colorTest);
                vector<float> pt1 {(float)(triangle[0][0]), (float)(triangle[0][1]), (float)(triangle[0][2])};
                vector<float> pt2 {(float)(triangle[1][0]), (float)(triangle[1][1]), (float)(triangle[1][2])};
                vector<float> pt3 {(float)(triangle[2][0]), (float)(triangle[2][1]), (float)(triangle[2][2])};
                drawTriangleBuffer(pt1, pt2, pt3, zbuffer, image, colorTest);
                //drawTriangleBuffer(point1, point2, point3, *zbuffer, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
            }
        }
    }
}


void testInterpolation(string name, TGAImage &image){
    /* TODO
    cube de couleur
    test apllication sur un simple triangle*/
}




int main(int argc, char** argv) {

    /*TGAImage image(100, 100, TGAImage::RGB);
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
    image5.flip_vertically();
    image5.write_tga_file("output5.tga");

    TGAImage image6(800,800, TGAImage::RGB);
    backFaceCulling("african_head.obj", image6);
    image6.flip_vertically();
    image6.write_tga_file("output6.tga");*/

    /*TGAImage image7(800,800, TGAImage::RGB);
    backFaceCulling("african_head.obj", image7);
    image7.flip_vertically();
    image7.write_tga_file("output7.tga");*/

    TGAImage image8(800,800, TGAImage::RGB);
    //drawTriangleTga("african_head.obj", image8);
    drawTriangle(vector<int>{50,50},vector<int>{400,550},vector<int>{750,650},image8, red);
    image8.flip_vertically();
    image8.write_tga_file("output8.tga");

    return 0;
}
