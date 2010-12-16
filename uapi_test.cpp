#include <iostream>
#include "uapi/uforumapi.h"
using namespace std;

int main(void)
{
	UForumApi* uapi = new UForumApi("pichi", "iampichi");
	std::cout << "Is Login: " << std::boolalpha << uapi->Login() << std::endl;
	delete uapi;
	return 0;
}