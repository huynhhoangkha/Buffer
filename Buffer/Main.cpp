#pragma warning (disable:4996)
#include <iostream>
#include "Buffer.h"
#include <string>
using namespace std;

int main() {
	string msg = "Huynh Hoang Kha";
	try {
		ArrayBuffer aSB(msg, LITTLE_ENDIAN);
		aSB.writeInt(3, 1511449);
		cout << aSB.getInt(3) << endl;
		//cout << aSB.getString() << endl;
		//cout << aSB.getByte(3) << endl;
	}
	catch (BufferException bE) {
		cout << bE.getMessage() << endl;
	}
	system("pause");
	return 0;
}