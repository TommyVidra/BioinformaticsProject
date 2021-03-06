#include <iostream> 
#include <fstream>
#include <vector> 
#include <string>
#include <cstring>
#include <cctype>
#include <sstream>
#include <chrono>
#include <map>
#include <time.h>

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
#define LINEAR_GRAPH_PATH		"GraphFiles/ref10000_linear.gfa"
#define TANGLE_GRAPH_PATH		"GraphFiles/ref10000_tangle.gfa"
#define	SNP_GRAPH_PATH			"GraphFiles/ref10000_snp.gfa"
#define ONECHAR_GRAPH_PATH		"GraphFiles/ref10000_onechar.gfa"
#define TWOPATH_GRAPH_PATH		"GraphFiles/ref10000_twopath.gfa"
#define SMALL_EXAMPLE_LINEAR 	"GraphFiles/ref10000_small_linear.gfa"		
#define SMALL_EXAMPLE_TWOPATH 	"GraphFiles/ref10000_small_twopath.gfa"
#define SMALL_EXAMPLE_SNP 		"GraphFiles/ref10000_small_snp.gfa"

#define NODE_CHAR 'S'
#define NODE_ID_LOCATION 2
#define NODE_VALUE_STARTING_LOCATION 4

#define EDGE_CHAR 'L'
#define EDGE_FIRST_NODE_STARTING_POSITION 2
#define EDGE_APPENDING_FIRST_STARTING_POSITION 4
#define EDGE_LAST_NODE_STARTING_POSITION 6
#define EDGE_APPENDING_LAST_STARTING_POSITION 8

#define FIRST_TRHESHOLD 100
#define LAST_TRHESHOLD 100000

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

map<int, int> DictForInDegreeCount;

vector<string> patterns;



// *** HELPING FUNCTIONS *** 
// Method used for splitting strings (written by Nina Anić)
vector<string> splitStr(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

// Method used for generating test patterns (written by Tomislav Vidović)
vector<string> getPatterns() {
	vector<string> patterns;
	int steps = 11
    for (int k = 1; k < steps; k++)
    {
        int threshold = rand() % LAST_TRHESHOLD + FIRST_TRHESHOLD;
        string pattern = "";

        for (int i = 0; i < threshold; i++)
        {
            int j = rand() % 5 + 1;
            switch (j)
            {
            case 1:
                pattern += "T";
                break;
            case 2:
                pattern += "A";
                break;
            case 3:
                pattern += "C";
                break;
            case 4:
                pattern += "G";
                break;
            }
        }
        patterns.push_back(pattern);
    }
	return patterns;
}

// Methos used for printing results part 1 (written by Nina Anić)
void firstPrint(vector<Node> Nodes, vector<Edge> Edges, ofstream& MyFile, vector<Node> TmpNodes, string name) {
	MyFile << "load graph" << endl;
	MyFile << Nodes.size() << " original nodes" << endl;
	MyFile << Nodes.size() << " split nodes" << endl;
	MyFile << TmpNodes.size() << "bp" << endl;
	MyFile << Edges.size() << " edges" << endl;

	for (int i=0; i<Nodes.size(); i++) {
		DictForInDegreeCount[Nodes[i].id] = 0;
	}
	for (int i=0; i<Nodes.size(); i++) {
		for (int j=0; j<Edges.size(); j++) {
			if (Edges[j].nextNode == Nodes[i].id) {
				DictForInDegreeCount[Nodes[i].id]++;
			}
		}
	}
	cout << endl;
	map<int, int>::iterator itr;
	int counter = 0;
	for (itr = DictForInDegreeCount.begin(); itr != DictForInDegreeCount.end(); ++itr) {
    	//cout << '\t' << itr->first << '\t' << itr->second << '\n';
		if (itr->second >= 2) counter++;
    }
	MyFile << counter << " nodes with in-degree >= 2" << endl;

	MyFile << "load reads" << endl;
	MyFile << "preprocess graph" << endl;

	if (name == "linear" || name == "onechar") {
		MyFile << "The graph is linear / a tree / a forest" << endl;
	}
	else if (name == "twopath" || name == "snp") {
		MyFile << "The graph is a DAG" << endl;
	}
	else {
		MyFile << "The graph is cyclic" << endl;
	}

	MyFile << "Collapsed nodes: " << Nodes.size() << endl;
	MyFile << "Collapsed edges: " << Edges.size() << endl;
}

// Methos used for printing results part 2 (written by Nina Anić)
void secondPrint(vector<Node> Nodes, vector<Edge> Edges, ofstream& MyFile) {
	MyFile << "Nodes: " << Nodes.size() << endl;
	MyFile << "Edges: " << Edges.size() << endl;
	MyFile << "BPs: " << Nodes.size() << endl;
	//MyFile << "start algorithm for approximate pattern matching on hypertext" << endl;
	//MyFile << "algorithm took " << duration << "us for pattern size of " << patternSize << " characters" << endl;
	//MyFile << "############" << endl;
}

// Methos used for printing results part 3 (written by Nina Anić)
void thirdPrint(ofstream& MyFile, float duration, int patternSize) {
	MyFile << endl;
	MyFile << "start algorithm for approximate pattern matching on hypertext" << endl;
	MyFile << "algorithm took " << duration << "us for pattern size of " << patternSize << " characters" << endl;
}



// *** GRAPH PARSING ***
// Method for parsing a linear graph to onechar graph (written by Tomislav Vidović)
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

// Method for parsing a OneChar, TwoPath, Snp and Tngle graph (written by Tomislav Vidović)
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

// Method used to generate multiple nodes and edges from one node for lates analisys (written by Tomislav Vidović)
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

	//for (int i = 0; i < TmpEdges.size(); i++)
	//{
	//	cout << TmpEdges[i].previousNode + 1 << " -> " << TmpEdges[i].nextNode + 1 << "\n";
	//	cout << TmpNodes[TmpEdges[i].previousNode].value << " -> " << TmpNodes[TmpEdges[i].nextNode].value << "\t" << TmpNodes[TmpEdges[i].previousNode].id << " " << TmpNodes[TmpEdges[i].nextNode].id << "\n";
	//}

}



// *** ALGORITHM for approximate pattern matching on hypertext ***
// Implemented method from the work of Gonzalo Navarro (https://www.sciencedirect.com/science/article/pii/S0304397599003333) for approximate pattern matching on hypertext
// (written by Nina Anić)
int** Propagate(int previousNode, int nextNode, int i, int** C) {
	if (C[i][nextNode] > 1 + C[i][previousNode]) {
		C[i][nextNode] = 1 + C[i][previousNode];

		for (int j = nextNode; j < Edges.size(); j++) {
			Propagate(Edges[j].previousNode, Edges[j].nextNode, i, C);
		}
	}

	return C;
}

// Implemented method from the work of Gonzalo Navarro (https://www.sciencedirect.com/science/article/pii/S0304397599003333) for approximate pattern matching on hypertext
// (written by Nina Anić)
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
				string s(1, pattern[i-1]);
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

	//ofstream MyFile("test.txt");
//
	//// print matrix C 
	//for (int i = 0; i < pattern.size()+1; i++) {
	//	for (int j = 0; j < Nodes.size()+1; j++) {
	//		//cout << C[i][j] << " ";
	//		MyFile << C[i][j] << " ";
	//	}
	//	//cout << endl;
	//	MyFile << endl;
	//}
//
	//MyFile.close();
	free(C);
}


// *** MAIN FUNCTION *** 
// (written by Nina Anić and Tomislav Vidović)
int main() {	
	// Helping vectors for different graph types 
	vector<string> onechar_graphs;
	onechar_graphs.push_back(LINEAR_GRAPH_PATH);
   	onechar_graphs.push_back(ONECHAR_GRAPH_PATH);
   	onechar_graphs.push_back(TWOPATH_GRAPH_PATH);
   	onechar_graphs.push_back(SMALL_EXAMPLE_LINEAR);
   	onechar_graphs.push_back(SMALL_EXAMPLE_TWOPATH);
	vector<string> multichar_graphs;
   	multichar_graphs.push_back(TANGLE_GRAPH_PATH);
   	multichar_graphs.push_back(SNP_GRAPH_PATH);
   	multichar_graphs.push_back(SMALL_EXAMPLE_SNP);

	string path = LINEAR_GRAPH_PATH;	// Path of graph which we want to test  

	// Creating new txt file for every graph to store results 
	vector<string> v1 = splitStr(path, "/");
	vector<string> v2 = splitStr(v1[1], ".");
	vector<string> v3 = splitStr(v2[0], "_"); // ne radi za small examples 
	ofstream MyFile("Results/" + v2[0] + ".txt");

	bool found = (find(onechar_graphs.begin(), onechar_graphs.end(), path) != onechar_graphs.end()); // Determine which graph type is used for testing (onechar or multichar)
	clock_t start, end;
	double cpu_time_used;

	patterns = getPatterns();

	if (found == 1) {
		if (path == LINEAR_GRAPH_PATH || path == SMALL_EXAMPLE_LINEAR) {
			LinearGraphParsing();	// Parse graph 
			firstPrint(Nodes, Edges, MyFile, Nodes, v3[1]); // Print first part of results
		} 
		else {
			GraphParsing(path); 	// Parse graph 
			firstPrint(Nodes, Edges, MyFile, Nodes, v3[1]); // Print first part of results 
		}
		secondPrint(Nodes, Edges, MyFile); 		// Print second part of results

		for (int i=0; i<patterns.size(); i++) {
			// count CPU time; clock() --> On Windows it basically runs of the wall clock, while on e.g. Linux it's the process CPU time.
			start = clock();										// Start counting CPU time required for execution of implemented algorithm
			Search(Nodes, Edges, patterns[i]);							// Execute algorithm described in https://www.sciencedirect.com/science/article/pii/S0304397599003333
			end = clock();											// Stop counting CPU time required for execution of implemented algorithm
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // Total CPU time required for execution of implemented algorithm
			thirdPrint(MyFile, cpu_time_used * 1000000, patterns[i].size());
		}
	}
	else {
		GraphParsing(path);

		for (int i=0; i<Nodes.size(); i++) {
			GenerateOneChar(Nodes[i]);
		}
		secondPrint(TmpNodes, TmpEdges, MyFile); 		// Print second part of results

		for (int i=0; i<patterns.size(); i++) {
			// count CPU time; clock() --> On Windows it basically runs of the wall clock, while on e.g. Linux it's the process CPU time.
			start = clock();										// Start counting CPU time required for execution of implemented algorithm
			Search(TmpNodes, TmpEdges, patterns[i]);							// Execute algorithm described in https://www.sciencedirect.com/science/article/pii/S0304397599003333
			end = clock();											// Stop counting CPU time required for execution of implemented algorithm
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // Total CPU time required for execution of implemented algorithm
			thirdPrint(MyFile, cpu_time_used * 1000000, patterns[i].size());
		}
	}

	MyFile.close();
	return 0;
}