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
#define SMALL_EXAMPLE_TWOPATH "GraphFiles/ref10000_small_twopath.gfa"

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
	string value;
};

struct Edge
{
	int previousNode; // Not the real id of node, but in the vector its position
	int nextNode;
};

vector<Node> Nodes;
vector<Edge> Edges;

// Helping vectors for multichar nodes
vector<Node> TmpNodes;
vector<Edge> TmpEdges; 


// Method for parsing a linear graph to onechar graph
void LinearGraphParsing() {
	string line;
	ifstream graphFile(SMALL_EXAMPLE);
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

// Works for OneChar and TwoPath and Snp and Tngle 
void GraphParsing(string path) {

	string line;
	ifstream graphfile(path);

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
				i = NODE_VALUE_STARTING_LOCATION + id.length() - 1;
				id = "";
				while (isalpha(line[i]))
				{
					id += line[i];
					i++;
				}
				node.value = id;
				Nodes.push_back(node);
			}
			else if (line[0] == EDGE_CHAR)
			{
				Edge edge;
				int i = EDGE_FIRST_NODE_STARTING_POSITION;
				string id = "";

				while (isdigit(line[i]))
				{
					id += line[i];
					i++;
				}
				if(path == TANGLE_GRAPH_PATH) edge.previousNode = stoi(id);
				else edge.previousNode = stoi(id) - 1;
				int lengthFirstElement = id.length();

				i = EDGE_LAST_NODE_STARTING_POSITION + lengthFirstElement - 1;
				id = "";
				while (isdigit(line[i]))
				{
					id += line[i];
					i++;
				}

				if(path == TANGLE_GRAPH_PATH) edge.nextNode = stoi(id);
				else edge.nextNode = stoi(id) - 1;
				Edges.push_back(edge);
			}
		}
	}

	for (int i = 0; i < Edges.size(); i++)
	{
		cout << Edges[i].previousNode + 1 << " -> " <<  Edges[i].nextNode + 1<< "\n";
		cout << Nodes[Edges[i].previousNode].value << " -> " << Nodes[Edges[i].nextNode].value << "\t" << Nodes[Edges[i].previousNode].id << " " << Nodes[Edges[i].nextNode].id << "\n";
	}

}

int** Propagate(int previousNode, int nextNode, int i, int** C) {
	if (C[i][nextNode] > 1 + C[i][previousNode]) {
		C[i][nextNode] = 1 + C[i][previousNode];

		for (int j = nextNode; j < Edges.size(); j++) {
			Propagate(Edges[j].previousNode, Edges[j].nextNode, i, C);
		}
	}

	return C;
}

void Search(vector<Node> Nodes, vector<Edge> Edges, string pattern) {
	// init matrix C
	int** C = (int**)malloc((pattern.size() + 1) * sizeof(int * ));
	for (int i = 0; i < pattern.size() + 1; i++) C[i] = (int*)malloc((Nodes.size() + 1) * sizeof(int));

	// fill in matrix C
	for (int i = 0; i < pattern.size()+1; i++) {
		C[i][0] = i;
		for (int j = 0; j < Nodes.size()+1; j++) {
			C[0][j] = j;
			
			if (i > 0 && j > 0) {
				string s(1, pattern[i]);
				if (s == Nodes[j-1].value) {
					C[i][j] = C[i-1][j-1];
				}
				else {
					C[i][j] = 1 + min(min(C[i-1][j], C[i][j-1]), C[i-1][j-1]);
				}
			}
		}

		if (i > 0) {
			for (int j = 0; j < Edges.size(); j++) {
				C = Propagate(Edges[j].previousNode, Edges[j].nextNode, i, C);
			}
		}
	}

	ofstream MyFile("test.txt");

	// print matrix C 
	for (int i = 0; i < pattern.size()+1; i++) {
		for (int j = 0; j < Nodes.size()+1; j++) {
			//cout << C[i][j] << ' ';
			MyFile << C[i][j] << ' ';
		}
		//cout << endl;
		MyFile << endl;
	}

	MyFile.close();
	free(C);
}

void SearchTextPattern(string text, string pattern) {
	// init matrix C
	int** C = (int**)malloc((text.size() + 1) * sizeof(int*));
	for (int i = 0; i < text.size() + 1; i++) C[i] = (int*)malloc((pattern.size() + 1) * sizeof(int));

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

	free(C);
}

// Method used to generate multiple nodes and edges from one node for lates analisys
void GenerateOneChar(Node node)
{
	for (int i = 0; i < node.value.length(); i++)
	{
		Node tmpNode;
		tmpNode.id = i + 1;
		tmpNode.value = node.value[i];

		Edge tmpEdge;
		tmpEdge.previousNode = i;
		tmpEdge.nextNode = i + 1;

		TmpNodes.push_back(tmpNode);
		TmpEdges.push_back(tmpEdge);
	}
	TmpEdges.pop_back(); // Remove an edge that is a dead end

	for (int i = 0; i < TmpEdges.size(); i++)
	{
		cout << TmpEdges[i].previousNode + 1 << " -> " << TmpEdges[i].nextNode + 1 << "\n";
		cout << TmpNodes[TmpEdges[i].previousNode].value << " -> " << TmpNodes[TmpEdges[i].nextNode].value << "\t" << TmpNodes[TmpEdges[i].previousNode].id << " " << TmpNodes[TmpEdges[i].nextNode].id << "\n";
	}

}

int main() {

	Node n;
	n.id = 12;
	n.value = "TTACGTTTTT";
	GenerateOneChar(n);
	//LinearGraphParsing();
	//GraphParsing(SMALL_EXAMPLE_TWOPATH);
	Search(Nodes, Edges, "TTTT");
	return 0;
}