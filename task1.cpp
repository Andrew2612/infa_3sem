#include <iostream>
#include <vector>
#include <cstdint>

using u16 = uint16_t;

class MyLongNum
{
private:
    bool TryConstructNum(const std::string& string)
    {
        if (string.empty()) {return false;}

        length = string.length();
        
        if (string[0] == '0' && length != 1) {return false;}

        if (!isdigit(string[0]))
        {
            if (string[0] == '-')
            {
                if (length == 1 || string[1] == '0') {return false;}
                negative = true;
            }
            else
            {
                return false;
            }
        }

        size_t index = 0;
        if (negative) {index++;}
        for (index; index < length; index++)
        {
            if (!isdigit(string[index])) {return false;}
        }
        index = 0;
        if (negative) {index++;}
        for (index; index < length; index++)
        {
            digits.push_back(string[index] - '0');
        }
        if (negative) {length--;}

        return true;
    }

    MyLongNum AddMyNum(const MyLongNum& a, const MyLongNum& b) const
    {
        MyLongNum res;
        size_t index = 0;
        bool increase = false;
        u16 sum = 0;

        while (index < a.length && index < b.length)
        {
            sum = sum/10 + a.digits[a.length - index - 1] + b.digits[b.length - index - 1];

            res.digits.insert(res.digits.begin(), sum % 10);
            index++;
        }
        while (index < a.length)
        {
            sum = sum/10 + a.digits[a.length - index - 1];

            res.digits.insert(res.digits.begin(), sum % 10);
            index++;
        }
        while (index < b.length)
        {
            sum = sum/10 + b.digits[b.length - index - 1];

            res.digits.insert(res.digits.begin(), sum % 10);
            index++;
        }
        if (sum/10 != 0) {res.digits.insert(res.digits.begin(), 1);}

        res.length = res.digits.size();
        res.negative = a.negative;
        return res;
    }
    MyLongNum SubtractMyNum(const MyLongNum& a, const MyLongNum& b) const
    {
        MyLongNum res;
        size_t index = 0;
        bool decrease = false;
        u16 sum = 0;

        while (index < a.length && index < b.length)
        {
            if (a.digits[a.length - index - 1] > b.digits[b.length - index - 1])
            {
                sum = a.digits[a.length - index - 1] - b.digits[b.length - index - 1];
                if (decrease) {sum -= 1;}

                decrease = false;
            }
            else
            {
                sum = 10 + a.digits[a.length - index - 1] - b.digits[b.length - index - 1];
                if (decrease) {sum -= 1;}

                if (sum == 10) {sum = 0; decrease = false;}
                else {decrease = true;}
            }

            res.digits.insert(res.digits.begin(), sum);
            index++;
        }

        while (index < a.length)
        {
            sum = a.digits[a.length - index - 1];

            if (decrease)
            {
                if (sum == 0) {decrease = true; sum = 9;}
                else {sum--;}
            }
            res.digits.insert(res.digits.begin(), sum);
            index++;
        }
        res.length = res.digits.size();

        for (size_t i = 0; i < res.length; i++)
        {
            if (res.digits[0] == 0)
            {
                res.digits.erase(res.digits.cbegin());
            }
            else
            {
                break;
            }
        }

        res.length = res.digits.size();
        return res;
    }

public:
    bool negative = false;
    std::vector<u16> digits;
    u16 length = 0;
    MyLongNum()   //конструктор по умолчанию
    {
        length = 0;
        negative = false;
    }
    MyLongNum(std::string string)   //конструктор
    {
        if (!TryConstructNum(string))
        {
            std::cout << "Wrong input ^_^" << std::endl;
            length = 0;
            return;
        }
    }
    MyLongNum(const MyLongNum& rhs)  //копирование
        : length(rhs.length), negative(rhs.negative)
    {
        for (size_t i = 0; i < length; i++) {
            digits.push_back(rhs.digits[i]);
        }
    }
    MyLongNum(MyLongNum&& rhs)  //копирование перемещения
        : digits(rhs.digits), length(rhs.length), negative(rhs.negative)
    {
        rhs.length = 0;
        rhs.digits.clear();
    }

    MyLongNum& operator= (const MyLongNum& rhs) { //присваивание копированием
        if (this == &rhs) {return *this;}
        MyLongNum t(rhs);
        std::swap(length, t.length);
        std::swap(digits, t.digits);
        std::swap(negative, t.negative);
        return *this;
    }
    MyLongNum& operator= (MyLongNum&& rhs) { //присваивание премещением
        if (this == &rhs) {return *this;}
        MyLongNum t(rhs);
        std::swap(length, t.length);
        std::swap(digits, t.digits);
        std::swap(negative, t.negative);
        return *this;
    }

    bool operator ==(const MyLongNum& rhs) const
    {
        if (rhs.length != length || rhs.negative != negative) {return false;}

        for (int i = 0; i < length; i++)
        {
            if (digits[i] != rhs.digits[i]) {return false;}
        }
        return true;
    }
    bool operator !=(const MyLongNum& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator >(const MyLongNum& rhs) const
    {
        if (!negative && rhs.negative) {return true;}
        if (negative && !rhs.negative) {return false;}

        if (negative && length > rhs.length) {return false;}
        if (!negative && length > rhs.length) {return true;}

        for (int i = 0; i < length; i++)
        {
            if (digits[i] > rhs.digits[i])
            {
                return !negative;
            }
            else if (digits[i] < rhs.digits[i])
            {
                return negative;
            }
        }
        return false;
    }
    bool operator <(const MyLongNum& rhs) const
    {
        if (!negative && rhs.negative) {return false;}
        if (negative && !rhs.negative) {return true;}

        if (negative && length > rhs.length) {return true;}
        if (!negative && length > rhs.length) {return false;}

        for (size_t i = 0; i < length; i++)
        {
            if (digits[i] > rhs.digits[i])
            {
                return negative;
            }
            else if (digits[i] < rhs.digits[i])
            {
                return !negative;
            }
        }
        return false;
    }
    bool operator >=(const MyLongNum& rhs) const
    {
        return (*this > rhs || *this == rhs);
    }
    bool operator <=(const MyLongNum& rhs) const
    {
        return (*this < rhs || *this == rhs);
    }

    MyLongNum operator -() const
    {
        MyLongNum res = *this;
        res.negative = !negative;

        return res;
    }

    MyLongNum operator +(MyLongNum& b) const
    {
        if ((negative && b.negative)
            || (!negative && !b.negative)) {return AddMyNum(*this, b);}

        if (!negative && b.negative)
        {
            if (length == b.length)
            {
                if (*this > -b) {return SubtractMyNum(*this, b);}
                else {return -SubtractMyNum(-b, -*this);}
            }
            else if (length > b.length) {return SubtractMyNum(*this, b);}
            else {return -SubtractMyNum(-b, -*this);}
        }
        if (negative && !b.negative)
        {
            if (length == b.length)
            {
                if (-*this > b) {return -SubtractMyNum(-*this, -b);}
                else {return SubtractMyNum(b, *this);}
            }
            else if (b.length > length) {return SubtractMyNum(b, *this);}
            else {return -SubtractMyNum(-*this, -b);}
        }

        return b;
    }

    MyLongNum operator-(MyLongNum& b) const
    {
        if ((negative && !b.negative)
            || (!negative && b.negative)) {return AddMyNum(*this, b);}
        
        if (!negative && !b.negative)
        {
            if (length >= b.length) {return SubtractMyNum(*this, b);}
            else {return -SubtractMyNum(-b, -*this);}
        }

        if (negative && b.negative)
        {
            if (b.length > length) {return SubtractMyNum(b, *this);}
            else {return -SubtractMyNum(-*this, -b);}
        }

        return b;
    }

    MyLongNum& operator +=(MyLongNum& rhs)
    {
        *this = *this + rhs;
        return *this;
    }
    MyLongNum& operator -=(MyLongNum& rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    MyLongNum& operator ++()
    {
        MyLongNum one{"1"};
        *this += one;
        return *this;
    }
    MyLongNum operator ++(int)
    {
        MyLongNum one{"1"};
        MyLongNum tmp = *this;
        *this += one;
        return tmp;
    }
    MyLongNum& operator --()
    {
        MyLongNum one{"1"};
        *this -= one;
        return *this;
    }
    MyLongNum operator --(int)
    {
        MyLongNum one{"1"};
        MyLongNum tmp = *this;
        *this -= one;
        return tmp;
    }

    MyLongNum operator *(MyLongNum& b) const
    {
        MyLongNum res;

        size_t index = 0;
        u16 sum = 0;

        MyLongNum tmp;

        for (size_t i = 0; i < length; i++)
        {
            for (size_t j = 0; j < i; j++)
            {
                tmp.digits.push_back(0);
                tmp.length++;
            }

            for (size_t j = 0; j < b.length; j++)
            {
                sum = sum/10 + b.digits[b.length - 1 - j] * digits[length - 1 - i];

                tmp.digits.insert(tmp.digits.begin(), sum % 10);
                tmp.length++;
            }

            sum /= 10;
            if (sum != 0)
            {
                tmp.digits.insert(tmp.digits.begin(), sum % 10);
                tmp.length++;
                sum = 0;
            }

            res += tmp;

            tmp.digits.clear();
            tmp.length = 0;
        }

        if ((negative && b.negative)
            || (!negative && !b.negative)) {res.negative = false;}
        else {res.negative = true;}
        
        return res;
    }
    MyLongNum& operator *=(MyLongNum& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
};

std::ostream& operator<<(std::ostream & os, const MyLongNum& num) {
    if (num.length == 0) {return os;}

    os << "MyNum ";

    if (num.negative) {os << '-';}
    for (int i = 0; i < num.length; i++)
    {
        os << num.digits[i];
    }
    os << '\n';

    return os;
}

int main()
{
    MyLongNum a{"1000004"};
    MyLongNum b{"-10000005"};
    MyLongNum c = a + b;
    
    c *= a;

    std::cout << ++c;
    return 0;
}