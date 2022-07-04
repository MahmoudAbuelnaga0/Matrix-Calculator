#include <iostream>
#include <cmath>
using namespace std;

int multiply_matrices(double matrix_A[][10], double matrix_B[][10], int &A_rows, int &A_cols, int &B_rows, int &B_cols);
int take_matrix_values(double matrix[][10], int &rows, int &cols);
void print_matrix(int matrix[][10], int &rows, int &cols);
int add_matrices(double matrix_A[][10], double matrix_B[][10], int &A_rows, int &A_cols, int &B_rows, int &B_cols); 
int subtract_matrices(double matrix_A[][10], double matrix_B[][10], int &A_rows, int &A_cols, int &B_rows, int &B_cols);
void extract_row(double matrix[][10], int &cols, double extracted_row[], int &index);
void extract_col(double matrix[][10], int &rows, double extracted_col[], int &index);
double dot_product(double row[], double column[], int &length);
void print_error_msg();
int single_subset(double matrix[][10], int &max_rows, int &max_cols, int &row_erased_index,int &col_erased_index, double storage_matrix[][10]);
double det(double matrix[][10], int max_rows, int max_cols);
int my_round(double num);
int calc_inverse_matrix(double matrix[][10], int &rows, int &cols, double inverse[][10]);
int calc_adjoint_matrix(double matrix[][10], int &max_rows, int &max_cols, double adjoint_matrix[][10]);
void calc_cofactor_matrix(double matrix[][10], int &max_rows, int &max_cols, double cofactor_matrix[][10]);


int main() {
    double matrix_A[10][10], matrix_B[10][10], inverse_B[10][10]; //Defining matrices A & B of size (10 x 10)
    int A_rows, A_cols, B_rows, B_cols, operation, done; // Defining variables to store input values of rows and columns

    // Asking using for matrix A dimensions
    cout << "Please enter dimensions of Matrix A:" << "\n";
    cin >> A_rows >> A_cols; // Taking inputs
    if (cin.fail()) return 0; //  Exit program if user mess with inputs

    // Asking for matrix B dimensions
    cout << "Please enter dimensions of Matrix B:" << "\n";
    cin >> B_rows >> B_cols; // Taking inputs
    if (cin.fail()) return 0; //  Exit program if user mess with inputs

    // Asking for values of matrix A
    cout << "Please enter values of Matrix A:" << "\n";
    done = take_matrix_values(matrix_A, A_rows, A_cols);   // Taking inputs formatrix A
        //  print_matrix(matrix_A, A_rows, A_cols);
    if(!done) return 0;   //  Exit program if user mess with inputs

    // Asking for values of matrix B
    cout << "Please enter values of Matrix B:" << "\n";
    done = take_matrix_values(matrix_B, B_rows, B_cols);   // Taking inputs for Matrix B
        //  print_matrix(matrix_B, B_rows, B_cols);
    if(!done) return 0;   //  Exit program if user mess with inputs

    while(1) {
        // Asking for the operation
        cout << "Please choose operation type(1: A+B, 2: A-B, 3: AxB, 4: A*inverse(B), 5: |A|, 6: |B|, 7: quit):" << "\n";  // Print message
        cin >> operation;   // Taking input
        // Exit program if user inputted a string
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10, '\n');
            continue;
        }   

        switch(operation) {
            case 1:
                done = add_matrices(matrix_A, matrix_B, A_rows, A_cols, B_rows, B_cols);
                if(!done) print_error_msg();
                break;
            case 2:
                done = subtract_matrices(matrix_A, matrix_B, A_rows, A_cols, B_rows, B_cols);
                if(!done) print_error_msg();
                break;
            case 3:
                done = multiply_matrices(matrix_A, matrix_B, A_rows, A_cols, B_rows, B_cols);
                if(!done) print_error_msg();
                break;
            case 4:
                done = calc_inverse_matrix(matrix_B, B_rows, B_cols, inverse_B);
                //cout << done;
                if(!done) {
                    print_error_msg();
                    break;
                } else {
                    done = multiply_matrices(matrix_A, inverse_B, A_rows, A_cols, B_rows, B_cols);
                    if(!done) {
                        print_error_msg();
                        break;
                    }
                }
                break;
            case 5:
                if ((A_rows != A_cols) || (A_rows == 1) || (A_cols == 1)) {
                    print_error_msg();
                }else {
                    cout << my_round(det(matrix_A, A_rows, A_cols)) << endl;
                }
                break;
            case 6:
                if ((B_rows != B_cols) || (B_rows == 1) || (B_cols == 1)) {
                    print_error_msg();
                } else {
                    cout << my_round(det(matrix_B, B_rows, B_cols)) << endl;
                }
                break;
            case 7:
                goto exit;
                break;
            default:
                print_error_msg();
        }
    }
    exit: ;
    cout << "Thank you!" << endl;
}


// A function that asks user for the inputs of matrix
// Takes(the matrix *where we are going to store the values*, no of rows, no of columns)
int take_matrix_values(double matrix[][10], int &rows, int &cols) {
    // Looping through rows
    for(int row = 0; row < rows; row++) {

        // Looping through columns
        for(int col = 0; col < cols; col++) {
            // Taking value from user at each matrix position
            cin >> matrix[row][col];
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10, '\n');
                return 0;
            }
        }   // End of loop

    }   // End of loop

    return 1;
}

// A function that prints the matrix
// Takes(the matrix, no of rows, no of columns)
void print_matrix(int matrix[][10], int &rows, int &cols) {
    // Looping through rows
    for(int row = 0; row < rows; row++) {

        // Looping through columns
        for(int col = 0; col < cols; col++) {
            // Printing values in matrix
            cout << matrix[row][col] << " ";
        }   // End of loop

        cout << "\n";
    }   // End of loop
}

// A funtion to multiply matrices with dimension (k x m) & (m x l)
// The result is a matrix of dimensions (k x l)
// Takes(matrix A, matrix B, no of rows of A, no of columns of A, no of rows of B, no of columns of B)
int multiply_matrices(double matrix_A[][10], double matrix_B[][10], int &A_rows, int &A_cols, int &B_rows, int &B_cols) {
    int res_matrix[10][10]; //Matrix to store the result
    double row[10], column[10];    // arrays to store the extractions of rows and columns

    // Check if operation can be done
    if(A_cols != B_rows) return 0;  // operation can't be done

    // Extracting rows of A and columns of B
    for (int index_of_A_row = 0; index_of_A_row < A_rows; index_of_A_row++) {
        extract_row(matrix_A, A_cols, row, index_of_A_row); // Extract current A row

        for(int index_of_B_col = 0; index_of_B_col < B_cols; index_of_B_col++) {
            extract_col(matrix_B, B_rows, column, index_of_B_col);  // Extract current B column
            res_matrix[index_of_A_row][index_of_B_col] = my_round(dot_product(row, column, A_cols));  //Input dot product of current A row and B column in the result matrix
        }
    }

    print_matrix(res_matrix, A_rows, B_cols);   // print result matrix at the end
    return 1;
}   


// A function that extracts row with certain index
// Takes(the matrix, no of rows, array *to store extracted column*, index of row to extract)
void extract_row(double matrix[][10], int &cols, double extracted_row[], int &index) { 
    // Looping through columns of row[index][]
    for(int col = 0; col < cols; col++) {
        extracted_row[col] = matrix[index][col];   // Saving values of columns of certain row in an array
            //  cout << extracted_row[col] << " "; 
    }
}

// A function that extracts column with certain index
// Takes(the matrix, no of rows, array *to store extracted column*, index of column to extract)
void extract_col(double matrix[][10], int &rows, double extracted_col[], int &index) {
    // Looping through rows of the column[][index]
    for(int row = 0; row < rows; row++) {
        extracted_col[row] = matrix[row][index];  // Saving values of rows of certain column in an array
            //  cout << extracted_col[row] << " ";
    }
}

// Dot product of 2 arrays of the same size
// Takes(array 1, array 2, length of any of 2 arrays)
double dot_product(double row[], double column[], int &length) {
    double accumulator = 0;
    for(int index = 0; index < length; index++) {
        accumulator += row[index] * column[index];
    }

    return accumulator;
}

void print_error_msg() {
    cout << "The operation you chose is invalid for the given matrices." << endl;
}

// Function to add matrices with the same dimensions
// Takes(matrix A, matrix B, no fo rows of A, no of columns of A, no of rows of B, no of columns of B)
int add_matrices(double matrix_A[][10], double matrix_B[][10], int &A_rows, int &A_cols, int &B_rows, int &B_cols) {
    if(A_rows != B_rows || A_cols != B_cols) return 0;    // If the two matrices don't have the same dimensions function return 0
    // Looping through the matrix
    for (int row = 0; row < A_rows; row++) {
        for(int col = 0; col < A_cols; col++) {
            cout << my_round(matrix_A[row][col] + matrix_B[row][col]) << " ";
        }
        cout << "\n";
    }
    return 1;
}

// Function to subtract matrices with the same dimensions
// Takes(matrix A, matrix B, no fo rows of A, no of columns of A, no of rows of B, no of columns of B)
int subtract_matrices(double matrix_A[][10], double matrix_B[][10], int &A_rows, int &A_cols, int &B_rows, int &B_cols) {
    if(A_rows != B_rows || A_cols != B_cols) return 0;  // If the two matrices don't have the same dimensions function return 0
    // Looping through the matrix
    for (int row = 0; row < A_rows; row++) {
        for(int col = 0; col < A_cols; col++) {
            cout << my_round(matrix_A[row][col] - matrix_B[row][col]) << " ";
        }
        cout << "\n";
    }
    return 1;
}

// The function subset the current matrix (n x n) into a smaller one with dimensions (n-1 x n-1)
// Takes (The matrix, no of rows of matrix, no of columns, index of column to erase, another matrix to store the subset values)
// By default the function removes row[0]
int single_subset(double matrix[][10], int &max_rows, int &max_cols, int &row_erased_index,int &col_erased_index, double storage_matrix[][10]) {
    int storage_row;
    int storage_col;

    // If the input matrix is (2 x 2) it's stored the same
    if (max_rows == 2 && max_cols == 2) {
        for(int i = 0; i < max_rows; i++) {
            for(int j = 0; j < max_cols; j++) {
                storage_matrix[i][j] = matrix[i][j];
            }
        }
        return 1;   // function stops
    }

    // ELSE the following is done
    // Looping through the original matrix and subsetting new one
    for (int row = 0; row < max_rows; row++) {
        if (row == row_erased_index) continue;
        if (row < row_erased_index)
            storage_row = row;
        else
            storage_row = row -1;

        for(int col = 0; col < max_cols; col++) {
            if (col == col_erased_index) continue;
            if (col < col_erased_index)
                storage_col = col;
            else
                storage_col = col - 1;

            storage_matrix[storage_row][storage_col] = matrix[row][col];
        }
    }

    return 1;
}

// Function to determine the determinant of a matrix
// Takes (the matrix, no of rows of matrix, no of columns of matrix)
double det(double matrix[][10], int max_rows, int max_cols) {
    double storage_matrix[10][10]; // Matrix to store the subset we obtain if the the square matrix is greater tham (2 x 2)
    double accumulator = 0;    // Just a normal accumulator
    int erased_row_index = 0;   // Erased row is always row 0

    // If the input matrix is (2 x 2) we return a result
    if(max_rows == 2 && max_cols == 2) {
        return ((matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]));
    }

    // If the matrix is not (2 x2) then we do the following:
    // Going through each column of row 0
    // Then getting a subset based on the column we are at
    // Get the determinant of the subset we got (the process will repeat until the matrix is (2 x 2))
    // Accumulate the values obtained from each column of row[0] and return the result
    for(int col = 0; col < max_cols; col++) {
        single_subset(matrix, max_rows, max_cols, erased_row_index, col, storage_matrix);
        if((col+1) % 2 == 0) {
            accumulator += -matrix[0][col] * det(storage_matrix, max_rows-1, max_cols-1);
        } else {
            accumulator += matrix[0][col] * det(storage_matrix, max_rows-1, max_cols-1);
        }
    }

    return accumulator;
}

int my_round(double num) {
    double fraction;
    if (num == 0) { // If the number is zero return it
        return num;
    } else {
        fraction = num - (int)num;
        if (abs(fraction) >= 0.5) {
            if (num > 0) {  // If number is postive
                return (int)num + 1;
            } else {    // If number is negative
                return (int)num - 1;
            }
        } else {
            return (int)num;
        }
    }
}

void calc_cofactor_matrix(double matrix[][10], int &max_rows, int &max_cols, double cofactor_matrix[][10]) {
    double storage_matrix[10][10];
    for(int row = 0; row < max_rows; row++) {
        for (int col = 0; col < max_cols; col++) {
            single_subset(matrix, max_rows, max_cols, row, col, storage_matrix);
            cofactor_matrix[row][col] = pow(-1, row+col+2) * det(storage_matrix, max_rows - 1, max_cols - 1);
        }
    }
}

int calc_adjoint_matrix(double matrix[][10], int &max_rows, int &max_cols, double adjoint_matrix[][10]) {
    if (max_rows == 2 && max_cols == 2) {
        adjoint_matrix[0][0] = matrix[1][1];
        adjoint_matrix[0][1] = -matrix[0][1];
        adjoint_matrix[1][0] = -matrix[1][0];
        adjoint_matrix[1][1] = matrix[0][0];
        return 1;
    }

    for(int row = 0; row < max_rows; row++) {
        for(int col = 0; col < max_cols; col++) {
            adjoint_matrix[col][row] = matrix[row][col];
        }
    }
    return 1;
}

int calc_inverse_matrix(double matrix[][10], int &rows, int &cols, double inverse[][10]) {
    double cofactor_matrix[10][10], adjoint_matrix[10][10];
    double determinant = det(matrix, rows, cols);

    if (determinant == 0 || rows != cols || rows == 1 || cols == 1) {
        return 0;   // There is no inverse for that matrix
    }

    if (rows == 2 && cols == 2) {
        calc_adjoint_matrix(matrix, rows, cols, adjoint_matrix);
    } else {
        calc_cofactor_matrix(matrix, rows, cols, cofactor_matrix);
        calc_adjoint_matrix(cofactor_matrix, rows, cols, adjoint_matrix);
    }

    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            inverse[row][col] = (1/determinant) * adjoint_matrix[row][col];
        }
    }
    return 1;
}   




/*int check_failure() {
    if(cin.fail()) {
        cin.clear();
        cin.ignore(10, '\n');
        return 0;
    }

    return 1;
} */

/*  int full_subset(int matrix[][10], int max_rows, int max_cols, int col_erased_index) {
    int col;
    int subset_matrix[10][10]; // Defining for variable to store the subset matrix
    // If the matrix is (2 x 2) stop the recursion process
    if (max_rows == 2 && max_cols == 2) {
        print_matrix(matrix, max_rows, max_cols);   // Remember to comment it later
        return 1;
    }

    // Looping through the original matrix and subsetting new one
    for(int row = 1; row < max_rows; row++) {
        if(col_erased_index == 0) {
            for(col = 1; col < max_cols; col++) {
                subset_matrix[row-1][col-1] = matrix[row][col];
            }
        } else {
            for(col = 0; col < max_cols; col++) {
                if(col < col_erased_index) {
                    subset_matrix[row-1][col] = matrix[row][col];
                } else if (col == col_erased_index) {
                    continue;
                } else {
                    subset_matrix[row-1][col-1] = matrix[row][col];
                }
            }
        }
    }

    subset(subset_matrix, (max_rows - 1), (max_cols - 1), col_erased_index);  // Subset the subset_matrix
    return 1;
}  */