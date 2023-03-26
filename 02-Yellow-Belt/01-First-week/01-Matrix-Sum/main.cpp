#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;


class Matrix {
public:
    Matrix() {
        NumRows = 0;
        NumCols = 0;
    };
    Matrix(int num_rows, int num_cols) {
        if (num_rows < 0 || num_cols < 0) {
            throw out_of_range("Constructor num_rows = " + to_string(num_rows) + " num_cols = " + to_string(num_cols));
        }
        if (num_rows == 0 || num_cols == 0) {
            num_cols = 0;
            num_rows = 0;
        }
        data.resize(num_rows);
        for (auto& row : data) {
            row.resize(num_cols);
        }
        NumRows = num_rows;
        NumCols = num_cols;
    }
    void Reset(int num_rows, int num_cols) {
        if (num_rows < 0 || num_cols < 0) {
            throw out_of_range("Reset num_rows = " + to_string(num_rows) + " num_cols = " + to_string(num_cols));
        }
        if (num_rows == 0 || num_cols == 0) {
            num_cols = 0;
            num_rows = 0;
        }
        data.resize(num_rows);
        for (auto& row : data) {
            row.resize(num_cols);
            for (auto& item : row) {
                item = 0;
            }
        }
        NumRows = num_rows;
        NumCols = num_cols;
    }
    int At(int num_row, int num_col) const {
        if (num_row < 0 || num_row >= NumRows || num_col < 0 || num_col >= NumCols) {
            throw out_of_range("At num_rows = " + to_string(num_row) + " num_cols = " + to_string(num_col));
        }
        return data[num_row][num_col];
    }
    int &At(int num_row, int num_col) {
        if (num_row < 0 || num_row >= NumRows || num_col < 0 || num_col >= NumCols) {
            throw out_of_range("&At num_rows = " + to_string(num_row) + " num_cols = " + to_string(num_col));
        }
        return data[num_row][num_col];
    }
    unsigned long GetNumRows() const {
        return NumRows;
    }
    unsigned long GetNumColumns() const {
        return NumCols;
    }
    friend std::istream& operator>>(std::istream& ist, Matrix &m);
    friend std::ostream& operator<<(std::ostream& ost, const Matrix &m);
    bool operator== (const Matrix &m) const {
        if (NumRows != m.NumRows || NumCols != m.NumCols) {
            return false;
        }
        for (unsigned long i = 0; i < NumRows; ++i) {
            for (unsigned long j = 0; j < NumCols; ++j) {
                if (data[i][j] != m.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    Matrix operator+(const Matrix& rhs) const {
        if (NumRows != rhs.NumRows || NumCols != rhs.NumCols) {
            throw invalid_argument("Operator +: numcols " + to_string(NumCols) + " vs " + to_string(rhs.NumCols));
        }
        Matrix result (NumRows, NumCols);
        for (unsigned long i = 0; i < NumRows; ++i) {
            for (unsigned long j = 0; j < NumCols; ++j) {
                result.data[i][j] = data[i][j] + rhs.data[i][j];
            }
        }
        return result;
    };
private:
    vector<vector<int>> data;
    unsigned long NumRows;
    unsigned long NumCols;
};

std::istream& operator>>(std::istream& cin, Matrix &m) {
    int num_rows, num_cols;
    cin >> num_rows >> num_cols;
    m.Reset(num_rows, num_cols);
    for (auto& row : m.data) {
        for (auto& item : row) {
            cin >> item;
        }
    }
    return cin;
};

std::ostream& operator<<(std::ostream& cout, const Matrix &m) {
    cout << m.NumRows << " " << m.NumCols << endl;
    for (const auto& row : m.data) {
        for (const auto& item : row) {
            cout << item << " ";
        }
        cout << endl;
    }
    return cout;
};


int main() {
  Matrix one;
  Matrix two;

  cin >> one >> two;
  cout << one + two << endl;
  return 0;
}

