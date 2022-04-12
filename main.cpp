#include<iostream>
#include<vector>
#include<fstream>
#include<boost/algorithm/string/split.hpp>
#include<boost/algorithm/string.hpp>
#include<string>
#include <iostream>
#include <sstream>
#include <regex>
#include <map>
#include <cmath>
#include "time.h"
#include <stdlib.h>
#include "main_head.h"

#define basepath        "C:\\Users\\aabhyudai\\Documents\\"
#define ibmFile         "ibm01"

string netsFile="";
string cellsFile="";
string plFile="";
string wtFile="";
string sclFile="";

double Temperature;
int  numcells =0;
float numCellsPerRow = 0;

struct Point_limits
{
int minX, maxX, minY, maxY;
};

Point_limits b;

int main()
{
	//////////////////////////////////////////////////////////////
    //CONCATENATING NAMES OF FILES FOR READING
    std::stringstream tempString;
    tempString<<basepath<<ibmFile<<"\\"<<ibmFile<<".nets";
    netsFile=tempString.str();

    std::stringstream tempString1;
    tempString1<<basepath<<ibmFile<<"\\"<<ibmFile<<".cells";
    cellsFile=tempString1.str();

    std::stringstream tempString2;
    tempString2<<basepath<<ibmFile<<"\\"<<ibmFile<<".pl";
    plFile=tempString2.str();

    std::stringstream tempString3;
    tempString3<<basepath<<ibmFile<<"\\"<<ibmFile<<".wts";
    wtFile=tempString3.str();

    std::stringstream tempString4;
    tempString4<<basepath<<ibmFile<<"\\"<<ibmFile<<".scl";
    sclFile=tempString4.str();

    //////////////////////////////////////////////////////////////
    //DOCUMENTING TIME OF EXECUTION FOR EACH FILE (Variable: TOE)
    time_t TOE_start = time(NULL);
    time_t TOE;


    readBenchmarkFiles();
	findCenterOfBox();
	CreateCellGraph();
	WriteToOuputFile("before1.txt");
	timberWolfAlgorithm();
	WriteToOuputFile("after1.txt");

	TOE = time(NULL)-TOE_start;
	printf( "Time of execution(s) for %s: %4.1f\n",(double)TOE);
	return 0;
}

void readBenchmarkFiles()
   {
    	fstream file;
    	string fileLine;
        int row=0;
    	vector<string> stringTemp; //A vector of strings
    	using boost::is_any_of;
        int Value=2;

		file.open(cellsFile,ios::in);
		while (getline(file, fileLine))
		{
			row++;
			if(row>7)
			{
				boost::algorithm::split(stringTemp,fileLine,is_any_of(" ,\t"),boost::token_compress_on);
				cell n;
				if (stringTemp[4]=="terminal")
				{
					Value = 1;
				}
				else
				{
					Value = 0;
				}
				n.setParametercells(stringTemp[1],atoi(stringTemp[2].c_str()),atoi(stringTemp[3].c_str()),Value);
				cellId.insert(pair<string,cell>(stringTemp[1],n));
			}
		}
		file.close();

		row=0;

		map<string,cell>::iterator itr;

		file.open(wtFile, ios :: in);

		while (getline(file, fileLine))
		{
			row++;
			if(row>5)
			{
				boost::algorithm::split(stringTemp,fileLine,is_any_of(" ,\t"),boost::token_compress_on);
				cellId[stringTemp[1]].setParameterWts(atoi(stringTemp[2].c_str()));

			}
		}
		file.close();

		row=0;

		file.open(wtFile, ios :: in);

		while (getline(file, fileLine))
		{
			row++;
			if(row>5)
			{
				boost::algorithm::split(stringTemp,fileLine,is_any_of(" ,\t"),boost::token_compress_on);
				cellId[stringTemp[1]].setParameterWts(atoi(stringTemp[2].c_str()));

			}
		}
		file.close();

		row=0;

		file.open(plFile,ios :: in);

		while (getline(file,fileLine))
		{
			row++;
			if(row>6)
			{
				boost::algorithm::split(stringTemp,fileLine,is_any_of(" ,\t"),boost::token_compress_on);
				cellId[stringTemp[0]].setParameterPl(atoi(stringTemp[1].c_str()),atoi(stringTemp[2].c_str()),stringTemp[4]);

			}
		}
		file.close();

		row=0; NetId=1; unsigned int a=0,j;

		file.open(netsFile, ios :: in);

		regex find ("\\b(NetDegree : )");
		smatch key;
		string temp;

		while (getline(file, fileLine))
		{
			row++;
			if(row>7)
			{
				regex_search(fileLine, key, find);
				temp = key.suffix();
				a = atoi(temp.c_str());
				vector<string> strTemp;
				for (j=0; j<a; j++)
				{

					getline(file,fileLine);
					boost::algorithm::split(stringTemp,fileLine,is_any_of(" ,\t"),boost::token_compress_on);
					strTemp.push_back(stringTemp[1]);
					cellId[stringTemp[1]].setNetList(NetId);
				}
				netToCell.insert(pair<int, vector <string> >(NetId,strTemp));
				NetId++;
			}

		}

		file.close();

		unsigned int rowCoordinate,row1;
		unsigned int rowheight;
		unsigned int siteWidth;
		unsigned int siteSpacing;
		unsigned int siteSymmetry;
		unsigned int siteRowOrigin;

		row=0; row1=0; Id=1;

		file.open(sclFile, ios::in);

		while (getline(file, fileLine))
		{
			row++;
			if(row>8)
			{
				boost::algorithm::split(stringTemp,fileLine,is_any_of(" ,\t"),boost::token_compress_on);
				//After Every 9th row, The counter for rows goes down to 0
				row1 = row%9;
				switch(row1)
				{
					case 1: rowCoordinate = atoi(stringTemp[3].c_str()); break;
					case 2: rowheight = atoi(stringTemp[3].c_str()); break;
					case 3: siteWidth = atoi(stringTemp[3].c_str()); break;
					case 4: siteSpacing = atoi(stringTemp[3].c_str()); break;
					case 5: siteOrient = stringTemp[3]; break;
					case 6: siteSymmetry = stringTemp[3]; break;
					case 7: {siteRowOrigin = atoi(stringTemp[3].c_str());
							numSites = atoi(stringTemp[6].c_str()); break;}
					case 8: {row r; //for each object of the class row
							r.setId(Id);
							rowId.insert(pair<int,row>(Id,r));
							rowId[Id].setParameterRows(rowCoordinate,rowheight,siteWidth,siteSpacing,siteOrient,siteSymmetry,siteRowOrigin,numSites);
							Id++;break;}
				}
			}
		}
	file.close();

   }



void boundingBoxCreation()
{
	int xval,yval;
	b.minX = 32767, b.maxX = -32768,b.minY = 32767, b.maxY = -32768;
	map<string, cell> ::iterator itcell;
	for(itcell = cellId.begin(); itcell != cellId.end(); ++itcell)
	{
		if(itcell->second.terminal == 1)
		{
			xval = itcell->second.xCoordinate;
			yval = itcell->second.yCoordinate;
			if(xval < b.minX)
			{
				b.minX = xval;
			}
			if(xval > b.maxX)
			{
				b.maxX = xval;
			}
			if(xval < b.minY)
			{
				b.minY = yval;
			}
			if(xval > b.maxY)
			{
				b.maxY = yval;
			}

		}
	}
	if (b.minX==32767)
		cout<<"error"<<endl;
		b.minX = 0;
	if (b.minY == 32767)
		b.minY = 0;
}

void CreateCellGraph()
{
	map<int, row> ::iterator itRow;
	map<string, cell> ::iterator itcell;
	for(itRow = rowId.begin(); itRow != rowId.end(); ++itRow)
	{
		for(itcell = cellId.begin(); itcell != cellId.end(); ++itcell)
		{
			if(itcell->second.height = 16)
			{
			if((itRow->second.coOrdinate <= itcell->second.yCoordinate) && ((itRow->second.coOrdinate)+(itRow->second.height)) >= itcell->second.yCoordinate)
			{
				itcell->second.setRowId(itRow->first);
				itRow->second.setCellList(itcell->first);
			}
			}
		}
	}
}

void WriteToOuputFile(string file_name)
{
	ofstream myfile(file_name);
  	if (myfile.is_open())
  	{
		int x = 0,y = 0,w = 0,h = 0;

		for(map<string,cell>::iterator it_cell = cellId.begin();it_cell != cellId.end();++it_cell)
		{

	     		x = it_cell->second.xCoordinate;
      			y = it_cell->second.yCoordinate;
      			w = it_cell->second.width;
      			h = it_cell->second.height;

			myfile << x << " " << y << " "<< w<< " "<<h<<" "<<endl;
		}
  	}
  	else cout << "Unable to open file";
}

int macroPlacement()
{
	int xValue=b.minX,yValue=0;
	xLimit=b.minX;
	map<int, row> ::iterator itRow;
	itRow = rowId.begin();
	int rowHeight = itRow->second.height;

	map<string,cell>::iterator itcell;
	for(itcell = cellId.begin();itcell != cellId.end();++itcell)
	{
		if(itcell->second.terminal == false && itcell->second.height > rowHeight)
		{
			if(xValue + itcell->second.width > xLimit)
			{
				xLimit = xValue + itcell->second.width+1;
			}
			if(yValue + itcell->second.height < b.maxY)
			{
				itcell->second.yCoordinate = yValue;
				itcell->second.xCoordinate = xValue;
			}
			else
			{
				yValue = 0;
				xValue = xLimit;
				itcell->second.yCoordinate = yValue;
				itcell->second.xCoordinate = xValue;
			}
			yValue = yValue + itcell->second.height;
		}
	}
	return xLimit;
}

void initialPlacement()
{
map<string, cell> ::iterator itcell;
map<int, row> ::iterator itRow=rowId.begin();
int totalWidth = 0, rowWidth = 0, cnt=0,count=0, xCord=xLimit, yCord=0;
for(itcell = cellId.begin(); itcell != cellId.end(); ++itcell)
{
	if(itcell->second.terminal == 0 && itcell->second.height == 16)
	{
		totalWidth += itcell->second.width;
		numcells++;
	}
}
itcell = cellId.begin();

totalWidth = totalWidth + numcells;
rowWidth = totalWidth/rowId.size();
numCellsPerRow = ceil(float (numcells)/rowId.size())-1;


cout<<"Total Number of Rows="<<rowId.size()<<endl;
cout<<"Total Number of Cells="<<numcells<<endl;

for(itcell = cellId.begin(); itcell != cellId.end(); ++itcell)
{
	if(itRow != rowId.end() && itcell->second.terminal == 0 && itcell->second.height <= itRow->second.height && cnt < (numCellsPerRow))
	{
		itcell->second.yCoordinate = itRow->second.coOrdinate;
		itcell->second.xCoordinate = xCord;
		xCord += itcell->second.width+1;
		cnt++;

		if(cnt == numCellsPerRow)
		{
			cnt = 0;
			xCord = xLimit;
			++itRow;
		}
	}
}
findCenterOfBox();
CreateCellGraph();
}

void findCenterOfBox()
  {
	map<string, cell> ::iterator itcell;

	int xBy2 =0, yBy2 =0;
	for(itcell = cellId.begin(); itcell != cellId.end(); ++itcell)
	{
		xBy2 = (itcell->second.xCoordinate)+((itcell->second.width)/2);
		yBy2 = (itcell->second.yCoordinate)+((itcell->second.height)/2);
		itcell->second.setCenter(xBy2,yBy2);
	}
  }

long int wirelength_Calculation()
{
	map<int, vector<string>> :: iterator itNet;
	vector<string> :: iterator itCellList;
	int xVal, yVal, wirelength_Calculation=0;
	for(itNet= netToCell.begin(); itNet != netToCell.end(); ++itNet)
	{
			int minXW = 32767, minYW = 32767, maxXW = -32768 , maxYW = -32768;
			for(itCellList = itNet->second.begin(); itCellList != itNet->second.end(); ++itCellList)
			{
				xVal = cellId[*itCellList].xBy2;
				yVal = cellId[*itCellList].yBy2;
				if(xVal < minXW)
				minXW = xVal;
				if(xVal > maxXW)
				maxXW = xVal;
				if(yVal < minYW)
				minYW = yVal;
				if(yVal > maxYW)
				maxYW = yVal;
			}
			wirelength_Calculation += abs((maxXW-minXW)) + abs((maxYW-minYW));
	}
return wirelength_Calculation;
}

long int cellOverlap()
{

map<string,cell>::iterator cellit=cellId.begin();
map<int,row>::iterator rowit;
int x1=0,w1=0,x2=0,w2=0,h1=0,h2=0,overlap=0,i=0;
long int totaloverlap=0;

vector<string>list;
for(rowit=rowId.begin();rowit!=rowId.end();++rowit)
{
	overlap=0;
	list=rowit->second.sortByX();
	overlap=0;
	for(i=0;i<list.size();i++)
	{
		cell cellobj=cellId.find(list[i])->second;
		x1=cellobj.xCoordinate;
		w1=cellobj.width;
		h1=cellobj.height;
		i++;
		if(i==list.size())
		{
			break;
		}
		x2=cellId.find(list[i])->second.xCoordinate;
		w2=cellId.find(list[i])->second.width;
		h2=cellId.find(list[i])->second.height;
		h1 = h2>h1?h1:h2;
		if(x2<=(x1+w1)&&(x2+w2)>=(x1+w1))
		{
			overlap+=((x1+w1)-x2)*h1;
		}
		else if(x2>=x1 && x2<=(x1+w1) && (x2+w2)<=(x1+w1))
		{
			overlap+=w2*h1;
		}
		i--;
	}
totaloverlap+=overlap;

}
return totaloverlap;
}

void initiateMove(int xLimit)
{
srand(time(NULL));
int randomCellnum, randRow, randXcord;
stringstream randomCellTemperature;
string randomCellStr,randomCell, randomCell2;
cell n;
row r;
map<string, cell> ::iterator itcell, itcell2;
map<int, row> ::iterator itRow, itRow2;
bool accept;
long int cost2,cost1,prevCost;
	cost1 = wirelength_Calculation();
	cost2 = cellOverlap();
	prevCost = cost1 + cost2;
	randomCellnum = rand() %  numcells +1;
	randomCellTemperature << randomCellnum;
	randomCellStr = randomCellTemperature.str();
	randomCell = "a" + randomCellStr;

if (rand()/(RAND_MAX+0.0)<=0.8)
{
	randRow = rand() % (rowId.size()) + 1;
	randXcord = rand() %((int)numCellsPerRow)+xLimit;

	itRow = rowId.find(randRow);
	itcell = cellId.find(randomCell);
	n = itcell->second;
	itcell->second.yCoordinate = itRow->second.coOrdinate;
	itcell->second.xCoordinate = randXcord;

	rowId[itcell->second.cellRowId].cellList.erase(std::remove(rowId[itcell->second.cellRowId].cellList.begin(), rowId[itcell->second.cellRowId].cellList.end(),randomCell), rowId[itcell->second.cellRowId].cellList.end());
	itRow->second.cellList.push_back(randomCell);
	itcell->second.cellRowId = randRow;
	findCenterOfBox();

	accept = validateSwap(prevCost);

	if(!accept)
	{
		itcell->second = n;
		itRow->second.cellList.erase(std::remove(itRow->second.cellList.begin(),itRow->second.cellList.end(),randomCell), itRow->second.cellList.end());
		rowId[itcell->second.cellRowId].cellList.push_back(randomCell);
		if (rand()/(RAND_MAX+0.0)<=0.1)
		{
			itcell->second.setOrientation(intToOrient(rand()%4));
			accept = validateSwap(prevCost);
			if(!accept)
				itcell->second = n;
		}
	}
}
else
{
	randomCellnum = rand() %  numcells +1 ;
	randomCellTemperature.str("");
	randomCellTemperature << randomCellnum;
	randomCellStr = randomCellTemperature.str();
	randomCell2 = "a" + randomCellStr;

	cout<<"exchange "<<randomCell<<" with "<<randomCell2<< endl;

	int n1, n2;
	cell cellSec1, cellSec2;
	itcell = cellId.find(randomCell);
	itcell2 = cellId.find(randomCell2);
	n1 = itcell->second.cellRowId; n2 = itcell2->second.cellRowId;
	itRow = rowId.find(n1);
	itRow2 = rowId.find(n2);
	cellSec1 = itcell->second;
	cellSec2 = itcell2->second;


	int xTemp, yTemp;
	xTemp = itcell->second.xCoordinate;
	yTemp = itcell->second.yCoordinate;
	itcell->second.xCoordinate = itcell2->second.xCoordinate;
	itcell->second.yCoordinate = itcell2->second.yCoordinate;
	itcell2->second.xCoordinate = xTemp;
	itcell2->second.yCoordinate = yTemp;

	itRow->second.cellList.erase(std::remove(itRow->second.cellList.begin(), itRow->second.cellList.end(),randomCell), itRow->second.cellList.end());
	itRow->second.cellList.push_back(randomCell2);
	itcell->second.cellRowId = n2;

	itRow2->second.cellList.erase(std::remove(itRow2->second.cellList.begin(), itRow2->second.cellList.end(),randomCell2), itRow2->second.cellList.end());
	itRow2->second.cellList.push_back(randomCell);
	itcell2->second.cellRowId = n1;

	findCenterOfBox();

	accept = validateSwap(prevCost);
	if(!accept)
	{
		n1 = itcell->second.cellRowId; n2 = itcell2->second.cellRowId;
		itcell->second = cellSec1;
		itRow->second.cellList.erase(std::remove(itRow->second.cellList.begin(),itRow->second.cellList.end(),randomCell2), itRow->second.cellList.end());
		itRow->second.cellList.push_back(randomCell);

		itcell2->second = cellSec2;
		itRow2->second.cellList.erase(std::remove(itRow2->second.cellList.begin(),itRow2->second.cellList.end(),randomCell), itRow2->second.cellList.end());
		itRow2->second.cellList.push_back(randomCell2);
	}
}
findCenterOfBox();
}


void update_Temperature()
{
if(Temperature>30000)
{
	Temperature=0.8*Temperature;
}
else if(Temperature<=30000 && Temperature >2000)
{
	Temperature=0.94*Temperature;
}
else if(Temperature<=2000 && Temperature<20)
{
	Temperature=0.8*Temperature;
}
else if(Temperature<20)
{
	Temperature=0.1*Temperature;
}
}

bool validateSwap(long int prevCost)
{
srand(time(NULL));
long int cost2=0,cost1=0,newCost=0;
int delCost=0;
double factor=0;
double prob = rand()/(RAND_MAX+0.0);

		cost1 = wirelength_Calculation();
		cost2 = cellOverlap();
		newCost = cost1 + cost2;
		delCost = newCost - prevCost;
		factor = exp(-delCost/Temperature);

		if(delCost < 0)
		{
			prevCost = newCost;
			return true;
		}
		else
		{
			return false;
		}
}

void timberWolfAlgorithm()
{
	initialPlacement();
	Temperature = 40000;
	int i;

	while (Temperature > 1 )
	{
		i=5;
		while(i > 0)
		{
			initiateMove(xLimit);
			i--;
		}
		update_Temperature();

	}

}
