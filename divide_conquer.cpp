#include <fstream>
#include <iostream>
#include <vector>
#include "simple_matrix.hpp"

using namespace std;

SMatrix<float> strassen(SMatrix<float> A, SMatrix<float> B);
SMatrix<float> sumOfSMatrix(const SMatrix<float> s1, const SMatrix<float> s2);
SMatrix<float> diffOfSMatrix(const SMatrix<float> s1, const SMatrix<float> s2);



int main(int argc, char const *argv[]) {

        // file exceptions
        ifstream in(argv[1]);

        if(!in) {
                cout << "Cannot open  input file;" << endl;
                return -1;
        }

        ofstream out(argv[2]);

        if(!out) {
                cout << "Cannot open  output file;" << endl;
                return -1;
        }

        int sets;
        in >> sets;

        for(int i=0; i<sets; ++i) {
                //input
                int n;
                in >> n;
                SMatrix<float> A(n), B(n), C(n);

                for(int r=0; r<n; ++r) {
                        for(int c=0; c<n; ++c) {
                                in >> A(r,c);
                        }
                }

                for(int r=0; r<n; ++r) {
                        for(int c=0; c<n; ++c) {
                                in >> B(r,c);
                        }
                }

                //calculate
                C = strassen(A,B);

                //output
                for(int r=0; r<n; ++r) {
                        for(int c=0; c<n; ++c) {
                                out << C(r,c) << " ";
                        }
                }

                out << endl;

        }

        return 0;
}




SMatrix<float> strassen(SMatrix<float> A, SMatrix<float> B){
        //return obj
        SMatrix<float> C(A.size());

        // base case
        if(A.size() == 1) {
                C(0,0) = A(0,0)*B(0,0);
        }
        else{

                const int n = A.size()/2;
                SMatrix<float> aTemp[2][2];

                //divide A
                for(int i=0; i<2; ++i) {
                        for(int j=0; j<2; ++j) {
                                aTemp[i][j].resize(n);
                                //copy
                                for(int r=0; r<n; ++r) {
                                        for(int c=0; c<n; ++c) {
                                                aTemp[i][j](r,c) = A(r+i*n,c+j*n);
                                        }
                                }
                        }
                }

                SMatrix<float> bTemp[2][2];
                //divide B
                for(int i=0; i<2; ++i) {
                        for(int j=0; j<2; ++j) {
                                bTemp[i][j].resize(n);
                                //copy
                                for(int r=0; r<n; ++r) {
                                        for(int c=0; c<n; ++c) {
                                                bTemp[i][j](r,c) = B(r+i*n,c+j*n);
                                        }
                                }
                        }
                }

                //get S
                SMatrix<float> S[10];
                for(int i=0; i<10; ++i) {
                        S[i].resize(n);
                }

                S[0] = diffOfSMatrix(bTemp[0][1], bTemp[1][1]); //B12-B22
                S[1] = sumOfSMatrix(aTemp[0][0], aTemp[0][1]);  //A11+A12
                S[2] = sumOfSMatrix(aTemp[1][0], aTemp[1][1]);  //A21+A22
                S[3] = diffOfSMatrix(bTemp[1][0], bTemp[0][0]); //B21-B11
                S[4] = sumOfSMatrix(aTemp[0][0], aTemp[1][1]);  //A11+A22
                S[5] = sumOfSMatrix(bTemp[0][0], bTemp[1][1]); //B11+B22
                S[6] = diffOfSMatrix(aTemp[0][1], aTemp[1][1]);  //A12-A22
                S[7] = sumOfSMatrix(bTemp[1][0], bTemp[1][1]); //B21+B22
                S[8] = diffOfSMatrix(aTemp[0][0], aTemp[1][0]); //A11-A21
                S[9] = sumOfSMatrix(bTemp[0][0], bTemp[0][1]);  //B11+B12

                //get P (recusive)
                SMatrix<float> P[7];
                for(int i=0; i<7; ++i) {
                        P[i].resize(n);
                }

                P[0] = strassen(aTemp[0][0], S[0]); //A11*S1
                P[1] = strassen(S[1], bTemp[1][1]); //S2*B22
                P[2] = strassen(S[2], bTemp[0][0]);   //S3*B11
                P[3] = strassen(aTemp[1][1], S[3]); //A22*S4
                P[4] = strassen(S[4], S[5]); //S5*S6
                P[5] = strassen(S[6], S[7]); //S7*S8
                P[6] = strassen(S[8], S[9]); //S9*S10

                //merge
                SMatrix<float> cTemp[2][2];
                for(int i=0; i<2; ++i) {
                        for(int j=0; j<2; ++j) {
                                cTemp[i][j].resize(n);
                        }
                }

                cTemp[0][0] = sumOfSMatrix(diffOfSMatrix(sumOfSMatrix(P[4], P[3]), P[1]),P[5]);
                cTemp[0][1] = sumOfSMatrix(P[0],P[1]);
                cTemp[1][0] = sumOfSMatrix(P[2],P[3]);
                cTemp[1][1] = diffOfSMatrix(diffOfSMatrix(sumOfSMatrix(P[4],P[0]),P[2]),P[6]);

                //return
                for(int i=0; i<2; ++i) {
                        for(int j=0; j<2; ++j) {

                                SMatrix<float> temp = cTemp[i][j];

                                for(int r=0; r<n; ++r) {
                                        for(int c=0; c<n; ++c) {
                                                C(i*n+r,j*n+c) = temp(r,c);
                                        }
                                }

                        }
                }

        }

        return C;
}


SMatrix<float> sumOfSMatrix(SMatrix<float> s1, SMatrix<float> s2){
        int n = s1.size();
        SMatrix<float> temp(n);

        for(int r=0; r<n; ++r) {
                for(int c=0; c<n; ++c) {
                        temp(r,c) = s1(r,c) + s2(r,c);
                }
        }

        return temp;
}


SMatrix<float> diffOfSMatrix(SMatrix<float> s1, SMatrix<float> s2){
        int n = s1.size();
        SMatrix<float> temp(n);

        for(int r=0; r<n; ++r) {
                for(int c=0; c<n; ++c) {
                        temp(r,c) = s1(r,c) - s2(r,c);
                }
        }

        return temp;
}
