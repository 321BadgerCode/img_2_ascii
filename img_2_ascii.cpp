// Badger
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "./rgb.h"

#include "../lib/CImg/CImg.h"

using namespace std;
using namespace cimg_library;

void resize(CImg<int> &image, vector<int> size);
vector<vector<int> > readImage(string filename, vector<int> size);
void readImage(string filename, vector<vector<int> > &image, vector<int> size);
void writeImage(string filename, vector<vector<int> > &image);
void convertToBlock(vector<vector<int>> &image, vector<vector<char>>& ansii);
void convertToAscii(vector<vector<int> > &image, vector<vector<char> > &ascii);
void printAscii(vector<vector<char> > &ascii);
void printBlock(CImg<int>& image, vector<vector<char>>& ansii);
void writeAscii(string filename, vector<vector<char> > &ascii);

vector<int> resizeImage(vector<int> vector1, vector<int> vector2){
	vector<int> newVector;
	newVector.resize(2);
	if(vector1[0] > vector2[0] || vector1[1] > vector2[1]){
		if(vector1[0] > vector2[0]){
			newVector[0] = vector2[0];
			newVector[1] = (int)round((double)vector1[1]/(double)vector1[0]*(double)vector2[0]);
		}
		else{
			newVector[1] = vector2[1];
			newVector[0] = (int)round((double)vector1[0]/(double)vector1[1]*(double)vector2[1]);
		}
	}
	else{
		newVector[0] = vector1[0];
		newVector[1] = vector1[1];
	}
	return newVector;
}

CImg<int> grayscale_img(CImg<int> image){
	CImg<int> gray(image.width(), image.height(), 1, 1);
	cimg_forXY(image, x, y){
		int r = image(x, y, 0);
		int g = image(x, y, 1);
		int b = image(x, y, 2);
		int gray_value = (r + g + b) / 3;
		gray(x, y, 0) = gray_value;
	}
	return gray;
}

CImg<int> invert_img(CImg<int> image){
	CImg<int> inverted(image.width(), image.height(), 1, 3);
	cimg_forXY(image, x, y){
		int r = image(x, y, 0);
		int g = image(x, y, 1);
		int b = image(x, y, 2);
		inverted(x, y, 0) = 255 - r;
		inverted(x, y, 1) = 255 - g;
		inverted(x, y, 2) = 255 - b;
	}
	return inverted;
}

bool is_approx_equal2(rgb8 color1, rgb8 color2, int tolerance=0xff/2){
	return (pow(color1.r-color2.r,2)+pow(color1.g-color2.g,2)+pow(color1.b-color2.b,2))<=pow(tolerance,2);
	/*rgb8 diff = abs(color1 - color2);
	return diff <= tolerance;*/
}

float angleBetweenVectors(const rgb<float>& color1, const rgb<float>& color2) {
	float dotProduct = color1.r * color2.r + color1.g * color2.g + color1.b * color2.b;
	float magnitude1 = sqrt(color1.r * color1.r + color1.g * color1.g + color1.b * color1.b);
	float magnitude2 = sqrt(color2.r * color2.r + color2.g * color2.g + color2.b * color2.b);

	return acos(dotProduct / (magnitude1 * magnitude2));
}

string checkColor(const rgb<float>& color) {
	rgb<float> clr[9]={black,white,red,orange,yellow,green,blue,indigo,violet};
	string clr_name[9]={"black","white","red","orange","yellow","green","blue","indigo","violet"};
	float angle[9];
	for(int a=0;a<9;a++){
		angle[a]=angleBetweenVectors(color,clr[a]);
	}
	int min=0;
	for(int a=0;a<9;a++){
		if(angle[a]<angle[min]){
			min=a;
		}
	}
	return clr_name[min];
}

bool compareColors(const rgb<float>& color1, const rgb<float>& color2, float tolerance=50) {
	bool redCheck = (std::fabs(color1.r - color2.r) <= tolerance);
	bool greenCheck = (std::fabs(color1.g - color2.g) <= tolerance);
	bool blueCheck = (std::fabs(color1.b - color2.b) <= tolerance);

	return (redCheck && greenCheck && blueCheck);
}

string get_compare_clr_name(const rgb<float>& clr){
	rgb<float> clr_list[9]={black,white,red,orange,yellow,green,blue,indigo,violet};
	string clr_name[9]={"black","white","red","orange","yellow","green","blue","indigo","violet"};
	for(int a=0;a<9;a++){
		if(compareColors(clr,clr_list[a])){
			return clr_name[a];
		}
	}
	return "black";
}

string get_color_name(rgb8 color){
	if(is_approx_equal2(color, black)){
		return "black";
	}
	else if(is_approx_equal2(color, white)){
		return "white";
	}
	else if(is_approx_equal2(color, red)){
		return "red";
	}
	else if(is_approx_equal2(color, orange)){
		return "orange";
	}
	else if(is_approx_equal2(color, yellow)){
		return "yellow";
	}
	else if(is_approx_equal2(color, green)){
		return "green";
	}
	else if(is_approx_equal2(color, blue)){
		return "blue";
	}
	else if(is_approx_equal2(color, indigo)){
		return "indigo";
	}
	else if(is_approx_equal2(color, violet)){
		return "violet";
	}
	else{
		return "black";
	}
}

string get_var_name(string filename){
	string ans="";
	for(int a=0;a<filename.length();a++){
		if(isalpha(filename[a])||(a>0 && isdigit(filename[a]))||filename[a]=='_'){
			ans+=filename[a];
		}
		else if(filename[a]=='/'){
			ans+='_';
		}
		else if(filename[a]==' '){
			ans+='_';
		}
		else{
			continue;
		}
	}
	return ans;
}

string get_code(string filename,CImg<int> image,vector<vector<char>>& ascii){
	string ans="";
	cimg_forXY(image, x, y){
		int r = image(x, y, 0);
		int g = image(x, y, 1);
		int b = image(x, y, 2);
		rgb8 color(r, g, b);
		string color_name = get_color_name(color);
		char ascii_char = ascii[y][x];
		string end=(x==image.width()-1 && y==image.height()-1)?";":"+";
		string newline="";
		if(x==image.width()-1){ans+="\n";newline+="\\n";}
		ans+="(string)"+color_name+"+\""+string(1,ascii_char)+"\\033[0m"+newline+"\""+end;
	}
	return "#pragma once\n\n#include \"./rgb.h\"\n\nstring "+get_var_name(filename)+"="+ans;
}

void save(string filename, string txt){
	ofstream fout(filename.c_str());
	if(!fout){
		cout<<"Error opening file "<<filename<<endl;
		exit(1);
	}
	fout<<txt;
	fout.close();
}

int main(int argc, char *argv[]){
	CImg<int> img(argv[1]);
	CImg<int> gray = grayscale_img(img);
	CImg<int> inverted = invert_img(gray);
	if(!img){
		cout<<"Error opening file "<<argv[1]<<endl;
		return 1;
	}

	if(argc < 2){
		cout << "Usage: ./ascii <image file>" << endl;
		return 1;
	}
	vector<int> terminal_size={80, 24};
	vector<int> size={img.width(), img.height()};
	if(argc > 2){
		size=argc>3?resizeImage(size, {atoi(argv[2]), atoi(argv[3])}):resizeImage(size, {(int)(size[0]*((float)atoi(argv[2])/100)), (int)(size[1]*((float)atoi(argv[2])/100))});
	}

	vector<vector<int> > image;
	readImage(argv[1], image, size);

	vector<vector<char> > ascii;
	convertToAscii(image, ascii);

	vector<vector<char>> ansii;
	convertToBlock(image, ansii);

	resize(img, size);
	printBlock(img,ansii);
	writeAscii("image_2_ascii.txt", ascii);

	gray.save("./grayscale.png");
	inverted.save("./grayscale_inverted.png");

	string code = get_code(argv[1],img,ascii);
	vector<vector<int>> image_inverted;
	readImage("./grayscale_inverted.png",image_inverted,size);
	vector<vector<char>> inverted_ascii;
	convertToAscii(image_inverted, inverted_ascii);
	save("./img_2_ascii_code.h",code);
	save("./img_2_ascii_code_inverted.h",get_code((string)argv[1]+"_inverted",img,inverted_ascii));

	return 0;
}

void printAscii(CImg<int>& image, vector<vector<char> > &ascii)
{
	for(int i = 0; i < ascii.size(); i++)
	{
		for(int j = 0; j < ascii[i].size(); j++)
			cout << ascii[i][j];
		cout << endl;
	}
}

void printBlock(CImg<int>& image, vector<vector<char>>& ansii){
	for(int i = 0; i < ansii.size(); i++){
		for(int j = 0; j < ansii[i].size(); j++){
			int r = image(j, i, 0);
			int g = image(j, i, 1);
			int b = image(j, i, 2);
			rgb8 clr(r, g, b);
			cout<<(string)clr<<ansii[i][j]<<"\033[0m";
		}
		cout<<endl;
	}
}

void writeAscii(string filename, vector<vector<char> > &ascii)
{
	ofstream fout(filename.c_str());

	if(!fout)
	{
		cout << "Error opening file " << filename << endl;
		exit(1);
	}

	for(int i = 0; i < ascii.size(); i++)
	{
		for(int j = 0; j < ascii[i].size(); j++)
			fout << ascii[i][j];
		fout << endl;
	}

	fout.close();
}

void resize(CImg<int> &image, vector<int> size)
{
	image.resize(size[0],size[1]);
}

vector<vector<int> > readImage(string filename, vector<int> size)
{
	CImg<int> image(filename.c_str());
	resize(image, size);

	vector<vector<int> > imageVector;

	imageVector.resize(image.height());
	for(int i = 0; i < image.height(); i++)
	{
		imageVector[i].resize(image.width());
	}

	for(int i = 0; i < image.height(); i++)
	{
		for(int j = 0; j < image.width(); j++)
			imageVector[i][j] = image(j, i);
	}

	return imageVector;
}

void readImage(string filename, vector<vector<int>> &image, vector<int> size){
	image = readImage(filename, size);
}

void writeImage(string filename, vector<vector<int> > &image)
{
	ofstream fout(filename.c_str());

	if(!fout){
		cout << "Error opening file " << filename << endl;
		exit(1);
	}

	fout << image.size() << " " << image[0].size() << endl;

	for(int i = 0; i < image.size(); i++){
		for(int j = 0; j < image[i].size(); j++)
			fout << image[i][j] << " ";
		fout << endl;
	}

	fout.close();
}

void convertToAscii(vector<vector<int> > &image, vector<vector<char> > &ascii)
{
	ascii.resize(image.size());
	for(int i = 0; i < image.size(); i++)
	{
		ascii[i].resize(image[i].size());
	}

	for(int i = 0; i < image.size(); i++)
	{
		for(int j = 0; j < image[i].size(); j++)
		{
			if(image[i][j] < 25)
				ascii[i][j] = '@';
			else if(image[i][j] < 50)
				ascii[i][j] = '#';
			else if(image[i][j] < 75)
				ascii[i][j] = '%';
			else if(image[i][j] < 100)
				ascii[i][j] = '*';
			else if(image[i][j] < 125)
				ascii[i][j] = '+';
			else if(image[i][j] < 150)
				ascii[i][j] = '=';
			else if(image[i][j] < 175)
				ascii[i][j] = '-';
			else if(image[i][j] < 200)
				ascii[i][j] = ':';
			else if(image[i][j] < 225)
				ascii[i][j] = '.';
			else
				ascii[i][j] = ' ';
		}
	}
}

void convertToBlock(vector<vector<int>> &image, vector<vector<char>>& ansii){
	ansii.resize(image.size());
	for(int i = 0; i < image.size(); i++){
		ansii[i].resize(image[i].size());
	}
	for(int i = 0; i < image.size(); i++){
		for(int j = 0; j < image[i].size(); j++){
			ansii[i][j] = '*';
		}
	}
}