#include <iostream>
#include <fstream>
#include "String.h"
using namespace std;

void toString(int number, char *result)
{
    if (number == 0)
    {
        result[0] = '0';
        result[1] = '\0';
        return;
    }
    char temp[20];
    int index = 0;

    do
    {
        temp[index++] = '0' + (number % 10);
        number /= 10;
    } while (number > 0);

    temp[index] = '\0';

    int j = 0;
    for (int i = index - 1; i >= 0; --i)
    {
        result[j++] = temp[i];
    }
    result[j] = '\0';
}

void replaceLine(String filename, int l, String replace)
{
    ifstream file(filename.getdata());
    String arr[6];
    char *buffer = new char[1000];
    for (int i = 0; i < 6; i++)
    {
        file.getline(buffer, 1000);
        if (i == l - 1)
        {
            arr[i] = replace;
        }
        else
        {
            arr[i] = buffer;
        }
    }
    file.close();

    ofstream updated_file(filename.getdata());

    for (int i = 0; i < 6; i++)
    {
        updated_file << arr[i] << '\n';
    }
    updated_file.close();
    return;
}

char *getLineFile(String filename, int l)
{
    ifstream file(filename.getdata());
    char *buffer = new char[1000];
    for (int i = 0; i < l; i++)
    {
        file.getline(buffer, 1000);
    }
    return buffer;
}
int stringToInt(String &s)
{
    int result = 0;
    int index = 0;

    int length = s.getLength();

    for (int i = index; i < length; i++)
    {
        result = result * 10 + (s[i] - '0');
    }

    return result;
}
char *extract(String s, int col, char end)
{
    int curr = 1;
    int count = 0;
    int length = s.getLength();
    int i = 0;

    for (; i < length; i++)
    {
        if (curr > col)
        {
            break;
        }
        if (s[i] == end)
        {
            curr++;
            continue;
        }
    }

    int start = i; // Save the starting position
    for (; i < length && s[i] != end; i++)
    {
        count++;
    }

    char *result = new char[count + 1];
    int j = start;
    int k = 0;

    for (; k < count; j++)
    {
        result[k++] = s[j];
    }
    result[count] = '\0';

    return result;
}

// char* extract(String s, int col, char end)
//{
//	int curr = 1;
//	int count = 0;
//	int length = s.getLength();
//	//*temp = s;
//	int i = 0;
//	for (;length;i++)
//	{
//		if (curr > col)
//		{
//			break;
//		}
//		if (s[i] == end)
//		{
//			curr++;
//			continue;
//		}
//		if (curr == col)
//		{
//			count++;
//
//		}
//
//
//
//
//
//
//	}
//
//	char* result = new char[count + 1];
//	int j = i - count - 1;
//	int k = 0;
//	for (;k < count;j++)
//	{
//		result[k++] = s[j];
//
//	}
//	result[count] = '\0';
//	return result;
int csToInt(char *s)
{
    int size = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        size++;
    }
    int result = 0;
    int index = 0;

    int length = size;

    for (int i = index; i < length; i++)
    {
        result = result * 10 + (s[i] - '0');
    }

    return result;
}