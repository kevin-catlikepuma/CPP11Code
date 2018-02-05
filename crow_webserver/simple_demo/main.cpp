#include "crow_all.h"

using namespace std;

void func()
{
	this_thread::sleep_for(chrono::seconds(3));
	cout << "print process id in function" << this_thread::get_id() << endl;
	cout << "time out" << endl;
}

crow::json::wvalue func_json()
{
	crow::json::wvalue x;
	x["message"] = "Hello, World!";
	return x;
}

//https://github.com/ipkn/crow
int main()
{
	crow::SimpleApp app;
	CROW_ROUTE(app, "/")([]() {
		return "Hello, world";
	});

	CROW_ROUTE(app, "/json")(func_json);

	app.port(18080).multithreaded().run();

	return 0;
}