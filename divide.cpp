#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <math.h> 
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;


typedef struct cities{
	double x;
	double y;
	int index;
}city;


list<list<city>> dividedCities;
list<list<city>> processedDividedCities;

list<city> cities;

list<city> tempCity;
list<city> tour;

int lengthOfCities;





void createCities(string filename){
	
	bool coordSectionPassed = false;
	string line;

	city temp;
	ifstream file(filename);

	if (file.is_open()){
		while (getline(file, line)){
			
			if(line == "NODE_COORD_SECTION"){
				coordSectionPassed = true;
			}
			else if(line == "EOF" || line == " ")
				continue;
			else if(coordSectionPassed){
				sscanf(line.c_str(), "%d %lf %lf", &temp.index, &temp.x, &temp.y);
				
				lengthOfCities = temp.index;
				cities.push_back(temp);
				
			}
		}
		file.close();
	}
}

double getDistance(city location, city target){
	double distance;
	distance = pow(sqrt(abs(location.x-target.x))+sqrt(abs(location.y-target.y)),2);
	return distance;
}


void divideCities(list<city> cityList,int n){
	double minX = 999999999999.0;
	double maxX = 0.0;
	double middleX;
	double minY = 999999999999.0;
	double maxY = 0.0;
	double middleY;

	list<city> part1;
	list<city> part2;
	//if list size bigger than n, split the list
	if(cityList.size()>n){
		//take max x and min x in list
		std::list<city>::iterator itX = cityList.begin();
		for(int i = 0;i<cityList.size();i++){
			maxX = (*itX).x>maxX?(*itX).x:maxX;
			minX = (*itX).x<minX?(*itX).x:minX;
			std::advance(itX, 1);
		}
		//take max y and min y in list
		std::list<city>::iterator itY = cityList.begin();
		for(int i = 0;i<cityList.size();i++){
			maxY = (*itY).y>maxY?(*itY).y:maxY;
			minY = (*itY).y<minY?(*itY).y:minY;
			std::advance(itY, 1);
		}
		middleX = (minX+maxX)*.5;
		middleY = (minY+maxY)*.5;
		//compare x and y then split list to according to lower one
		std::list<city>::iterator it = cityList.begin();
		it = cityList.begin();
		if(middleX<middleY){
			for(int i = 0;i<cityList.size();i++){
				if((*it).x<middleX){
					part1.push_back(*it);
				}
				else{
					part2.push_back(*it);
				}
				std::advance(it,1);
			}
		}
		else{
			for(int i = 0;i<cityList.size();i++){
				if((*it).y<middleY){
					part1.push_back(*it);
				}
				else{
					part2.push_back(*it);
				}
				std::advance(it,1);
			}
		}
		//if list couldnt split ordinary
		std::list<city>::iterator it2 = cityList.begin();
		if(part1.size() == cityList.size()){
			part1.clear();
			part2.clear();

			for(int i = 0;i<cityList.size();i++){
				if(i<cityList.size()/2){
					part1.push_back(*it2);
				}
				else{
					part2.push_back(*it2);
				}
				std::advance(it2,1);
			}
		}
		else if(part2.size() == cityList.size()){
			part2.clear();
			part1.clear();
			for(int i = 0;i<cityList.size();i++){
				if(i<cityList.size()/2){
					part1.push_back(*it2);
				}
				else{
					part2.push_back(*it2);
				}
				std::advance(it2,1);
			}
		}
		//split splitted list
		divideCities(part1,n);
		divideCities(part2,n);
		
	}
	else{
		dividedCities.push_back(cityList); //<< if list length lower than n, append to divideCities list.
	}
}

int ii =0;
void exhaustiveAlg(list<city> cityList, int level, list<city>& v, double& minLength){
	double tempLength;
	std::list<city>::iterator iter3 = cityList.begin();
	std::list<city>::iterator iter4;
	//take ultimate result
    if(level == cityList.size()){

		double tempLength =0;
		for(int i =0;i<cityList.size();i++){
			iter4 = iter3;
			std::advance(iter4,1);
			tempLength += getDistance(*iter3,*iter4);
			std::advance(iter3,1);
		}

		if(tempLength<minLength){
			minLength = tempLength;
			v = cityList;
			ii++;
		}
    }
	
	//permutation
	std::list<city>::iterator iter1 = cityList.begin();
	std::list<city>::iterator iter2 = cityList.begin();
    for(int i=level;i<cityList.size();++i){
		iter2 = cityList.begin();
		std::advance(iter2, level);
        swap(iter2, iter1);
        exhaustiveAlg(cityList, level + 1, v, minLength);
        swap(iter2, iter1);
		std::advance(iter1, 1);
    }
}

void divideAndConquer(int index, int counter){
	divideCities(cities,9);
	printf("Cities are divided\n");
	
	
	//apply exhaustive algorithm for each divided city list
	std::list<list<city>>::iterator divCitIt = dividedCities.begin();
	for(int i=0;i<dividedCities.size();i++){
		double tempMinLength = 999999999;
		exhaustiveAlg(*divCitIt, 0, tempCity, tempMinLength);

		std::advance(divCitIt,1);
	}
	printf("Exhaustive Algorithm applied for each divided cities list\n");

	//merge
	std::list<list<city>>::iterator iIt2 = processedDividedCities.begin();
	for(int i = 0;i<processedDividedCities.size();i++){
		std::list<city>::iterator jIt2 = (*iIt2).begin();
		for(int j=0;j<(*iIt2).size();j++){
			tour.push_back(*jIt2);
			
			std::advance(jIt2, 1);
		}	
		std::advance(iIt2, 1);
	}
	printf("City lists merged");
}

int main(){
	int counter =0;
	double tourLength =0;

	createCities("ca4663.tsp");


	const clock_t start = clock();
	divideAndConquer(0, counter);
	const clock_t end = clock();
	const double duration = double(end - start) / CLOCKS_PER_SEC;
	printf("Time: %lf\n", duration);


	std::list<city>::iterator tourIt = tour.begin();
	std::list<city>::iterator tournextIt = tour.begin();
	
	for(int i=0;i<lengthOfCities;i++){
		
		std::advance(tournextIt,1);
		tourLength +=getDistance(*tourIt,*tournextIt);
		std::advance(tourIt,1);
	}
	printf("%lf", tourLength);
	ofstream tmpFile;
	tmpFile.open ("output");
	tourIt = tour.begin();
	for(int i=0;i<lengthOfCities;i++){
		tmpFile << (*tourIt).x << "\t"<< (*tourIt).y<< "\n";
		std::advance(tourIt,1);
	}
	tmpFile << "EOF\n";
	tmpFile.close();
	return 0;
}
