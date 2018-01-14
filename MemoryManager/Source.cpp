#include<iomanip>
#include<iostream>
#include<vector>
#include<utility>
#include<string>
#include<algorithm>

using namespace std;

int getDigits(int n)
{
	int res = 0;
	if (n == 0)
	{
		return 1;
	}
	while (n != 0)
	{
		n /= 10;
		res++;
	}
	return res;
}

struct mem
{
	int pos;
	int size;
	bool isFree;
	mem()
	{
		pos = 0;
		size = 0;
		isFree = true;
	}
	mem(int ps, int sz, bool is)
	{
		pos = ps;
		size = sz;
		isFree = is;
	}
	mem split(int sz)
	{
		if (sz == size)
		{
			isFree = false;
			cout << pos << endl;
			return *this;
		}
		int resPos = pos;
		pos += sz + 1;
		size -= sz;
		mem res(resPos, sz, false);
		cout << resPos << endl;
		return res;
	}

	void unite(mem& a)
	{
		if (a.pos > pos)
		{
			size += a.size;
		}
		else
		{
			pos = a.pos;
			size += a.size;
		}
	}
};

bool isFree(mem& a)
{
	return a.isFree;
}

class memmoryManager
{
	vector<mem> memory;
	int memoryCount = 0;
	int width, maxSize;
public:
	void getParameters()
	{
		cout << "Please set memory size and max output width: " << endl;
		cin >> maxSize >> width;
		memory.push_back(mem(0, maxSize, true));
	}

	void printCommands()
	{
		cout << "Available commands :\nhelp - show this help\nexit - exit this program\nprint - print memory blocks map\nallocate <num> - ";
		cout << "allocate <num> cells. Returns block first cell number\nfree <num> - free block with first cell number<num>";
	}

	void Run()
	{
		string command;
		cout << "Type `help` for additional info." << endl;
		while (command != "exit")
		{
			cin >> command;
			if (command == "exit")
			{
				break;
			}
			if (command == "help")
			{
				printCommands();
			}
			if (command == "allocate")
			{
				allocate();
			}
			if (command == "print")
			{
				print();
			}
			if (command == "free")
			{
				free();
			}
		}
	}
	void allocate()
	{
		int size, pos;
		pos = 0;
		cin >> size;
		if (memoryCount + size > maxSize)
		{
			cout << "No space" << endl;
		}
		else
		{
			auto it = find_if(memory.begin(), memory.end(), isFree);
			while ((*it).size < size)
			{
				it = find_if(memory.begin(), memory.end(), isFree);
			}
			auto newMem = memory.insert(it, (*it).split(size));
			memoryCount += size;
		}
	}
	void free()
	{
		int pos;
		cin >> pos;
		auto it = find_if(memory.begin(), memory.end(), [&pos](mem& a)
		{
			return a.pos == pos;
		});
		if (it == memory.end())
		{
			cout << "No such pos" << endl; 
			return;
		}
		if (((*it).pos == 0) && (*(it + 1)).isFree)
		{
			(*(it + 1)).unite(*it);
		}
		else if (((it + 1) == memory.end()) && (*(it - 1)).isFree)
		{
			(*(it - 1)).unite(*it);
		}
		else
		{
			auto unite = find_if(it - 1, it + 1, isFree);
			if ((*unite).isFree) (*unite).unite(*it);
			else (*it).isFree = true;
		}
		memoryCount -= (*it).size;
		cout << memoryCount << endl;
		if (!(*it).isFree) memory.erase(it);
	}
	void print()
	{
		auto it = memory.begin();
		int q = 0, count;
		char ch;
		cout << "|";
		while (it != memory.end())
		{

			if (!(*it).isFree) q += getDigits((*it).pos);
			if ((*it).isFree)
			{
				ch = ' ';
				count = (*it).size;
			}
			else
			{
				cout << (*it).pos;
				ch = 'x';
				count = (*it).size - getDigits((*it).pos);
			}
			if (q >= width)
			{
				cout << "|" << endl << "|";
				q = 0;
			}
			for (int i = 0; i < count; i++)
			{
				if (i == count - 1)
				{
					if (q != width - 1 && ch == 'x')
					{
						cout << '|';
					}
					else if (q == width - 1 && ch == 'x')
					{
						cout << "x";
					}
					q++;
					break;
				}
				else cout << ch;
				q++;
				if (q >= width)
				{
					cout << "|" << endl << "|";
					q = 0;
				}
			}
			it++;
		}
	}
};

int main()
{
	memmoryManager a;
	a.getParameters();
	a.Run();
}
