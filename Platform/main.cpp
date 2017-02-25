#include "Input.h"

#include <iostream>                                   
  
int main()
{                                           
	std::string tapResult = Input::GetTap();
	std::string touchResult = Input::GetTouch();
	std::string mouseResult = Input::GetMouse();
	
	std::cout << tapResult << std::endl;
	std::cout << touchResult << std::endl;
	std::cout << mouseResult << std::endl;	
	
	return 0;                                          
}