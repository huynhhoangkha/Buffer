#pragma warning (disable:4996)
#include <iostream>
#include "Buffer.h"
#include <string>
using namespace std;

int main() {
	string msg = "34567";
	try {
		char c;
		QueueArrayBuffer buffer(msg, LITTLE_ENDIAN);
		cout << buffer.getString() << endl;
		buffer.deQueue(&c); cout << c << endl; cout << buffer.getString() << endl;
		buffer.deQueue(&c); cout << c << endl; cout << buffer.getString() << endl;
		buffer.deQueue(&c); cout << c << endl; cout << buffer.getString() << endl;
		buffer.deQueue(&c); cout << c << endl; cout << buffer.getString() << endl;
		cout << endl << "Size: " << buffer.getSize() << endl;
		buffer.enQueue('6'); cout << buffer.getString() << endl;
		buffer.enQueue('5'); cout << buffer.getString() << endl;
		buffer.enQueue('4'); cout << buffer.getString() << endl;
		buffer.enQueue('3'); cout << buffer.getString() << endl;
		cout << endl << "Size: " << buffer.getSize() << endl;


	}
	catch (BufferException bE) {
		cout << bE.getMessage() << endl;
	}
	system("pause");
	return 0;
}