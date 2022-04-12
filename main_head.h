#ifndef MAIN_H
#define MAIN_H

#include<iostream>
#include<vector>
#include<fstream>
#include<boost/algorithm/string/split.hpp>
#include<boost/algorithm/string.hpp>
#include<string>
#include <iostream>
#include <regex>
#include <map>

using namespace std;

class cell;
map<string, cell> cellId;

 class cell
{

public:

string cellName;
int width;
int height;
int weight;
int terminal;
int xCoordinate;
int yCoordinate;
int tmp1;
int tmp2;
string orientation; 
int cellRowId;
vector <int> Netlist;

void setParametercells(string cellName, int width, int height,int terminal)
   {
 	this->cellName = cellName;
 	this->width = width;
	this->height = height;
	this->terminal = terminal;
   }

void setParameterWts(int weight)
   {
 	this->weight = weight;
   }

void setParameterPl(int xCoordinate,int yCoordinate,string orientation)
{
	this->xCoordinate = xCoordinate;
	this->yCoordinate = yCoordinate;
	this->orientation = orientation;
}

void setOrientation(string orient)
{
	cout<<(this->orientation)<<"   "<<orient<<endl;
	this->orientation = orient;
	if ((orient!=this->orientation) && (this->orientation[0]!='F'))
	{
		int rotateDistance = (8 + AssignNumbertoOrientation(orient) - AssignNumbertoOrientation(this->orientation)) % 4;
		if ((rotateDistance%2==1)&&(this->width<16))
		{
			int _width = this->width;
			int _height = this->height;
			this->width = _width;
			this->height = _height;
		}
	}
}

void setRowId(int cellRowId)
{
	this->cellRowId = cellRowId;
}

void setNetList(int NetId)
{
	Netlist.push_back(NetId);
}

void setCenter(int tmp1, int tmp2)
{
	this->tmp1 = tmp1;
	this->tmp2 = tmp2;
}

int AssignNumbertoOrientation(string a)
{
	int num=0;
	switch(a[0])
	{
	case 'N' : num = 0; break;
	case 'W' : num = 1; break;
	case 'S' : num = 2; break;
	case 'E' : num = 3; break;
	default : num = 4;
	}
	return num;
}

string AssignOrientationtoNumber(int num)
{
	string a;
	switch(num)
	{
	case 0 : a = "N";break;
	case 1 : a = "W";break;
	case 2 : a = "S";break;
	case 3 : a = "E";break;
	default: a = "F";
	}
	return a;
}



class row;
map<int, row> rowId;
int RowWidth;
int xLimit;
 class row
{

public:
int Id;
int coOrdinate;
int height;
int siteWidth;
int siteSpacing;
string siteOrient;
string siteSymmetry;
int siteRowOrigin;
int numSites;
int overlap;
vector <string> cellList;

void setId(int Id)

{
	this->Id=Id;
}


void setParameterRows(int coOrdinate,int height,int siteWidth,int siteSpacing,string siteOrient,string siteSymmetry,int siteRowOrigin,int numSites)
   {
 	this->coOrdinate = coOrdinate;
 	this->height = height;
	this->siteWidth = siteWidth;
	this->siteSpacing = siteSpacing;
 	this->siteOrient = siteOrient;
	this->siteSymmetry = siteSymmetry;
	this->siteRowOrigin = siteRowOrigin;
	this->numSites = numSites;
   }

void setCellList(string cellId)
 {
	cellList.push_back(cellId);
 }

vector<string> sortByX()
{
int i=0,x=0;
map<int,string> sortx;
map<int,string>::iterator it;
vector<string>::iterator itl;
vector<string>list;
for(i=0;i<(this->cellList.size());i++)
{
	x=cellId.find(cellList[i])->second.xCoordinate;
	sortx.insert(pair<int,string> (x,cellList[i]));
}
for(it=sortx.begin();it!=sortx.end();++it)
{
	list.push_back(it->second);
}
this->cellList=list;
return this->cellList;
}

void calcRowOverlap() //
{
	vector <string> :: iterator it1;
	int xLast=0, widthLast = 0;

	xLast = cellId[cellList[cellList.size()-1]].xCoordinate;
	widthLast = cellId[cellList[cellList.size()-1]].width;
	overlap = xLast + widthLast - (RowWidth+xLimit);

	this ->overlap = overlap;
}


map<int, vector<string> > netToCell;

#endif
