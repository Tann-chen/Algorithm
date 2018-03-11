#include <fstream>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;


vector<int> splitToDigits(int n);
int countDigits(int n);


int main(int argc, char const *argv[]) {
    // file exceptions
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

        // input
        int n;
        in >> n;
        int inputs[n];
        for (int i = 0; i < n; ++i) {
            in >> inputs[i];
        }

        int N;
        in >> N;

        //init
        int hasPlus[N]; // 1: has 0:no
        int T[N] ;
        vector<int> elements;


        for (int n = 0; n <= N; ++n) {
            vector<int> digits = splitToDigits(n);
            bool is_element = true;

            for (int d = 0; d < digits.size(); ++d) {  //check every digit
                bool is_exit = false;
                int sizeOfInput = sizeof(inputs) / sizeof(int);
                for (int input = 0; input < sizeOfInput; ++input) {
                    if (digits[d] == inputs[input]) {
                        is_exit = true;
                        break;
                    }
                }

                if (!is_exit) {   //if the digit not in input set
                    is_element = false;
                    break;
                }
            }

            // init works
            if (is_element) {
                if(n != 0){
                    T[n] = countDigits(n);
                    elements.push_back(n);
                    hasPlus[n] = 0;
                }else{
                    T[0] = 1;
                }
            } else {
                T[n] = (INT_MAX - 1) / 2;
                hasPlus[0] = 0;
            }
        }


        //dynamic programing
        for (int n = 1; n <= N; ++n) {
            int length = T[n];
            //find min
            for (int e = 0; e < elements.size(); ++e) {
                if (elements[e] < n) {

                    // use "*", but not "+" case
                    if (n % elements[e] == 0 && (hasPlus[n / elements[e]] == 0) && (T[n / elements[e]] + T[elements[e]] + 1 < length)) {
                        hasPlus[n] = 0;
                        length = T[n / elements[e]] + T[elements[e]] + 1;
                    }

                    // use "*", but if "+" is has existed, need "()"
                    if (n % elements[e] == 0 && (hasPlus[n / elements[e]] == 1) && (T[n / elements[e]] + T[elements[e]] + 3 < length)) {
                        hasPlus[n] = 0;
                        length = T[n / elements[e]] + T[elements[e]] + 3;
                    }

                    // use "+", if "+" big than "*", use "+",but need set flag
                    if (T[n - elements[e]] + T[elements[e]] + 1 < length) {     // use "+"
                        hasPlus[n] = 1;
                        length = T[n - elements[e]] + T[elements[e]] + 1;
                    }
                }
            }

            //set min to T[n]
            T[n] = length;
        }


        //return
        if (T[N] >= (INT_MAX - 1) / 2) {
            out << "N";
        } else {
            out << T[N];
        }
        if (s != sets - 1) {
            out << endl;
        }

    }

    return 0;
}


vector<int> splitToDigits(int n) {
    vector<int> digits;

    while (n >= 10) {
        int temp = n % 10;
        digits.push_back(temp);
        n /= 10;
    }
    digits.push_back(n);

    return digits;
}


int countDigits(int n) {
    int digits = 1;
    while (n >= 10) {
        digits++;
        n /= 10;
    }

    return digits;
}


