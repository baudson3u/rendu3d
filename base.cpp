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
vector<vector<string>> vlignes;

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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
}











/*void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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
}*/

int drawPointTga(string name, TGAImage &image){
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
            //copy(vstrings.begin(), vstrings.end(), ostream_iterator<string>(cout, "\n"));
            if (vstrings.size() > 0 && vstrings[0] == "v"){

                float x =  strtof((vstrings[1]).c_str(),0);
                float y =  strtof((vstrings[2]).c_str(),0);

                cout << ligne << 'x' << x << 'y' << y << endl;  // on l'affiche
                image.set((1+x)*400, (1+y)*400, white);
            }
        }

        //cout << ligne << "\n" << token << endl;  // on l'affiche

        fichier.close();  // on ferme le fichier
    }
    else  // sinon
        cerr << "Impossible d'ouvrir le fichier !" << endl;

    return 0;
}



int drawLineTga(string name, TGAImage &image){
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

                vector<string> point1 = vlignes[-1+stoi(vstrings[1].substr(0, vstrings[1].find("/")))];
                vector<string> point2 = vlignes[-1+stoi(vstrings[2].substr(0, vstrings[2].find("/")))];
                vector<string> point3 = vlignes[-1+stoi(vstrings[3].substr(0, vstrings[3].find("/")))];


                drawLine((1.+(float)strtof((point1[1]).c_str(),0))*400, (1.+(float)strtof((point1[2]).c_str(),0))*400, (1.+(float)strtof((point2[1]).c_str(),0))*400, (1.+(float)strtof((point2[2]).c_str(),0))*400, image, white);

                drawLine((1.+(float)strtof((point2[1]).c_str(),0))*400, (1.+(float)strtof((point2[2]).c_str(),0))*400, (1.+(float)strtof((point3[1]).c_str(),0))*400, (1.+(float)strtof((point3[2]).c_str(),0))*400, image, white);

                drawLine((1.+(float)strtof((point3[1]).c_str(),0))*400, (1.+(float)strtof((point3[2]).c_str(),0))*400, (1.+(float)strtof((point1[1]).c_str(),0))*400, (1.+(float)strtof((point1[2]).c_str(),0))*400, image, white);

                //float x =  strtof((vstrings[1]).c_str(),0);
                //float y =  strtof((vstrings[2]).c_str(),0);

                cout << ligne << endl;  // on l'affiche
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

    return 0;
}
