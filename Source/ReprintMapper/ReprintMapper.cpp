// Reprint Mapper v1.0.0
// John Christian Wood and M. H. Beals (2016) ReprintMapper v1,0.0 [Software]
// Available at https://github.com/mhbeals/ReprintMapper

// MIT License

// Copyright(c) 2016 John Christian Wood and M. H. Beals

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// // to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>

using namespace std;

int MaxDaysBetween = 200; //up to 1 year maximum, otherwise change the comparison logic
int OL_Tolerance = 90; //minimum left match
int OR_Tolerance = 90; //minimum right match
int PM_Tolerance = 160; //minimum overall or perfect match

struct Article //list all unique articles present in the input data
{
	string UI; //unique id of the article
	string Newspaper; //newspaper title
	unsigned int Year;
	unsigned int Month;
	unsigned int Day;
};

struct Comparison //list all match sets for comparison
{
	unsigned int PM; //perfect match
	unsigned int OL; //left match
	unsigned int OR; //right match
	bool bIsOriginal; //boolean indicating whether it is a successful source 
	Article A;//filename of target
	Article B;//filename of source
};


struct Results //final manifest
{
	string UID; //unique id of target article
	string TargetUID; //unique id of source article
	bool bIsOriginal; //boolean indicating a successful source
	bool bIsDeadEnd; //boolean indicating a dead-end source
};

vector<Comparison> Comparisons; //vector for comparison calculations

Comparison Comp; //this is used to initialise new entries in the Comparisons vector using push_back

vector<Results> UniqueArticle; //vector containing the UI of all the unique articles that meet minimum requirements

Results temp; //used to initialise new entries in the Results vector using push_back

int NumLines = 0; //reset initial line count

bool IsLeap(int yr) //determine of the target date is in a leap year
{
	if (yr % 4 != 0)
		return false;
	else if (yr % 100 != 0)
		return true;
	else if (yr % 400 != 0)
		return false;
	else
		return true;
}

int MonthToDays(int month, bool bIsLpYr)//convert days, months and years into days
{
	if (!bIsLpYr) //if it is a leap year
	{
		switch (month)
		{
		case 1: //january
			return 0;
		case 2: //february
			return 31; // includes days in January
		case 3: //march
			return 59; // includes days in January and leap year February
		case 4: //april
			return 90;
		case 5: //may
			return 120;
		case 6: //june
			return 151;
		case 7: //july
			return 181;
		case 8: //august
			return 212;
		case 9: //september
			return 243;
		case 10://october
			return 273;
		case 11://november
			return 304;
		case 12://december
			return 334;
		default:
			return 0;
		}
	}
	else // if not a leap year
	{
		switch (month)
		{
		case 1:
			return 0;
		case 2:
			return 31;
		case 3:
			return 60;
		case 4:
			return 91;
		case 5:
			return 121;
		case 6:
			return 152;
		case 7:
			return 182;
		case 8:
			return 213;
		case 9:
			return 244;
		case 10:
			return 274;
		case 11:
			return 305;
		case 12:
			return 335;
		default:
			return 0;
		}
	}
}

void GenerateComparisonDatabase()
{
	string location = "C:\\users\\username\\Desktop\\manifest.txt"; //set arbitrary input location
	cout << "Your current input file is: " << location << endl; 
	cout << "What is your actual input file? ";
	getline(cin, location); //set correct input location

	ifstream Data(location); //load manifest and populate comparison database

	int PM; //perfect match
	int OL; //left match
	int OR; //right match
	string UI_A; //unique id of target title
	string UI_B; //unique id of source title
	string A_Newspaper; //target title
	string B_Newspaper; //source title
	int A_Year; //target year
	int A_Month; //target month
	int A_Day; //target day
	int B_Year; //source year
	int B_Month; //source month
	int B_Day; //source day
	string Section;
	int i = 0;
	bool bUI_A_Done = false; //mark target as uncompared
	bool bUI_B_Done = false; //mark source as uncompared
	int scratch = 0;

	while (Data >> PM >> OL >> OR >> A_Year >> A_Month >> A_Day >> A_Newspaper >> UI_A >> B_Year >> B_Month >> B_Day >> B_Newspaper >> UI_B) //while data is available
	{
		++NumLines;
		if (A_Newspaper == B_Newspaper) //do not include matches from same title
		{
		}
		else if (OR <= OR_Tolerance && OL <= OL_Tolerance && OR + OL <= PM_Tolerance) //remove small matches
		{
		}
		else if (A_Year > B_Year) //if the target year is later than the source year
		{
			if (A_Year - B_Year == 1) // and they are concurrent years
			{
				int A_Days = MonthToDays(A_Month, IsLeap(A_Year)) + A_Day;//calculate numbers days into target year 
				bool bIsLeapYear = IsLeap(B_Year); //determine if the source year is a leap year
				int B_Days = MonthToDays(B_Month, bIsLeapYear) + B_Day; //calculate the number of days into source year
				if (bIsLeapYear)
					B_Days = 366 - B_Days; //calculate the number of days remaining in leap source year
				else
					B_Days = 365 - B_Days; //calculate the number of days remaining in non-leap source year
				if (A_Days + B_Days <= MaxDaysBetween) //if days between is fewer than 200, proceed to print item in comparisons database
				{
					Comparisons.push_back(Comp);
					Comparisons[i].PM = PM; //perfect match
					Comparisons[i].OL = OL; //left match
					Comparisons[i].OR = OR; //right match
					Comparisons[i].bIsOriginal = false;
					Comparisons[i].A.UI = UI_A; //unique id for target
					Comparisons[i].A.Newspaper = A_Newspaper; // target title
					Comparisons[i].A.Year = A_Year; //target year
					Comparisons[i].A.Month = A_Month; //target month
					Comparisons[i].A.Day = A_Day; //target day
					Comparisons[i].B.UI = UI_B; //unique id for source
					Comparisons[i].B.Newspaper = B_Newspaper; //source title
					Comparisons[i].B.Year = B_Year;//source year
					Comparisons[i].B.Month = B_Month;//source month
					Comparisons[i].B.Day = B_Day;//source day
					++i;
					bUI_A_Done = false; //reset done variable
					bUI_B_Done = false; //reset done variable
					for (unsigned int it = 0; it != size(UniqueArticle); ++it)//for every item in the unique id list
					{
						if (UniqueArticle[it].UID == UI_A) //if target article is already in unique id list
							bUI_A_Done = true; //mark target article as checked
						if (UniqueArticle[it].UID == UI_B) //if source article is already in unique id list
							bUI_B_Done = true; //mark source article as checked
					}
					if (!bUI_A_Done) //if target article is not marked as checked
					{
						scratch = size(UniqueArticle);
						UniqueArticle.push_back(temp);
						UniqueArticle[scratch].UID = UI_A; //add target article to unique id list
					}
					if (!bUI_B_Done) //if source article is not marked as checked
					{
						scratch = size(UniqueArticle);
						UniqueArticle.push_back(temp);
						UniqueArticle[scratch].UID = UI_B; //add target article to unique id list
					}
				}
			}
		}

		else if (A_Month > B_Month || A_Month == B_Month && A_Day > B_Day) //if they are the same year and the target month is later than the source month, or they are the same month and the target day is later than the source day
		{
			int A_Days = MonthToDays(A_Month, IsLeap(A_Year)) + A_Day; //calculate days in target year so far
			int B_Days = MonthToDays(B_Month, IsLeap(B_Year)) + B_Day; //calculate days in source year so far
			if (A_Days - B_Days <= MaxDaysBetween) //calculate days between articles to see if equal or under 200; if so, proceed
			{
				Comparisons.push_back(Comp); //add to Comparisons list
				Comparisons[i].PM = PM; //perfect match
				Comparisons[i].OL = OL; //left match
				Comparisons[i].OR = OR; //right match
				Comparisons[i].bIsOriginal = false; //boolean to indicate successful source
				Comparisons[i].A.UI = UI_A; //unique id for target
				Comparisons[i].A.Newspaper = A_Newspaper; //target title
				Comparisons[i].A.Year = A_Year; //target year
				Comparisons[i].A.Month = A_Month; //target month
				Comparisons[i].A.Day = A_Day; //target day
				Comparisons[i].B.UI = UI_B; //unique id for source 
				Comparisons[i].B.Newspaper = B_Newspaper; //source title
				Comparisons[i].B.Year = B_Year; //source year
				Comparisons[i].B.Month = B_Month; //source month
				Comparisons[i].B.Day = B_Day; //source day
				++i;
				bUI_A_Done = false; //reset done variable
				bUI_B_Done = false; //reset done variable
				for (unsigned int it = 0; it < size(UniqueArticle); ++it) //for every item in the unique id list
				{
					if (UniqueArticle[it].UID == UI_A) //if target article is already in unique id list
						bUI_A_Done = true; //mark target article as checked
					if (UniqueArticle[it].UID == UI_B) //if target article is already in unique id list
						bUI_B_Done = true; //mark target article as checked
				}
				if (!bUI_A_Done) //if source article is not marked as checked
				{
					scratch = size(UniqueArticle);
					UniqueArticle.push_back(temp);
					UniqueArticle[scratch].UID = UI_A; //add target article to unique id list
				}
				if (!bUI_B_Done) //if source article is not marked as checked
				{
					scratch = size(UniqueArticle);
					UniqueArticle.push_back(temp);
					UniqueArticle[scratch].UID = UI_B; //add source article to unique id list
				}
			}
		}
	}
}

void ProcessSelectedComparisons() //Map Reprints
{
	string UID; //unique article id
	unsigned int targetIndex;
	string BestMatch; //variable to indicate highest perfect match score
	unsigned int Score; //perfect match score
	bool bIsEarliest;

	for (unsigned int it = 0; it < size(UniqueArticle); ++it) //for each unique article id
	{
		UID = UniqueArticle[it].UID; //set UID to unique article id
		targetIndex = 0; //set targetIndex to null
		BestMatch = ""; //set BestMatch to null
		Score = 0; //set Score to null
		bIsEarliest = false; //boolean to indicate if it is earliest match
		unsigned int year = 0; //set year to null
		unsigned int month = 0; //set month to null
		unsigned int day = 0; //set day to null
		for (unsigned int it2 = 0; it2 < size(Comparisons); ++it2) //go through the Comparisons list
		{
			if (Comparisons[it2].A.UI == UID) //if a target article in that line of the Comparisons matches current unique article id
			{
				if (Comparisons[it2].PM > Score) //check to see if better perfect match has already been found; if not, proceed
				{
					if (Comparisons[it2].B.Year > Comparisons[it2].A.Year || Comparisons[it2].B.Year == Comparisons[it2].A.Year && Comparisons[it2].B.Month > Comparisons[it2].A.Month || Comparisons[it2].B.Year == Comparisons[it2].A.Year && Comparisons[it2].B.Month == Comparisons[it2].A.Month && Comparisons[it2].B.Day > Comparisons[it2].A.Day) //if source article is later than target article; should not occur.
					{
						BestMatch = Comparisons[it2].B.UI; //unique id of source
						Score = Comparisons[it2].PM; //set score as current best score
						bIsEarliest = true; //label target as earliest
						year = Comparisons[it2].B.Year;  //set source year as year
						month = Comparisons[it2].B.Month; //set source month as month
						day = Comparisons[it2].B.Day; //set source day as day
					}
					else if (!bIsEarliest) //if no match has been found yet, print
					{
						BestMatch = Comparisons[it2].B.UI; //unique id of source
						Score = Comparisons[it2].PM; //set score as current best score
						bIsEarliest = false; //allows it to keep searching for better matches
						year = Comparisons[it2].B.Year; // set source year as year
						month = Comparisons[it2].B.Month; //set source month as month
						day = Comparisons[it2].B.Day; //set source day as day
					}
				}
				if (Comparisons[it2].PM == Score) //check to see if perfect match is the same as current source; if so, proceed
				{
					if (bIsEarliest) //if bIsEarliest is set to true; shouldn't be
					{
						if (Comparisons[it2].B.Year > year || Comparisons[it2].B.Year == year && Comparisons[it2].B.Month > month || Comparisons[it2].B.Year == year && Comparisons[it2].B.Month == month && Comparisons[it2].B.Day > day) //if current test is earlier than previous best match, replace entries with new data
						{
							BestMatch = Comparisons[it2].B.UI; //set as best match for target
							Score = Comparisons[it2].PM; //set as new score
							bIsEarliest = true; //label target as earliest
							year = Comparisons[it2].B.Year;  //set source year as year
							month = Comparisons[it2].B.Month; //set source month as month
							day = Comparisons[it2].B.Day; //set source day as day
						}
					}
					else if (Comparisons[it2].B.Year > Comparisons[it2].A.Year || Comparisons[it2].B.Year == Comparisons[it2].A.Year && Comparisons[it2].B.Month > Comparisons[it2].A.Month || Comparisons[it2].B.Year == Comparisons[it2].A.Year && Comparisons[it2].B.Month == Comparisons[it2].A.Month && Comparisons[it2].B.Day > Comparisons[it2].A.Day)//if current test is earlier than previous best match, replace entries with new data
					{
						BestMatch = Comparisons[it2].B.UI; //unique id of source
						Score = Comparisons[it2].PM; //set score as current best score
						bIsEarliest = false; //allows it to keep searching for better matches
						year = Comparisons[it2].B.Year; // set source year as year
						month = Comparisons[it2].B.Month; //set source month as month
						day = Comparisons[it2].B.Day; //set source day as day
					}
				}
			}
		}
		if (BestMatch == "") //if there was no best match
		{
			bIsEarliest = true; //label target as earliest
		}
		if (bIsEarliest) //if target is labeled as earliest 
		{
			UniqueArticle[it].bIsDeadEnd = true; //label as DeadEnd
			UniqueArticle[it].TargetUID = BestMatch;
			UniqueArticle[it].bIsOriginal = bIsEarliest;
		}
		else //if target is not labeled as earliest
		{
			UniqueArticle[it].TargetUID = BestMatch;
			UniqueArticle[it].bIsOriginal = bIsEarliest;
		}
	}
}

void WriteOutputFile()
{
	string outputRM = "C:\\users\\username\\Desktop\\reprintmap.csv"; //set arbitrary output location
	cout << "Your current reprint map (output) file is: " << outputRM << endl;
	cout << "What is your preferred reprint map (output) file? ";
	getline(cin, outputRM); //set correct output location
	
	string data = "";

	ofstream file(outputRM); //print ancestor-descendent pairs into file
	for (unsigned int i = 0; i < size(UniqueArticle); ++i)
	{
		if (!UniqueArticle[i].bIsDeadEnd)
			data = data + UniqueArticle[i].UID + "," + UniqueArticle[i].TargetUID + "\n";
	}
	file << data;
	return;
}


void WriteRecieverFile()
{
	string outputED = "C:\\users\\username\\Desktop\\deadendlist.csv"; //set arbitrary output location
	cout << "Your current list of evolutionary dead-ends (output) file is: " << outputED << endl;
	cout << "What is your preferred list of evolutionary dead-ends (output) file? ";
	getline(cin, outputED); //set correct output location
	
	string data = "";

	ofstream file(outputED); //print list of evolutionary dead-ends
	for (unsigned int i = 0; i < size(UniqueArticle); ++i)
	{
		if (UniqueArticle[i].bIsDeadEnd)
			data = data + UniqueArticle[i].UID + "," + UniqueArticle[i].TargetUID + "\n";
	}
	file << data;
	return;
}

/*For Diagnostics Only
void WriteComparisonTable() 
{
	string data = "";
	ofstream file("C:\\users\\username\\Desktop\\RawComparisonTable.csv"); //print raw comparison table

	for (unsigned int i = 0; i < size(Comparisons); ++i)
	{
		data = data + Comparisons[i].A.UI + "," + Comparisons[i].B.UI + "\n";
	}
	file << data;
	return;
}*/

int main() //provide user feedback
{

	cout << "Generating database of useable article matches..." << endl;
	GenerateComparisonDatabase();
	cout << "Finished." << endl << endl;
	cout << "Processing selected article matches..." << endl;
	ProcessSelectedComparisons();
	cout << "Finished." << endl << endl;
	cout << "Writing Output File..." << endl;
	WriteOutputFile();
	cout << "Finished." << endl << endl;


	cout << "Writing Reciever File..." << endl;
	WriteRecieverFile();
	cout << "Finished." << endl << endl;
	
	// WriteComparisonTable() //for diagnostics only
	
	cout << "The Comparisons vector has " << size(Comparisons) << " elements." << endl << endl;
	cout << "The Unique article vector has " << size(UniqueArticle) << " elements." << endl << endl;
	cout << "We have processed " << NumLines << " lines of raw data from the input file." << endl << endl;


	system("pause");

	return 0;
}
