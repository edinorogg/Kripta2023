#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

class Des {
public:
	virtual vector<vector <unsigned char>> Generate_Key();
private: 
	
protected: vector<unsigned char> value;

};
class Key : public Des {


};
void Print(const vector<unsigned char>& input) {
	for (int i : input) {
		std::cout << "текущее число: " << i << endl;
		for (int j = 0; j < 8; j++) {
			std::cout << ((i >> (7 - j)) & 1) << "";
		}
		std::cout << "\n";

	}
}

vector <unsigned char> Transposition(const vector<unsigned char>& input, const vector<int>& ptable) {

	unsigned char result{}; //временная переменная для засовывания байтов
	vector<unsigned char> output{}; //вектор байтов на выход
	int i = 0;
	int amount = ptable.size() % 8;
	for (int j : ptable) {
		i++;
		if (j > input.size() * 8) {
			std::cout << "Одно из значений перестановки больше чем количество битов в переданной информации" << endl;
			std::cout << "Это значение: " << j << endl;
			exit(0);
		}
		else if ((j != 0) && (j < input.size())) {
			int sector_num = j / 8;
			if (j % 8 == 0) {
				sector_num = j / 8 - 1;
			}
			int bit_position = j % 8;
			unsigned char current_bit = ((input.at(sector_num) >> (8 - bit_position)) & 1);
			result = result | current_bit;
			if (i % 8 != 0) {
				result = result << 1;
			}
			else {
				output.push_back(result);
				result = {};
			}

		}
		
	}
	if (amount != 0) {
		result = result >> 1;
		result = result << (8 - amount);
		output.push_back(result);
	}
	if (output.size() != input.size()) {
		int missing = input.size() - output.size();
		result = {};
		for (int i = 0; i < missing; i++) {
			output.push_back(result);
		}
	}
	std::cout << "This is input: " << endl;

	Print(input);

	std::cout << "This is output: " << endl;
		Print(output);
	return(output);
}

int main(){
	setlocale(LC_ALL, "Russian");
	const unsigned char length = UCHAR_MAX;
	//const short MAX2 = SHRT_MAX;
	unsigned char example = 191;
	vector<int> ptable {1,2,1,2,1,2,1,2,1};
	unsigned char result{};
	/*ptable = {2,1,2};*/
	ptable = {1,1,2,10,1,1,1,1};
	vector<unsigned char> input; //вектор байтов на вход
	vector<unsigned char> output; //вектор байтов на выход
	for (int i = 0; i < 3; i++)
	{
		input.push_back(example);
	}
	vector<unsigned char>outer =  Transposition(input, ptable);


	


}
