#include<iostream>
#include<vector>
#include<ctime>
using namespace std;
vector<int>known;//已知的數字
struct guess //存取猜過的數字
{
	int a;
	int b;
	int number;
	guess(){ a = 0; b = 0; number = 0; }
};
int arr[100][4];
bool if9(guess *n)//測試有沒有9
{
	if (n[0].a + n[0].b + n[1].a + n[1].b == 4) return false;//沒有9 return false
	else return true;
}
bool checkSame(vector<int>buffer, int x)//檢查有沒有重複選擇到
{
	bool same = false;
	for (int n = 0; n<buffer.size(); n++)
	{
		if (buffer[n] == x)
		{
			same = true;
			break;
		}
	}
	return same;
}
void decideA(guess *n, int p, int k)
{
	int count = 0;
	vector<int>buffer;
	buffer.push_back(-1);
	for (int r = 0; r<4; r++)//把選過的a扣掉
	if (known[r] != 0)
		buffer.push_back(r);
	for (;;)//隨機找尋一位
	{
		int x = rand() % 4;
		bool same = checkSame(buffer, x);
		if (same)
			continue;
		buffer.push_back(x);
		known[x] = arr[p][x];
		count++;
		if (count == k)
			break;
	}
}
void decideB(guess *n, int p, int x)
{
	int count = 0;
	vector<int>buffer;
	buffer.push_back(-1);
	for (int r = 0; r<4; r++)//把選過的a扣掉
	if (known[r] != 0)
		buffer.push_back(r);
	for (;;)
	{
		int y = rand() % 4;
		bool same = checkSame(buffer, y);
		if (same)
			continue;
		buffer.push_back(y);
		for (int m = 0; m<4; m++)
		{
			if (known[m] == 0 && y != m)//放入猜測的數字且不放在自己那格
			{
				known[m] = arr[p][y]; count++; break;
			}
			if (known[m] == 0 && y == m)//當y==m發生(發生在第2個b只剩一格時) 重新random
			{
				y = rand() % 4;
				bool same = checkSame(buffer, y);
				if (!same)
				{
					known[m] = arr[p][y]; count++; break;
				}
				else
					m = 0;
			}
		}
		if (count == x)
			break;
	}
}
bool error()//最後檢查數字有沒有重複
{
	bool error = false;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		if (known[i] == known[j] && i != j)
			error = true;
	}
	return error;
}
void checkAB(int ab[], int size, int u)
{
	int A = 0, B = 0;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			if (arr[u][i] == known[j] && i != j)
			{
				B++;
			}
			if (arr[u][i] == known[j] && i == j)
			{
				A++;
			}
		}
	}
	ab[0] = A;
	ab[1] = B;
}
bool correct = true;//檢查錯誤
void calculate34(guess *n, int i)
{
	known.clear();//vector known 初始化
	for (int s = 1; s <= 4; s++)
		known.push_back(0);
	for (int j = 0; j <= 1; j++)
	{
		arr[j][3] = n[j].number % 10;//個位
		arr[j][2] = (n[j].number / 10) % 10;//十位
		arr[j][1] = (n[j].number / 100) % 10;//百位
		arr[j][0] = (n[j].number / 1000);//千位
	}
	for (int p = 0; p <= 1; p++)
	{
		switch (n[p].a)
		{
		case 1:
			decideA(n, p, 1);
			break;
		case 2:
			decideA(n, p, 2);
			break;
		case 3:
			decideA(n, p, 3);
			break;
		case 4:
			decideA(n, p, 4);
			break;
		default:
			break;
		}
		switch (n[p].b)
		{
		case 1:
			decideB(n, p, 1);
			break;
		case 2:
			decideB(n, p, 2);
			break;
		case 3:
			decideB(n, p, 3);
			break;
		case 4:
			decideB(n, p, 4);
			break;
		default:
			break;
		}
	}
}
void calculate5(guess *n, int i)
{
	int count = 0;
	for (;;)
	{
		bool select = false;
		known.clear();//vector known 初始化
		known.push_back(rand() % 9 + 1);
		for (;;)//產生不含0的四位數字亂數
		{
			bool same = false;
			int num = rand() % 9 + 1;//產生1~9之間的亂數
			for (int t = 0; t<known.size(); t++)
			if (num == known[t])
			{
				same = true; break;
			}
			if (same)
				continue;
			else
				known.push_back(num);
			if (known.size() == 4)
				break;
		}
		for (int j = 0; j<i; j++)
		{
			arr[j][3] = n[j].number % 10;//個位
			arr[j][2] = (n[j].number / 10) % 10;//十位
			arr[j][1] = (n[j].number / 100) % 10;//百位
			arr[j][0] = (n[j].number / 1000);//千位
		}
		for (int u = 0; u<i; u++)
		{
			int ab[2] = { 0 };
			checkAB(ab, 2, u);
			if (ab[0] != n[u].a || ab[1] != n[u].b)
				break;
			if (ab[0] == n[u].a || ab[1] == n[u].b)
			{
				if (u == i - 1)
					select = true;
			}
		}
		if (select)
			break;
		count++;
		if (count == 9 * 8 * 7 * 6)
		{
			cout << "cheat" << endl; correct = false; break;
		}
	}
}
int generate(guess *n, int i)//電腦猜測的數字
{
	bool thereIs9 = if9(n);
	if (i == 0) { return 1234; }
	else if (i == 1){ return 5678; }
	else if (i == 2 || i == 3)
	{
		if (thereIs9)//如果有9
		{
			calculate34(n, i);
			for (int m = 0; m<4; m++) //放9到空的vector
			if (known[m] == 0)
				known[m] = 9;
			for (;;)
			{
				if (error())
					calculate34(n, i);
				else
					break;
			}
			return known[0] * 1000 + known[1] * 100 + known[2] * 10 + known[3];
		}
		else
		{
			calculate34(n, i);
			for (;;)
			{
				if (error())
					calculate34(n, i);
				else
					break;
			}
			return known[0] * 1000 + known[1] * 100 + known[2] * 10 + known[3];
		}
	}
	else
	{
		calculate5(n, i);
		return known[0] * 1000 + known[1] * 100 + known[2] * 10 + known[3];
	}
}
vector<int>selection;//防止後面要猜的數字跟前面一樣
int main()
{
	srand((unsigned)time(NULL));
	guess *n = new guess[100];
	for (int x = 1; x <= 4; x++)
		known.push_back(0);
	int number = 0;
	selection.push_back(0);
	int i = 0;
	for (;; i++)
	{
		number = generate(n, i);
		for (int k = 0; k < selection.size(); k++)
		{
			if (number == selection[k])
			{
				number = generate(n, i); k = 0;
			}
		}
		selection.push_back(number);
		n[i].number = number;
		if (correct == false)
			break;
		else
			cout << n[i].number << endl;
		
		cin >> n[i].a;
		if (n[i].a > 4){cout << "cheat" << endl;break;}//輸入大於4錯誤
		
		
		cin>>n[i].b;
		if (n[i].b > 4){ cout << "cheat"<<endl; break; }//輸入大於4錯誤
		
		
		if (n[i].a + n[i].b > 4){ cout << "cheat"<<endl ; break; }//A+B大於四錯誤
		if (n[0].a + n[0].b == 4)
		{ 
			/*if (n[i].a ==4 && n[i].b==0)
			{
				cout << "Done" << endl;
				break;
			}
			else if(n[i].a ==3 && n[i].b==1)
			{
				cout << "cheat!it's 4A" << endl;
				break;
			}
			
			else
			{*/
				i++; 
				n[1].a = 0;
				n[1].b = 0;
				n[1].number = 5678;
			//}	
		}
		if (n[0].a + n[0].b + n[1].a + n[1].b > 4 )//第一組數字的A跟B加第二組數字的A跟B不能超過4
		{
			cout << "cheat" << endl;
			break;
		}
		if(n[0].a + n[0].b + n[1].a + n[1].b < 3 && i==1)//第一組數字的A跟B加第二組數字的A跟B不能小於三
        {
			cout << "cheat" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}