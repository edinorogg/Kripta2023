#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <unordered_set>
#include <sstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random.hpp>

using namespace boost::multiprecision;
namespace mp = boost::multiprecision;
using std::cout;
using std::cin;
using std::endl;
using std::string;
enum class Test {
    FermaTest,
    MillerRabinTest,
    SoloveiStrassenTest
};


cpp_int eucledian_algorythm(cpp_int a, cpp_int b) {

    while (b != 0) {
        cpp_int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
cpp_int pow_big_int(cpp_int base, cpp_int exponent) {

    cpp_int result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base;
        }
        base = base * base;
        exponent >>= 1;
    }
    return result;
}

cpp_int mod_pow(cpp_int base, cpp_int exponent, cpp_int  mod) {

    cpp_int result = 1;
    cpp_int base_4_test = base;
    base_4_test = base_4_test % mod;  // Убедимся, что base находится в диапазоне [0, mod - 1]

    while (exponent > 0) {
        // Если текущий бит экспоненты равен 1, умножаем result на base и берем остаток по модулю
        if (exponent % 2 == 1) {
            result = (result * base_4_test) % mod;
        }
        // Возводим base в квадрат и берем остаток по модулю
        base_4_test = (base_4_test * base_4_test) % mod;

        // Экспонента сдвигается вправо
        exponent >>= 1;
    }

    return result;
}
class SymbolCalculator {
public:

    bool check(cpp_int const& a, cpp_int const& p) {
        if ((a < 0) || (a > p))
            throw std::invalid_argument("a должно быть положительным числом и меньше p.");
        else {
            return true;
        }
    }
    cpp_int legendre_symbol(cpp_int a, cpp_int p) {
        cout << "a is " << a << endl;
        cout << "p is " << p << endl;
        check(a, p);
        if (p <= 0 || (p % 2 == 0 && p != 2)) {
            throw std::invalid_argument("p должно быть простым числом или 2");
        }
        a = a % p;
        if (a < 0) {
            a += p; // приводим a к положительному значению в диапазоне [0, p)
        }
        if (a == 0) {
            return 0;
        }
        if (a == 1) {
            return 1;
        }
        if (a % 2 == 0) {
            return legendre_symbol(a / 2, p) * pow_big_int(-1, (p * p - 1) / 8);
        }
        else {
            return legendre_symbol(p % a, a) * pow_big_int(-1, (a - 1) * (p - 1) / 4);
        }
    }

    cpp_int jacobi_symbol(cpp_int a, cpp_int n) {
        if (n <= 0 || n % 2 == 0) {
            throw std::invalid_argument("n должно быть нечетным положительным числом");
        }
        if (n == 1) {
            return 1;
        }
        a = a % n;
        if (a < 0) {
            a += n; // приводим a к положительному значению в диапазоне [0, n)
        }
        if (a == 0) {
            return 0;
        }
        if (a == 1) {
            return 1;
        }
        if (a % 2 == 0) {
            return jacobi_symbol(a / 2, n) * pow_big_int(-1, (n * n - 1) / 8);
        }
        else {
            if (a % 4 == 3 && n % 4 == 3) {
                return -jacobi_symbol(n, a);
            }
            else {
                return jacobi_symbol(n, a);
            }
        }
    }


};
class PrimalityTest {
protected:

    virtual bool customize_test(cpp_int const& test_value) = 0;
    virtual int iterations_counter(const float& is_prime_min_probability) = 0;
    long randomiser() {
        std::random_device random_device; //рандомайзер без параметров
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<> dis(500, 10000);
        return dis(generator);
    }
    void randomiser(cpp_int const& min, cpp_int const& max, cpp_int& a) { //рандомайзер с параметрами

        boost::random::random_device gen;
        boost::random::uniform_int_distribution<cpp_int> dist(min, max);
        a = dist(gen);
    }
    void randomiser_bit_length(int n, cpp_int& test_value) {
        if ((n > 10000) || (n < 1)) {
            throw(std::invalid_argument)"Битовая длина числа не может иметь параметр более 10000 или менее 1";

        }
        cpp_int min = pow_big_int(2, n - 1);
        cpp_int max = pow_big_int(2, n) - 1;
        boost::random::random_device gen;
        boost::random::uniform_int_distribution<cpp_int> ui(min, max);

        test_value = ui(gen);
    }
    bool cheking_if_even(long const& value) {
        if (value % 2 == 0) {
            return true;
        }
        else {
            return false;
        }
    }
public: cpp_int runTest(float is_prime_min_probability, int key_length) { //сюда ещё нужно будет вписать название теста в качестве принимаемого параметра./////
    bool test_failed = false;
    int iterations_amount = iterations_counter(is_prime_min_probability);
    int succesful_operations = 0;
    int amount_of_tests = 0;
    int i = 0;
    cpp_int test_value = 0;
    while (succesful_operations != iterations_amount) {
        randomiser_bit_length(key_length, test_value);
        cpp_int result = test_value & 1;
        //cout <<"test value is " << test_value << endl;
        //cout << "result value is " << result << endl;
        while ((test_value & 1) != 1) {
            randomiser_bit_length(key_length, test_value);
        }
        i = 0;
        test_failed = false; //обнуляем переменные при выходе из цикла
        succesful_operations = 0;
        for (size_t i = 0; i < iterations_amount; i++)
        {
            try {
                customize_test(test_value);
                succesful_operations++;
            }
            catch (std::exception& e) {
                amount_of_tests++;
                test_failed = true;
                break;
            }
        }
    }
    cout << "Количество тестов для поиска: " << amount_of_tests + 1 << endl;
    return test_value;
}




};

class FermaTest : public PrimalityTest {
protected:
    int iterations_counter(const float& is_prime_min_probability) override;
    bool customize_test(cpp_int const& test_value)  override;
};

int FermaTest::iterations_counter(const float& is_prime_min_probability) {
    if (is_prime_min_probability < 0.5 || is_prime_min_probability >= 1.0) {
        throw(std::exception)"Ошибка: Минимальная вероятность должна быть в диапазоне[0.5, 1).";
    }
    else {
        int k = static_cast<int>(std::ceil(std::log(1.0 / (1.0 - is_prime_min_probability)) / std::log(2.0)));
        return k;
    }

}

bool FermaTest::customize_test(cpp_int const& test_value)
{

    std::unordered_set<int> uniqueNumbers;
    if (test_value < 1) {
        return false;
    }
    else if (test_value <= 3) {
        return true;
    }
    cpp_int a = 0;
    randomiser(2, test_value - 1, a);
    //creating_a(a, uniqueNumbers, test_value);

    if (powm(a, test_value - 1, test_value) == 1) {
        return true;
    }
    else {
        throw(std::exception)"Число не прошло тест";
        return false;
    }


}

class MillerRabinTest : public PrimalityTest {
protected:
    int iterations_counter(const float& is_prime_min_probability) override;
    bool customize_test(cpp_int const& test_value) override;
};
int MillerRabinTest::iterations_counter(const float& is_prime_min_probability) {
    if (is_prime_min_probability < 0.5 || is_prime_min_probability >= 1.0) {
        throw(std::exception)"Ошибка: Минимальная вероятность должна быть в диапазоне[0.5, 1).";
    }
    double q = 1.0 - is_prime_min_probability; // Вероятность ошибки
    int k = std::ceil(std::log(q) / std::log(0.25)); // Вычисление k
    return k;
}

bool MillerRabinTest::customize_test(cpp_int const& test_value) {
    if (test_value < 1) return false;
    if (test_value <= 3) return true;
    cpp_int d = test_value - 1;
    int poweroftwo = 0;
    while (d % 2 == 0) {
        d /= 2;
        poweroftwo++;
    }
    cpp_int a = 0;
    randomiser(2, test_value - 2, a);
    cpp_int x = powm(a, d, test_value); //тут возможно ошибка
    if (x == 1 || x == test_value - 1) {
        return true;
    }
    for (size_t i = 0; i < poweroftwo - 1; i++)
    {
        x = powm(x, 2, test_value);
        if (x == 1) {
            throw(std::exception)"Число не прошло тест";
        }
        else if (x == test_value - 1) {
            return true;
        }
    }
    throw(std::exception)"Число не прошло тест";
}
class SoloveiStrassenTest : public PrimalityTest {
protected:
    bool customize_test(cpp_int const& test_value) override;
    int iterations_counter(const float& is_prime_min_probability) override;
};
int SoloveiStrassenTest::iterations_counter(const float& is_prime_min_probability) {

    if (is_prime_min_probability < 0.5 || is_prime_min_probability >= 1.0) {
        throw(std::exception)"Ошибка: Минимальная вероятность должна быть в диапазоне[0.5, 1).";
    }
    else {
        int k = static_cast<int>(std::ceil(std::log(1.0 / (1.0 - is_prime_min_probability)) / std::log(2.0)));
        return k;
    }
}
bool SoloveiStrassenTest::customize_test(cpp_int const& test_value) {
    cpp_int a = 0;
    randomiser(2, test_value, a);
    SymbolCalculator symbolCalculator;
    if (eucledian_algorythm(a, test_value) > 1) {
        throw(std::exception)"Число не прошло тест";
    }
    int jacobisymbol;
    try {
        int jacobisymbol = symbolCalculator.jacobi_symbol(a, test_value).convert_to<int>();
    }
    catch (...) {
    };

    cpp_int s = powm(a, (test_value - 1) / 2, test_value);
    if (jacobisymbol != s) {
        throw(std::exception)"Число не прошло тест";
    }
    else {
        return true;
    }
}

class RSA {
public:
    void encryption();
protected:
    void get_key(int n, cpp_int& q, cpp_int& p);
    Test choose_test();
    [[nodiscard]] int get_key_length();
};
cpp_int mod_inverse(cpp_int a, cpp_int m) {
    cpp_int m0 = m;
    cpp_int x0 = 0, x1 = 1;

    while (a > 1) {
        cpp_int q = a / m;
        cpp_int t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) {
        x1 += m0;
    }

    return x1;
}
void RSA::get_key(int n, cpp_int& q, cpp_int& p) {
    FermaTest fermaTest;
    p = fermaTest.runTest(0.999, n);
    q = fermaTest.runTest(0.999, n);
}

Test RSA::choose_test() {

    int c;
    cout << "Выберите каким тестом вы хотите воспользоваться для генерации ключа" << endl;
    cout << "1. Тест Миллера-Рабина" << endl;
    cout << "2. Тест Ферма" << endl;
    cout << "3. Тест Соловея-Штрассена" << endl;
    do {
        string input;
        cin >> input;

        std::stringstream ss(input);

        if (ss >> c) {
            if (c > 3 || c < 1) {
                cout << "Введённое вами значение должно соответствовать номеру варианта." << std::endl;
                cin.clear();
            }
            else {
                break;
            }
        }
        else {
            cout << "Ошибка: Введенное значение не является числом. Попробуйте снова." << std::endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (c > 3 || c < 1);
    switch (c)
    {
    case 1:
        return Test::MillerRabinTest;
        break;
    case 2:
        return Test::FermaTest;
        break;
    case 3:
        return Test::SoloveiStrassenTest;
        break;
    default:
        throw(std::exception)"Что-то пошло не так при выборе теста";
    }
}
[[nodiscard]] int RSA::get_key_length() {
    cout << "Введите длину ключа в битах. Длина ключа ограничивается 1024 битами" << endl;

    while (true) {
        std::string input;
        int length_value;
        std::cout << "Введите число: ";
        std::cin >> input;

        std::stringstream ss(input);

        if (ss >> length_value) {
            if (length_value < 1 || length_value > 1024) {
                std::cout << "Ошибка: Введенное значение  не должно быть больше 1024 или меньше 0." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                return length_value;
                break;
            }
        }
        else {
            std::cout << "Ошибка: Введенное значение не является числом. Попробуйте снова." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void RSA::encryption() {
    int key_length = get_key_length();
    //Test variant = choose_test();
    cpp_int q, p;
    get_key(key_length, q, p);
    cout << "q is " << q << endl;
    cout << "p is " << p << endl;
    cpp_int n = p * q;
    cout << "n is " << n << endl;
    cpp_int EilerFunction = (p - 1) * (q - 1);
    int check = 0;
    long e = 65537;
    //do {
    //    e = randomiser();
    //    check = eucledian_algorythm(e, EilerFunction);
    //} while (check != 1);
    //check = 0;
    cpp_int d = 0;
    do {
        d = mod_inverse(e, EilerFunction);
    } while (d < (pow(n, 1 / 4) * 1 / 3));

    long LongNumber = 125;
    cpp_int C = mod_pow(LongNumber, e, n);
    cpp_int M = mod_pow(C, d, n);
    cout << LongNumber << endl;
    cout << M << endl;
}

long convert_text_to_number(const string& text) {
    int numericValue = 0;
    for (char i : text) {
        numericValue += static_cast<int>(i);
    }
    return numericValue;
}
string convert_number_to_text(long numericValue) {
    return 0;
}

int main()
{
    clock_t start = clock();
    setlocale(LC_ALL, "Russian");
    RSA test;
    test.encryption();



    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    cout << "\nTime spent: " << seconds;

}

