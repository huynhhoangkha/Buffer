#pragma warning (disable:4996)
#include <iostream>
#include "Buffer.h"
#include <string>
using namespace std;

int main() {
	string msg = "Huynh Hoang Kha";
	try {
		QueueArrayBuffer aSB(msg, LITTLE_ENDIAN);
		cout << aSB.getInt(0) << endl;
		aSB[3] = 'U';
		cout << aSB.getString() << endl;
		cout << aSB.getByte(3) << endl;
	}
	catch (BufferException bE) {
		cout << bE.getMessage() << endl;
	}
	system("pause");
	return 0;
}