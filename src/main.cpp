#include "Listener.hpp"
#include "Application.hpp"

int main() 
{
   	Application app;
   	if(app.Initialize()==false)
   		return -1;
   	
   	app.Run();

    return 0;
}
