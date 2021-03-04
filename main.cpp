#include <iostream>
#include <map>
#include <tuple>

template<typename T, int default_number>
class Matrix {
    using Matr = std::map<int, std::map<int, int>>;
public:
    class IntAdapter 
    {
        friend class Matrix;
    public:
        IntAdapter(Matrix &parent, int row, int col) : parent(parent), row(row), col(col){ }
        operator int() { 
            if ( parent.mat.find(row) != parent.mat.end() )
                if ( parent.mat[row].find(col) != parent.mat[row].end())
                    return parent.mat[row][col];
            return default_number;
        }
        int& operator =(const int &a) 
        {
            if (a == default_number)
            {
                if ( parent.mat.find(row) != parent.mat.end() )
                {
                    if ( parent.mat[row].find(col) != parent.mat[row].end()) {
                        parent.mat[row].erase(col);
                        if (parent.mat[row].size() == 0)
                            parent.mat.erase(row);
                    }
                }
            }
            else
                parent.mat[row][col] = a;
            return (int&)a;
        }
    private:
        Matrix& parent;
        int row;
        int col;
    };
    class RowProxy {
        friend class Matrix;
    public:
        RowProxy(Matrix &parent, int row) : parent(parent), row(row)
        { }
        IntAdapter operator[](int col)
        {
            return IntAdapter(parent, row, col);
        }
    private:
        Matrix& parent;
        int row;
    };
    class iterator
    {
    	friend class Matrix; 
    public:
        iterator(Matrix &parent, Matr::iterator it_1, std::map<int,int>::iterator it_2) : parent(parent), 
	it(it_1), it2(it_2) {}
	    iterator& operator=(const iterator &iter)
	    {
            it = iter.it;
            it2 = iter.it2;
	        return *this;
	    }
	    bool operator == (const iterator &iter) const
	    {
	        if(it != parent.mat.end())
	            return ((iter.it == it) && (iter.it2 == it2));
	        else 
                return (iter.it == it);
	    }
	    bool operator!=(const iterator &it) const
	    {
	        return !(it == *this);
	    }
	    iterator& operator++()
	    {
            it2 = ++it2;
            if(it2 == it->second.end()) {
                it++;
		    if(it != parent.mat.end())
		        it2 = it->second.begin();
	        }
            return *this;
        }		
	    std::tuple<int, int, int> operator*() const
	    {
            int a = it->first;
            int b = it2->first;
            int c = it2->second;
            return std::make_tuple(a, b, c);
	    }
	private:
	    Matr::iterator it;
	    std::map<int,int>::iterator it2;
	    Matrix& parent;
	};	
    int size() {
        int size = 0;
        for(const auto& row : mat)
            size += row.second.size();   
        return size;
    }
    RowProxy operator[](int row)
    {
        return RowProxy(*this, row);
    }
    
    iterator begin() {
        auto it_1 = mat.begin();
        auto it_2 = it_1->second.begin();
        iterator it(*this, it_1, it_2);
        return it;
    }
    iterator end() {
        auto it_1 = mat.end();
        auto it_2 = it_1->second.begin();
        iterator it(*this, it_1, it_2);
        return it;
    }
    
    Matr mat; // map<row, map<column,value>>
};


int main() {
    Matrix<int, 0> matrix_2;
    for(int i = 0; i < 10; i++)
    {
        matrix_2[i][i] = i;
        matrix_2[10-i][i] = 10-i;
    }        
    for(int i = 1; i <9; i++)
    {
        for(int j = 1; j<9; j++)
            std::cout << matrix_2[i][j] << " ";
        std::cout << std::endl;
    }    
    std::cout << matrix_2.size() << std::endl;

    for(auto c: matrix_2) {
        int x, y, v;
        std::tie(x,y,v) = c;
        std::cout << "[" << x << "]" <<
                     "[" << y << "]=" << v << std::endl;
    }
    return 0;
}
