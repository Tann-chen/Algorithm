#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


int castSet2Int(vector<int> set);
int numOfSet(vector<int> set);
vector<int> arrClone(vector<int> src);
int top2DownDp(vector<vector<int> > matrix, vector<vector<int> > min, vector<int> set, int endPoint);


int main(int argc, char const *argv[]) {
    ifstream in(argv[1]);

    if (!in) {
        cout << "Cannot open  input file;" << endl;
        return -1;
    }

    ofstream out(argv[2]);

    if (!out) {
        cout << "Cannot open  output file;" << endl;
        return -1;
    }

    int sets;
    in >> sets;

    for (int s = 0; s < sets; ++s) {
        int V, E;
        in >> V;
        in >> E;

        vector<vector<int> > adjacentMatrix(V);

        for (int i = 0; i < adjacentMatrix.size(); ++i) {
            adjacentMatrix[i].resize(V);
        }

        for (int e = 0; e < E; ++e) {
            int e1, e2, w;
            in >> e1;
            in >> e2;
            in >> w;
            // not directional
            adjacentMatrix[e1][e2] = w;
            adjacentMatrix[e2][e1] = w;
        }


        // define a buffer to record the dp data
        // first node (source) does not take into account
        int maxInt = (int) pow(2, V - 1);

        vector<vector<int> > Min(maxInt);

        for (int i = 0; i < maxInt; ++i) {
            Min[i].resize(V);

            for (int j = 0; j < V; ++j) {
                Min[i][j] = INT_MAX;
            }
        }


        //initially, every vertex(excludes source) is in set
        vector<int> vertexSet(V - 1);
        for (int i = 0; i < V - 1; ++i) {
            vertexSet[i] = 1;   //1 means in set, 0 means not in set
        }

        int result = top2DownDp(adjacentMatrix, Min, vertexSet, 0);


        //output
        out << result;
        if (s != sets - 1) {
            out << endl;
        }
    }

    return 0;

}


int top2DownDp(vector<vector<int> > matrix, vector<vector<int> > min, vector<int> set, int endPoint) {
    int match = castSet2Int(set);

    // dp: if result is in buffer, just use
    if (min[match][endPoint] < INT_MAX) {
        return min[match][endPoint];
    }
    // base case
    if (numOfSet(set) == 1) {
        int onlyVertex = 0;
        for (int i = 0; i < set.size(); ++i) {
            if (set[i] == 1) {
                onlyVertex = i + 1;   // 0 is not in set, vertex = index+1
                break;
            }
        }

        int value = matrix[0][onlyVertex] + matrix[onlyVertex][endPoint];
        //store the value
        min[match][endPoint] = value;
        return value;
    }
    else {
        vector<int> allVertexInSet;
        for (int i = 0; i < set.size(); ++i) {
            if (set[i] == 1) {
                allVertexInSet.push_back(i + 1);    // 0 is not is set, vertex = index+1
            }
        }

        //get the min, use recursion
        int minValue = INT_MAX;
        for (int i = 0; i < numOfSet(set); ++i) {
            int rmVertex = allVertexInSet[i];
            vector<int> tempSet = arrClone(set);
            tempSet[rmVertex - 1] = 0;

            int tempValue = matrix[rmVertex][endPoint] + top2DownDp(matrix, min, tempSet, rmVertex);
            if (tempValue < minValue) {
                minValue = tempValue;
            }
        }

        //store the value
        min[match][endPoint] = minValue;
        return minValue;
    }
}


int castSet2Int(vector<int> set) {
    int value = 0;
    long setLen = set.size();
    for (long i = setLen - 1; i >= 0; --i) {
        if (set[i] == 1) {
            value += pow(2, setLen - 1 - i);
        }
    }

    return value;
}


int numOfSet(vector<int> set) {
    long setLen = set.size();
    int counter = 0;
    for (int i = 0; i < setLen; ++i) {
        if (set[i] == 1) {
            counter++;
        }
    }

    return counter;
}


vector<int> arrClone(vector<int> src) {
    vector<int> tar(src.size());
    for (int i = 0; i < src.size(); ++i) {
        tar[i] = src[i];
    }

    return tar;
}

