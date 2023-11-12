//jagged array
//с семинара 6 по 8
//использовать перегрузку, & && аннотации, static functions, rule of five, unique_ptr, overload, ODR, наследование
//пересмотреть конспекты, учесть все по максимуму

template <typename T> class Matrix {
// некое представление
public:
// конструктор для создания матрицы, заполненной значением
Matrix(int cols, int rows, T val = T{});
// конструктор для создания из заданной последовательности
template <typename It>
Matrix(int cols, int rows, It start, It fin);
// "конструктор" для создания единичной матрицы
static Matrix eye(int n, int m);
}


//большая пятерка
template <typename T> class Matrix {
// некое представление
public:
// копирующий и перемещающий конструктор
Matrix(const Matrix &rhs);
Matrix(Matrix &&rhs);
// присваивание и перемещение
Matrix& operator=(const Matrix &rhs);
Matrix& operator=(Matrix &&rhs);
// деструктор
~Matrix();
}

template <typename T> class Matrix {
// некое представление
public: //селекторы
// базовые
int ncols() const; //количество столбцов
int nrows() const; // количество строк
// агрегатные
T trace() const;
bool equal(const Matrix& other) const;
bool less(const Matrix& other) const;
void dump(std::ostream& os) const;
}

template <typename T> class Matrix {
    // некое представление
public:
    // отрицание
    Matrix& negate() &; //возваращает указатель на this
    // почему не Matrix transpose() const?
    Matrix& transpose() &;
    // равенство
    bool equal(const Matrix& other) const;
}


template <typename T> class Matrix {
// ..... тут некое представление .....
    struct ProxyRow {
        T *row;
        const T& operator[](int n) const { return row[n]; }
        T& operator[](int n) { return row[n]; }
    };
public:
    // Мы бы хотели использовать m[x][y]
    ProxyRow operator[](int);
}


