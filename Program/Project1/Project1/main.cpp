#include <iostream> 
#include <fstream>
#include <vector> 
#include <string>
#include <cstring>
#include <cctype>
using namespace std;

#define FILE_OPENING_ERROR "File oppening error"

// windows
//#define LINEAR_GRAPH_PATH	"GraphFiles\\ref10000_linear.gfa"
//#define TANGLE_GRAPH_PATH	"GraphFiles\\ref10000_tangle.gfa"
//#define	SNP_GRAPH_PATH		"GraphFiles\\ref10000_snp.gfa"
//#define ONECHAR_GRAPH_PATH	"GraphFiles\\ref10000_onechar.gfa"
//#define TWOPATH_GRAPH_PATH	"GraphFiles\\ref10000_twopath.gfa"
//#define SMALL_EXAMPLE 		"GraphFiles/ref10000_small.gfa"	

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

//struct Node
//{
//	int id;
//	vector<int> nextNode;
//	//vector<int> previousNode;
//	bool nextElementComplement;
//	string value;
//};

//vector<Node> Graph;
//vector<char> LinearLine;

struct Node
{
	int id;
	string value;
};

struct Edge
{
	int previousNode; // Not the real id of node, but in the vector its position
	int nextNode;
};

vector<Node> Nodes;
vector<Edge> Edges;


// Method for parsing a linear graph to onechar graph
void LinearGraphParsing() {
	string line;
	ifstream graphFile(LINEAR_GRAPH_PATH);
	if (graphFile.is_open())
	{
		int id = 1;
		while (getline(graphFile, line)) {
			for (int i = 1; i < line.size(); i++)
			{
				if (isalpha(line[i]))
				{
					Node node;
					Edge edge;

					node.id = id;
					node.value = line[i];
					Nodes.push_back(node);

					edge.previousNode = id - 1;
					edge.nextNode = id;
					Edges.push_back(edge);
					id++;
				}
			}
		}
		graphFile.close();
		Edges.pop_back(); // Last edge is a dead end
	}
	else cout << FILE_OPENING_ERROR;

	/*for (int i = 9000; i < Edges.size(); i++)
	{
		cout << Edges[i].previousNode + 1 << " -> " <<  Edges[i].nextNode + 1<< "\n";
		cout << Nodes[Edges[i].previousNode].value << " -> " << Nodes[Edges[i].nextNode].value << "\t" << Nodes[Edges[i].previousNode].id << " " << Nodes[Edges[i].nextNode].id << "\n";
	}*/
}


void OneCharGraphParsing() {

	string line;
	ifstream graphfile(ONECHAR_GRAPH_PATH);

	if (graphfile.is_open())
	{
		while (getline(graphfile, line)) {
			if (line[0] == NODE_CHAR)
			{
				Node node;
				string id = "";
				int i = NODE_ID_LOCATION;
				while (isdigit(line[i]))
				{
					id += line[i];
					i++;
				}
				node.id = stoi(id);
				i = NODE_VALUE_STARTING_LOCATION - id.length() - 1;
				id = "";
				while (isalpha(line[i]))
				{
					id += line[i];
					i++;
				}
				//....
			}
		}
	}
}

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

void Search(vector<Node> Nodes, vector<Edge> Edges, string pattern) {
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
				Node newNode;
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

//int main() {
	/*string line;
	string tmp;*/

	//LinearGraphParsing();

	//ifstream graphFile(SNP_GRAPH_PATH);
	//if (graphFile.is_open())
	//{
	//	while (getline(graphFile, tmp)) {
	//		if (tmp[0] == NODE_CHAR)
	//		{
	//			Nodes newNode;
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
	//			Nodes.push_back(newNode);
	//		}
	//		else if (tmp[0] == EDGE_CHAR)
	//		{
	//			string id = "";
	//			int i = EDGE_FIRST_NODE_STARTING_POSITION;
	//			Edge edge;

	//			while (isdigit(tmp[i]))
	//			{
	//				id += tmp[i];
	//				i++;
	//			}

	//			edge.previousNode = stoi(id) - 1;
	//			//int firstNode = stoi(id)-1;
	//			int lengthFirstElement = id.length();

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
	//			edge.nextNode = stoi(id) - 1;
	//			Edges.push_back(edge);
	//			//Graph[firstNode].nextNode.push_back(stoi(id));
	//		}
	//		tmp = "";
	//	}
	//	graphFile.close();
	//	/*for (int i = 0; i < Graph.size(); i++)
	//	{
	//		cout << Graph[i].id << "\t" << Graph[i].value << "\n";
	//		for (int j = 0; j < Graph[i].nextNode.size(); j++)
	//			cout << Graph[i].nextNode[j] << "\n";
	//		cout << "\n";
	//	}*/
	//}
	//else cout << "File oppening error";

	//for (int i = 0; i <= line.length(); i++)
	//{
	//	cout << i + 1 << line[i] << "\n";
	//}
	//return 0;
//}
