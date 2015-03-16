// weak_ptr constructor example
#include <iostream>
#include <memory>
#include <map>
#include <string>
using namespace std;

int main () {
	typedef std::shared_ptr<string> strPtr;

	map <int, strPtr> index;

	index.insert(std::pair<int, strPtr>(1,(strPtr)new string("Hello")));
	index.insert(std::pair<int, strPtr>(2,(strPtr)new string("My")));
	index.insert(std::pair<int, strPtr>(3,(strPtr)new string("Friend")));

	std::cout << "Index contains:\n";
	for (std::map <int, strPtr>::iterator it = index.begin(); it!=index.end(); ++it)
		std::cout << it->first << " => " << *(it->second) << '\n';
	cout << endl;

	
	std::weak_ptr<string> weak(index[2]);

	std::cout << "1. weak " << (weak.expired()?"is":"is not") << " expired\n";

	index[2].reset();

	std::cout << "2. weak " << (weak.expired()?"is":"is not") << " expired\n";
	 
	cout << " ============================== 2nd PART =============================" << endl ;

	map <string, strPtr> indexStr;

	indexStr.insert(std::pair<string, strPtr>("1st",(strPtr)new string("Hello")));
	indexStr.insert(std::pair<string, strPtr>("2nd",(strPtr)new string("My")));
	indexStr.insert(std::pair<string, strPtr>("3rd",(strPtr)new string("Friend")));

	std::cout << "Index contains:\n";
	for (std::map <string, strPtr>::iterator it = indexStr.begin(); it!=indexStr.end(); ++it)
		std::cout << it->first << " => " << *(it->second) << '\n';
	cout << endl;

	std::weak_ptr<string> weakStr(indexStr["2nd"]);

	std::cout << "1. weak " << (weakStr.expired()?"is":"is not") << " expired\n";

	indexStr["2nd"].reset();

	std::cout << "2. weak " << (weakStr.expired()?"is":"is not") << " expired\n";
	
  system("pause");

  return 0;
}