#pragma warning (disable:4996)
#include <iostream>
#include "Buffer.h"
#include <string>
using namespace std;

int main() {
	string msg = "Huynh Hoang Kha";
	try {
		StackArrayBuffer aSB(msg, LITTLE_ENDIAN);
		cout << aSB.getString() << endl;
		cout << aSB.getCapacity() << endl;
		cout << aSB.getSize() << endl;
		aSB.push('C');
		cout << aSB.getString() << endl;
		cout << aSB.getCapacity() << endl;
		cout << aSB.getSize() << endl;
		char c;
		cout << aSB.top(&c) << endl;
		cout << aSB.getString() << endl;
		cout << aSB.getCapacity() << endl;
		cout << aSB.getSize() << endl;
		cout << aSB.pop(&c) << endl;
		cout << aSB.getString() << endl;
		cout << aSB.getCapacity() << endl;
		cout << aSB.getSize() << endl;
	}
	catch (BufferException bE) {
		cout << bE.getMessage() << endl;
	}
	system("pause");
	return 0;
}