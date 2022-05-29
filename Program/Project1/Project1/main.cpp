#include <iostream> 
#include <fstream>
#include <vector> 
#include <string>
#include <cstring>
#include <cctype>
using namespace std;

// windows
//#define LINEAR_GRAPH_PATH	"GraphFiles\\ref10000_linear.gfa"
//#define TANGLE_GRAPH_PATH	"GraphFiles\\ref10000_tangle.gfa"
//#define	SNP_GRAPH_PATH		"GraphFiles\\ref10000_snp.gfa"
//#define ONECHAR_GRAPH_PATH	"GraphFiles\\ref10000_onechar.gfa"
//#define TWOPATH_GRAPH_PATH	"GraphFiles\\ref10000_twopath.gfa"

//mac/linux
#define LINEAR_GRAPH_PATH	"GraphFiles/ref10000_linear.gfa"
#define TANGLE_GRAPH_PATH	"GraphFiles/ref10000_tangle.gfa"
#define	SNP_GRAPH_PATH		"GraphFiles/ref10000_snp.gfa"
#define ONECHAR_GRAPH_PATH	"GraphFiles/ref10000_onechar.gfa"
#define TWOPATH_GRAPH_PATH	"GraphFiles/ref10000_twopath.gfa"
#define SMALL_EXAMPLE 		"GraphFiles/ref10000_small.gfa"		// small graph for testing 

#define NODE_CHAR 'S'
#define NODE_ID_LOCATION 2
#define NODE_VALUE_STARTING_LOCATION 4

#define EDGE_CHAR 'L'
#define EDGE_FIRST_NODE_STARTING_POSITION 2
#define EDGE_APPENDING_FIRST_STARTING_POSITION 4
#define EDGE_LAST_NODE_STARTING_POSITION 6
#define EDGE_APPENDING_LAST_STARTING_POSITION 8

struct Node
{
	int id;
	vector<int> nextNode;
	//vector<int> previousNode;
	bool nextElementComplement;
	string value;
};

struct NewNodes
{
	int id;
	string value;
};

struct Edge
{
	int previousNode;
	int nextNode;
};

vector<NewNodes> Nodes;
vector<Edge> Edges;

vector<Node> Graph;
//vector<char> LinearLine;

//void LinearGraphParsing() {
//	string line;
//	ifstream graphFile("GraphFiles\\ref10000_linear.gfa");
//	if (graphFile.is_open())
//	{
//		while (getline(graphFile, line)) {
//			for (int i = 0; i < line.size(); i++)
//			{
//				if (isalpha(line[i]))
//				{
//					Node newNode = 
//				}
//			}
//		}
//		graphFile.close();
//
//	}
//	else cout << "File oppening error";
//
//	for (int i = 0; i <= line.length(); i++)
//	{
//		cout << i + 1 << line[i] << "\n";
//	}
//}

// Backup code
//
//
//if (graphFile.is_open())
//{
//	while (getline(graphFile, tmp)) {
//		if (tmp[0] == NODE_CHAR)
//		{
//			Node newNode;
//			string id = "";
//			int i = NODE_ID_LOCATION;
//			while (isdigit(tmp[i]))
//			{
//				id += tmp[i];
//				i++;
//			}
//			newNode.id = stoi(id);
//			i = NODE_VALUE_STARTING_LOCATION + id.length() - 1;
//			id = "";
//			while (isalpha(tmp[i]))
//			{
//				id += tmp[i];
//				i++;
//			}
//			newNode.value = id;
//			Graph.push_back(newNode);
//		}
//		else if (tmp[0] == EDGE_CHAR)
//		{
//			string id = "";
//			int i = EDGE_FIRST_NODE_STARTING_POSITION;
//
//			while (isdigit(tmp[i]))
//			{
//				id += tmp[i];
//				i++;
//			}
//			int firstNode = stoi(id) - 1;
//			int lengthFirstElement = id.length();
//
//			//Ovdje treba provjeriti kaj se dogada, jer postoji nekad i prije M-a da je minus, kako onda ide komplement?
//			i = EDGE_APPENDING_FIRST_STARTING_POSITION + lengthFirstElement - 1;
//			i = EDGE_LAST_NODE_STARTING_POSITION + lengthFirstElement - 1;
//			id = "";
//			while (isdigit(tmp[i]))
//			{
//				id += tmp[i];
//				i++;
//			}
//			//cout << id<< "\n";
//			Graph[firstNode].nextNode.push_back(stoi(id));
//		}
//		tmp = "";
//	}
//	graphFile.close();
//	for (int i = 0; i < Graph.size(); i++)
//	{
//		cout << Graph[i].id << "\t" << Graph[i].value << "\n";
//		for (int j = 0; j < Graph[i].nextNode.size(); j++)
//			cout << Graph[i].nextNode[j] << "\n";
//		cout << "\n";
//	}
//}
//
//

void Search(vector<NewNodes> Nodes, vector<Edge> Edges, string pattern) {
	// init matrix C
	int C[(pattern.size())+1][(Nodes.size())+1];
	
	// fill in matrix C
	for (int i = 0; i < pattern.size()+1; i++) {
		C[i][0] = i;
		for (int j = 0; j < Nodes.size()+1; j++) {
			C[0][j] = j;
			
			if (i > 0 && j > 0) {
				string s(1, pattern[i]);
				if (s == Nodes[j].value) {
					C[i][j] = C[i-1][j-1];
				}
				else {
					C[i][j] = 1 + min(min(C[i-1][j], C[i][j-1]), C[i-1][j-1]);
				}
			}
		}
	}

	// print matrix C 
	for (int i = 0; i < pattern.size()+1; i++) {
		for (int j = 0; j < Nodes.size()+1; j++) {
			cout << C[i][j] << ' ';
		}
		cout << endl;
	}
}


void SearchTextPattern(string text, string pattern) {
	// init matrix C
	int C[(text.size())+1][(pattern.size())+1];

	// fill in matrix C
	for (int i = 0; i < text.size()+1; i++) {
		C[i][0] = i;
		for (int j = 0; j < pattern.size()+1; j++) {
			C[0][j] = j;
			
			if (i > 0 && j > 0) {
				if (text[i] == pattern[j]) {
					C[i][j] = C[i-1][j-1];
				}
				else {
					C[i][j] = 1 + min(min(C[i-1][j], C[i][j-1]), C[i-1][j-1]);
				}
			}
		}
	}

	// print matrix C 
	for (int i = 0; i < text.size()+1; i++) {
		for (int j = 0; j < pattern.size()+1; j++) {
			cout << C[i][j] << ' ';
		}
		cout << endl;
	}
}

int main() {
	string line;
	string tmp;
	ifstream graphFile(SMALL_EXAMPLE);
	if (graphFile.is_open())
	{
		while (getline(graphFile, tmp)) {
			if (tmp[0] == NODE_CHAR)
			{
				NewNodes newNode;
				string id = "";
				int i = NODE_ID_LOCATION;
				while (isdigit(tmp[i]))
				{
					id += tmp[i];
					i++;
				}
				newNode.id = stoi(id);
				i = NODE_VALUE_STARTING_LOCATION + id.length() - 1;
				id = "";
				while (isalpha(tmp[i]))
				{
					id += tmp[i];
					i++;
				}
				newNode.value = id;
				Nodes.push_back(newNode);
			}
			else if (tmp[0] == EDGE_CHAR)
			{
				string id = "";
				int i = EDGE_FIRST_NODE_STARTING_POSITION;
				Edge edge;

				while (isdigit(tmp[i]))
				{
					id += tmp[i];
					i++;
				}

				edge.previousNode = stoi(id) - 1;
				//int firstNode = stoi(id)-1;
				int lengthFirstElement = id.length();

				//Ovdje treba provjeriti kaj se dogada, jer postoji nekad i prije M-a da je minus, kako onda ide komplement?
				i = EDGE_APPENDING_FIRST_STARTING_POSITION + lengthFirstElement - 1;
				i = EDGE_LAST_NODE_STARTING_POSITION + lengthFirstElement - 1;
				id = "";
				while (isdigit(tmp[i]))
				{
					id += tmp[i];
					i++;
				}
				//cout << id<< "\n";
				edge.nextNode = stoi(id) - 1;
				Edges.push_back(edge);
				//Graph[firstNode].nextNode.push_back(stoi(id));
			}
			tmp = "";
		}
		graphFile.close();
		/*for (int i = 0; i < Graph.size(); i++)
		{
			cout << Graph[i].id << "\t" << Graph[i].value << "\n";
			for (int j = 0; j < Graph[i].nextNode.size(); j++)
				cout << Graph[i].nextNode[j] << "\n";
			cout << "\n";
		}*/
		//for (int i = 0; i < Nodes.size(); i++) {
		//	cout << Nodes[i].id << ' ' << Nodes[i].value << endl;
		//}
		//for (int i = 0; i < Edges.size(); i++) {
		//	cout << Edges[i].previousNode << ' ' << Edges[i].nextNode << endl;
		//}

		//SearchTextPattern("this", "there");
		Search(Nodes, Edges, "TTTT");
		
	}
	else cout << "File oppening error";

	//for (int i = 0; i <= line.length(); i++)
	//{
	//	cout << i + 1 << line[i] << "\n";
	//}
	return 0;
}
