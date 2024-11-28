#include <iostream>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <cstdlib>

using namespace std;

template<typename T>
struct Point
{
    T x, y;

    Point(T x_val, T y_val) : x(x_val), y(y_val) {}

    Point() : x(0), y(0) {}
};

template<typename T>
class Polyline
{
private:
    Point<T>* _coords;
    size_t _number_points;
    size_t _capacity;

public:
    static constexpr double epsilon = 1e-6;

    Polyline(size_t num_points) : _number_points(num_points), _capacity(_number_points > 0 ? _number_points : 1)
    {
        if (num_points == 0)
            throw invalid_argument("Number of points must be greater than 0.");
        _coords = new Point<T>[_capacity];
    }

    Polyline(const Point<T>& point) : _number_points(1), _capacity(1)
    {
        _coords = new Point<T>[_capacity];
        _coords[0] = point;
    }

    Polyline(size_t num_points, T m1, T m2) :_number_points(num_points), _capacity(num_points)
    {
        _coords = new Point<T>[_capacity];
        for (size_t i = 0; i < _number_points; ++i)
        {
            T x = m1 +(rand()) / ((RAND_MAX / (m2 - m1)));
            T y = m1 + (rand()) / ((RAND_MAX / (m2 - m1)));
            _coords[i] = Point<T>(x, y);
        }
    }

    Polyline(const Polyline& other) : _number_points(other._number_points), _capacity(other._capacity)
    {

        if (_number_points > _capacity) {
            throw std::runtime_error("Number of points exceeds capacity in copied Polyline");
        }

        _coords = new Point<T>[_capacity];

        for (size_t i = 0; i < _number_points; ++i) {
            _coords[i] = other._coords[i];
        }
    }

    size_t get_number()
    {
        return _number_points;
    }

    Point<T>& get_coords()
    {
        return _coords;
    }

    Point<T>& operator[](size_t index)
    {
        if (index >= _number_points)
        {
            throw std::out_of_range("Index out of range");
        }
        return _coords[index];
    }

    const Point<T>& operator[](size_t index) const
    {
        if (index >= count)
        {
            throw std::out_of_range("Index out of range");
        }
        return _coords[index];
    }

    Polyline operator+(const Polyline& other) const
    {
        size_t new_number_points = this->_number_points + other._number_points;
        Polyline result(new_number_points);

        for (size_t i = 0; i < this->_number_points; ++i)
        {
            result[i] = this->_coords[i];
        }
        for (size_t j = 0; j < other._number_points; ++j)
        {
            result[this->_number_points + j] = other._coords[j];
        }

        return result;
    }

    Polyline& operator+=(const Point<T>& point)
    {

        if (_number_points >= _capacity) {

            size_t new_capacity = (_capacity == 0) ? 1 : _capacity * 2;
            Point<T>* new_coords = new Point<T>[new_capacity];


            for (size_t i = 0; i < _number_points; ++i) {
                new_coords[i] = _coords[i];
            }

            delete[] _coords;

            _coords = new_coords;
            _capacity = new_capacity;
        }


        _coords[_number_points] = point;
        _number_points++;

        return *this;
    }

    friend Polyline operator+(const Point<T>& point, const Polyline& polyline)
    {
        Polyline result(polyline._number_points + 1);
        result._coords[0] = point;
        for (size_t i = 0; i < polyline._number_points; ++i) {
            result._coords[i + 1] = polyline._coords[i];
        }
        result._number_points = polyline._number_points + 1;
        return result;
    }

    Polyline& operator=(const Polyline& other) {
        if (this == &other) {
            return *this;
        }

        delete[] _coords;
        _number_points = other._number_points;
        _capacity = other._capacity;
        _coords = new Point<T>[_capacity];
        for (size_t i = 0; i < _number_points; ++i) {
            _coords[i] = other._coords[i];
        }

        return *this;
    }

    bool operator==(const Polyline& other) const
    {
        if (_number_points != other._number_points)
            return false;

        for (size_t i = 0; i < _number_points; ++i)
        {
            
            if constexpr (std::is_arithmetic_v<T>) {
                if (abs(_coords[i].x - other._coords[i].x) > epsilon ||
                    abs(_coords[i].y - other._coords[i].y) > epsilon)
                {
                    return false;
                }
            }
            
            else if constexpr (std::is_same_v<T, std::complex<typename T::value_type>>) {
                if (abs(_coords[i].x - other._coords[i].x) > epsilon ||
                    abs(_coords[i].y - other._coords[i].y) > epsilon)
                {
                    return false;
                }
            }
        }

        return true;
    }
   





    bool operator!=(const Polyline& other) const
    {
        return !(*this == other);
    }

    double length() const
    {
        
        double total_length = 0.0;
        for (size_t i = 0; i < _number_points - 1; ++i)
        {
            bool sher = _coords[i].x.real();
            if (_coords[i+1].x == _coords[i].x)
            {
                total_length += abs(_coords[i+1].y - _coords[i].y);
            }
            if (_coords[i+1].y == _coords[i].y)
            {
                total_length += abs(_coords[i+1].x - _coords[i].x);
            }
            else
            {
                total_length += sqrt(pow(_coords[i+1].x - _coords[i].x, 2) + pow(_coords[i+1].y - _coords[i].y, 2));
            }
        }
        return total_length;
    }

    double length_complex() const
    {
        double total_length = 0.0;
        for (size_t i = 0; i < _number_points - 1; ++i)
        {
            complex<T> p1(_coords[i].x, _coords[i].y);
            complex<T> p2(_coords[i + 1].x, _coords[i + 1].y);

            total_length += std::abs(p2 - p1);  
        }
        return total_length;
    }

    friend ostream& operator<<(ostream& os, const Polyline& polyline)
    {
        for (size_t i = 0; i < polyline._number_points; ++i)
        {
            os << "(" << polyline._coords[i].x << ", " << polyline._coords[i].y << ")";
            if (i != polyline._number_points - 1)
                os << " -> ";
        }
        return os;
    }

    ~Polyline()
    {
        delete[] _coords;
    }
};

Polyline<double> create_letter()
{
    const size_t num_points = 8;
    Polyline<double> D(num_points);

    D[0] = Point<double>(0, 0);
    D[1] = Point<double>(0, 4);
    D[2] = Point<double>(2, 5);
    D[3] = Point<double>(4, 4);
    D[4] = Point<double>(4, 1);
    D[5] = Point<double>(2, 0);
    D[6] = Point<double>(0, 0);

    return D;
}

int main()
{
    Point<int> p1(1, 2);
    Polyline<int> l1(p1);
    cout << "Polyline 1:" << l1 << "\n";


    Polyline<int> l2(3);
    l2[0] = Point<int>(1, 2);
    l2[1] = Point<int>(2, 3);
    l2[2] = Point<int>(4, 5);
    cout << "Polyline 2:" << l2 << "\n";

    Polyline<float> l3(5, 1.0f, 8.0f);
    cout << "\nPolyline 3 (with random points):" << l3 << "\n";

    Polyline<int> l4 = Point<int>(-1, -1) + l2;
    cout << "\nPolyline 4 after adding point (-1, -1) at the beginning:\n";
    cout << "\nLength of Polyline 4: " << l4.length() << "\n";

    Polyline<int> l5 = l4 + l2;
    cout << "\nConcatenated l5 = l4 + l2:" << l5 << "\n";

    cout << "\nDemonstrate funcrtion:" << create_letter() << "\n";

    l4 += Point<int>(3, 4);
    cout << "\nPolyline 4 after adding point (3, 4):" << l4 << "\n";

    cout << "\nCopy constructor: " << "\n";
    Polyline<int> l6(3);
    l6[0] = Point<int>(10, 10);
    l6[1] = Point<int>(20, 20);
    l6[2] = Point<int>(30, 30);
    cout << "\nPolyline 6:" << l6 << "\n";


    Polyline<int> l6Copy = l6;
    cout << "\nCopy of Polyline 6 :" << l6 << "\n";





}