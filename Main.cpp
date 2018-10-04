//Distance Skeleton Algorithm (City-Block)
#include <iostream>
#include <fstream>
using namespace std;

class imageProcessing{
	public:
		
	int numRows, numCols, minVal, maxVal, newMinVal=0, newMaxVal=0;
	int** zeroFramedAry;
	int** skeletonAry;
	int neighborAry[5];
	
	//constructor
	imageProcessing(int numRows, int numCols, int minVal, int maxVal){
		
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
			
		zeroFramedAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++)
			zeroFramedAry[i] = new int[numCols+2];
			
		skeletonAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++)
			skeletonAry[i] = new int[numCols+2];
			
		for(int i=0;i<5;i++){
			neighborAry[i] = 0;
		}
	}
	
	//methods
	void zeroFrame(){
		
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				zeroFramedAry[i][j] = 0;
			}
		}
		
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				skeletonAry[i][j] = 0;
			}
		}
	}
		
	void loadImage(string input){
		
		ifstream in;
		in.open(input.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;
		for(int rows=0;rows<numRows;rows++) {
			for(int cols=0;cols<numCols;cols++) {
					in>>zeroFramedAry[rows+1][cols+1];
			}
		}
	}
	
	void loadNeighbors(int rows, int cols) {
		neighborAry[0] = zeroFramedAry[rows][cols];
		neighborAry[1] = zeroFramedAry[rows-1][cols]+1;
		neighborAry[2] = zeroFramedAry[rows][cols-1]+1;
		neighborAry[3] = zeroFramedAry[rows][cols+1]+1;
		neighborAry[4] = zeroFramedAry[rows+1][cols]+1;
	}
	
	void loadMaximaNeighbors(int rows, int cols){
		neighborAry[0] = zeroFramedAry[rows][cols];
		neighborAry[1] = zeroFramedAry[rows-1][cols];
		neighborAry[2] = zeroFramedAry[rows][cols-1];
		neighborAry[3] = zeroFramedAry[rows][cols+1];
		neighborAry[4] = zeroFramedAry[rows+1][cols];
	}
	
	void print() {
		for(int rows=0;rows<numRows+2;rows++) {
			for(int cols=0;cols<numCols+2;cols++) {
				cout<<zeroFramedAry[rows][cols]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	
	int min(int pass){
		int min = 0;
		if(pass==1){
			min = neighborAry[1];
			if(neighborAry[2]<min)
				min = neighborAry[2];
		}
		else if(pass==2) {
			min = neighborAry[0];
			for(int i=3;i<5;i++) {
				if(neighborAry[i]<min)
					min = neighborAry[i];
			}
		}
		return min;
	}
	
	void firstPass_4Distance(){
		for(int rows=0;rows<numRows;rows++) {
			for(int cols=0;cols<numCols;cols++) {
				if(zeroFramedAry[rows+1][cols+1]>0) {
					loadNeighbors(rows+1, cols+1);
					zeroFramedAry[rows+1][cols+1] = min(1);
				}
			}
		}
	}
	
	void secondPass_4Distance(){
		for(int rows=numRows;rows>0;rows--) {
			for(int cols=numCols;cols>0;cols--) {
				if(zeroFramedAry[rows+1][cols+1]>0) {
					loadNeighbors(rows+1, cols+1);
					zeroFramedAry[rows+1][cols+1] = min(2);
					if(zeroFramedAry[rows+1][cols+1]<newMinVal)
						newMinVal = zeroFramedAry[rows+1][cols+1];
					if(zeroFramedAry[rows+1][cols+1]>newMaxVal)
						newMaxVal = zeroFramedAry[rows+1][cols+1];
				}
			}
		}
	}
	
	int is_Maxima(int rows, int cols){
		loadMaximaNeighbors(rows, cols);
		if(zeroFramedAry[rows][cols]>=neighborAry[1])
			if(zeroFramedAry[rows][cols]>=neighborAry[2])
				if(zeroFramedAry[rows][cols]>=neighborAry[3])
					if(zeroFramedAry[rows][cols]>=neighborAry[4])
						return 1;
					else return 0;
				else return 0;
			else return 0;
		else return 0;
	}
	
	void compute_localMaxima(){
		for(int rows=0;rows<numRows+2;rows++) {
			for(int cols=0;cols<numCols+2;cols++) {
				if(zeroFramedAry[rows][cols]>0){
					if(is_Maxima(rows,cols)==1)
						skeletonAry[rows][cols] = 1;
					else 
						skeletonAry[rows][cols] = 0;
				}
			}
		}
	}
	
	void printImage(ofstream& file, int fileNum){
		file<<"newMinVal = "<<newMinVal<<endl;
		file<<"newMaxVal = "<<newMaxVal<<endl;
		if(fileNum==1){
			for(int rows=0;rows<numRows;rows++){
				for(int cols=0;cols<numCols;cols++){
					file<<zeroFramedAry[rows+1][cols+1]<<" ";
				}
				file<<endl;
			}
			file<<endl;
		}
		else if(fileNum==2){
			for(int rows=0;rows<numRows;rows++){
				for(int cols=0;cols<numCols;cols++){
					file<<skeletonAry[rows+1][cols+1]<<" ";
				}
				file<<endl;
			}
			file<<endl;
		}
	}
	
	void prettyPrintDistance(ofstream& file, int pass){
		file<<"Pass-"<<pass<<" Result:"<<endl;
		for(int rows=0;rows<numRows+2;rows++){
			for(int cols=0;cols<numCols+2;cols++){
				if(zeroFramedAry[rows][cols]>0)
					file<<zeroFramedAry[rows][cols]<<" ";
				else file<<"  ";
			}
			file<<endl;
		}
		file<<endl;
	}
	
	void prettyPrintSkeleton(ofstream& file){
		file<<"Skeleton Array:"<<endl;
		for(int rows=0;rows<numRows+2;rows++){
			for(int cols=0;cols<numCols+2;cols++){
				//file<<skeletonAry[rows][cols];
				
				if(skeletonAry[rows][cols]==0)
					file<<".";
				else file<<"9";
				
			}
			file<<endl;
		}
		file<<endl;
		
	}
};

int main(int argc, char* argv[]){
	ifstream input (argv[1]);
	string fileName = argv[1];
	ofstream outFile1 (argv[2]);
	ofstream outFile2 (argv[3]);
	ofstream outFile3 (argv[4]);
	int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;
	int** zeroFramedAry, skeletonAry;
	
	if(input.is_open()){
		input>>numRows>>numCols>>minVal>>maxVal;
		newMinVal = maxVal;
		newMaxVal = minVal;
		
		imageProcessing process(numRows, numCols, minVal, maxVal);
		process.zeroFrame();
		process.loadImage(fileName);
		process.firstPass_4Distance();
		process.prettyPrintDistance(outFile3,1);
		process.secondPass_4Distance();
		process.printImage(outFile1,1);
		process.prettyPrintDistance(outFile3,2);
		process.compute_localMaxima();
		process.printImage(outFile2,2);
		process.prettyPrintSkeleton(outFile3);	
		input.close();
		outFile1.close();
		outFile2.close();
		outFile3.close();
	}
	else cout<<"Couldn't retrieve data.";
	return 0;
}
