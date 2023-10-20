#include <iostream>
#include<string>
#include <cmath>
#include <random>
#include <unordered_set>

using std::cout;
using std::cin;
using std::endl;

long eucledian_algorythm(long a, long b) {

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long mod_pow(long base, long exponent, long  mod) {
    long result = 1;

    base = base % mod;  // Убедимся, что base находится в диапазоне [0, mod - 1]

    while (exponent > 0) {
        // Если текущий бит экспоненты равен 1, умножаем result на base и берем остаток по модулю
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        // Возводим base в квадрат и берем остаток по модулю
        base = (base * base) % mod;

        // Экспонента сдвигается вправо
        exponent >>= 1;
    }

    return result;
}
class SymbolCalculator {
public:
    bool check(int a, int p) {
        if ((a < 0) || (a > p))
            throw std::invalid_argument("a должно быть положительным числом и меньше p. Ваше число a: " + std::to_string(a));
        else {
            return true;
        }
    }
    int legendre_symbol(int a, int p) {
        check(a, p);
        if (p <= 0 || (p % 2 == 0 && p != 2)) {
            throw std::invalid_argument("p должно быть простым числом или 2");
        }
        a = a % p;
        if (a < 0) {
            a += p; // Приводим a к положительному значению в диапазоне [0, p)
        }
        if (a == 0) {
            return 0;
        }
        if (a == 1) {
            return 1;
        }
        if (a % 2 == 0) {
            return legendre_symbol(a / 2, p) * pow(-1, (p * p - 1) / 8);
        }
        else {
            return legendre_symbol(p % a, a) * pow(-1, (a - 1) * (p - 1) / 4);
        }
    }
    int jacobi_symbol(int a, int n) {
        if (n <= 0 || n % 2 == 0) {
            throw std::invalid_argument("n должно быть нечетным положительным числом");
        }
        if (n == 1) {
            return 1;
        }
        a = a % n;
        if (a < 0) {
            a += n; // Приводим a к положительному значению в диапазоне [0, n)
        }
        if (a == 0) {
            return 0;
        }
        if (a == 1) {
            return 1;
        }
        if (a % 2 == 0) {
            return jacobi_symbol(a / 2, n) * pow(-1, (n * n - 1) / 8);
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
public:

        virtual bool customize_test(long const  &test_value) = 0;
        virtual int iterations_counter(const float& is_prime_min_probability) = 0;
        long randomiser(){
            std::random_device random_device; //рандомайзер без параметров
            std::mt19937 generator(random_device());
            std::uniform_int_distribution<> dis(500, 10000);
            return dis(generator);
        }
        long randomiser(long min, long max) { //рандомайзер с параметрами
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<long> dist(min, max);
            return dist(gen);
        }
        bool cheking_if_even(long const& value) {
            if (value % 2 == 0) {
                return true;
            }
            else {
                return false;
            }
        }
        long runTest(float is_prime_min_probability) { //сюда ещё нужно будет вписать название теста в качестве принимаемого параметра./////
            bool test_failed = false;
            int iterations_amount = iterations_counter(is_prime_min_probability);
            int succesful_operations = 0;
            int i = 0;
            long test_value = 0;
                 while (succesful_operations != iterations_amount) {
                     test_value = randomiser();
                     while (cheking_if_even(test_value)) {
                         test_value = randomiser();
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
                         catch (...) {
                             test_failed = true;
                             break;
                         }
                     }
                 }
            return test_value;
        }
protected: 


            
    };

    class FermaTest : public PrimalityTest {
    public:
        //void creating_a(long& result, std::unordered_set<int>  &uniquenumbers, long const& test_value) {
        //    bool is_unique_flag = false;
        //    while (!is_unique_flag) {
        //        result = randomiser(2, test_value - 1);
        //        if (uniquenumbers.find(result) != uniquenumbers.end()) {
        //            result = randomiser(2, test_value - 1);
        //        }
        //        else {
        //            is_unique_flag = true;
        //            uniquenumbers.insert(result);
        //        }
        //    }
        //};
        int iterations_counter( const float& is_prime_min_probability) override;
        bool customize_test(long const& test_value)  override;

   
    protected:

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
    
    bool FermaTest::customize_test(long const& test_value)
    {

        std::unordered_set<int> uniqueNumbers;
        if (test_value < 1) {
            return false;
        }
        else if (test_value <= 3) {
            return true;
        }
        long a = randomiser(2, test_value - 1);
        //creating_a(a, uniqueNumbers, test_value);

        if (mod_pow(a, test_value - 1, test_value) == 1) {
            return true;
        }
        else {
            throw(std::exception)"Число не прошло тест";
            return false;
        }


    }

    class MillerRabinTest : public PrimalityTest {
    public:
        int iterations_counter(const float& is_prime_min_probability) override;
        bool customize_test(long const& test_value) override;


    };   
    int MillerRabinTest::iterations_counter(const float& is_prime_min_probability) {
        if (is_prime_min_probability < 0.5 || is_prime_min_probability >= 1.0) {
            throw(std::exception)"Ошибка: Минимальная вероятность должна быть в диапазоне[0.5, 1).";
        }
        double q = 1.0 - is_prime_min_probability; // Вероятность ошибки
        int k = std::ceil(std::log(q) / std::log(0.25)); // Вычисление k
        return k;
    }
    
    bool MillerRabinTest::customize_test(long const&test_value) {
        if (test_value < 1) return false;
        if (test_value <= 3) return true;
        long d = test_value - 1;
        int poweroftwo = 0;
        while (d % 2 == 0) {
            d /= 2;
            poweroftwo++;
        }
        long a = randomiser(2, test_value - 2);
        long x = mod_pow(a, d, test_value); //тут возможно ошибка
        if (x == 1 || x == test_value - 1) {
            return true;
        }
        for (size_t i = 0; i < poweroftwo-1; i++)
        {
            x = mod_pow(x, 2, test_value);
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
    public:
        bool customize_test(long const& test_value) override;
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
    bool SoloveiStrassenTest::customize_test(long const& test_value) {
        long a = randomiser(2, test_value - 1);
        SymbolCalculator symbolCalculator;
        if (eucledian_algorythm(a, test_value) > 1) {
            throw(std::exception)"Число не прошло тест";
        }
        int jacobisymbol = symbolCalculator.jacobi_symbol(a, test_value);
        int s = mod_pow(a, (test_value - 1) / 2, test_value);
        if (jacobisymbol != s) {
            throw(std::exception)"Число не прошло тест";
        }
        else {
            return true;
        }
    }

int main()
{
    setlocale(LC_ALL, "Russian");
    FermaTest fermaTest;
    MillerRabinTest millerrabTest;
    SoloveiStrassenTest solovstrassenTest;
	try {
        for (size_t i = 0; i < 10; i++)
        {
            cout << solovstrassenTest.runTest(0.7) << endl;
        }
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

}

