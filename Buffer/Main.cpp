#pragma warning (disable:4996)
#include <iostream>
#include "Buffer.h"
#include <string>
using namespace std;

int main() {
	string msg = "34567";
	try {
		char c;
		StackArrayBuffer buffer(msg, LITTLE_ENDIAN);
		
	}
	catch (BufferException bE) {
		cout << bE.getMessage() << endl;
	}
	system("pause");
	return 0;
}