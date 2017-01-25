#ifndef Included_ReadFile_H
#define Included_ReadFile_H

#include <iostream>
#include <fstream>
#include "List.h"
#include "dataTypes.h"

int readFile(std::ifstream& inFile, std::string& type, List<Vector*>* myList1, List<Hamming*>* myList2, List<MatrixPoint*>* myList3);
int readFileConfig(std::ifstream& inFile, int& noClusters, int& noHashFunctions, int& noHashTables, int& claransSample, int& claransIterations);
bool readVectorFile(std::ifstream& inFilem, std::string& line, List<Vector*>* myList);
bool readHammingFile(std::ifstream& inFile, List<Hamming*>* myList);
bool readMatrixFile(std::ifstream& inFile, List<MatrixPoint*>* myList);

bool readMatrixQuery(std::ifstream& inFile, List<MatrixPoint*>* queryList);
bool readRadius(std::ifstream& inFile, double& radius);

#endif
