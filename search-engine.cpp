#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;
char unique_terms[10000][100] = { '\0' };
char doc_list[50][30] = { '\0' };
char stop_word[500][20] = { '\0' };
int term_frequency_in_all_documents[11][100] = { '\0' };
int k = 0;
int z = 0;
int mystrlen(char arr[])
{
	int i;
	for (i = 0; arr[i] != '\0'; i++);
	return i;
}
int mystrcmp(char arr1[], char arr2[])
{
	if (arr1[0] == '\0')
	{
		return 1;
	}
	for (int i = 0; i < mystrlen(arr1) || i < mystrlen(arr2); i++)
	{
		if (arr1[i] > arr2[i])
		{
			return 1;
		}
		else if (arr2[i] > arr1[i])
		{
			return -1;
		}
	}
	return 0;
}
int mystrcpy(char arr1[100], char arr2[100])
{
	for (int i = 0; i < 100; i++)
	{
		arr1[i] = '\0';
	}
	for (int i = 0; arr2[i] != '\0'; i++)
	{
		arr1[i] = arr2[i];
	}
	return 0;
}
void read_stop_words()
{
	cout << "\n<==Reading stop word list==>\n";
	ifstream stopfin("stoplist.txt");
	if (stopfin.is_open())
	{
		for (int i = 0; i < 500; i++)
		{
			stopfin >> stop_word[i];
		}
	}
	else
		cout << "Stop word file not opened \n";
	stopfin.close();
}
void read_doc_list()
{
	cout << "\n<==Reading doclist ==>\n";
	ifstream doclistfin("doclist.txt");
	if (doclistfin.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			doclistfin >> doc_list[i];
		}
	}
	else
		cout << "Doc list file not opened \n";
	doclistfin.close();
}
void write_docs_with_IDs()
{
	cout << "\n<==Writing file docids==>\n";
	ofstream docidout("docids.txt");
	if (docidout.is_open())
	{
		for (int i = 0; i < 50 && doc_list[i][0] != '\0'; i++)
		{
			docidout << i << "\t";
			for (int j = 0; j < 30 && doc_list[i][j] != '\0'; j++)
			{
				docidout << doc_list[i][j];
			}
			docidout << endl;
		}
	}
	docidout.close();
}
void convert_to_tokens(char docname[30])
{
	char word[100] = { '\0' };
	ifstream fin;
	fin.open(docname);
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> word;
			char temp[100] = { '\0' };
			int j = 0;
			bool found = false;
			for (int i = 0; stop_word[i][0] != '\0' && !found; i++)
			{
				if (mystrcmp(stop_word[i], word) == 0)
					found = true;

				else
					found = false;
			}
			if (!found && mystrlen(word) > 2)
			{

				for (int i = 0; word[i] != '\0'; i++)
				{
					if ((word[i] > 64 && word[i] < 91))
					{
						temp[j] = word[i] + 32;
						j++;
					}
					if ((word[i] > 96 && word[i] < 123))
					{
						temp[j] = word[i];
						j++;
					}
				}
				temp[j] = '\0';
				for (int i = 0; stop_word[i][0] != '\0' && !found; i++)
				{
					if (mystrcmp(stop_word[i], temp) == 0)
						found = true;

					else
						found = false;
				}
				for (int i = 0; i < 10000 && !found; i++)
				{
					if (mystrcmp(unique_terms[i], temp) == 0)
						found = true;
					else
						found = false;
				}
				if (!found && mystrlen(temp) > 2)
				{
					mystrcpy(unique_terms[k], temp);
					if (temp[0] != '\0')
						k++;

				}
			}
		}
	}
	else
		cout << "File " << docname << " not opened \n";
	fin.close();
}
void sort()
{
	cout << "\n<==sorting unique terms==>\n";
	char temp[100] = { '\0' };
	for (int i = 1; unique_terms[i][0] != '\0'; i++)
	{
		for (int j = 1; unique_terms[j][0] != '\0'; j++)
		{
			if (mystrcmp(unique_terms[j - 1], unique_terms[j]) > 0)
			{
				mystrcpy(temp, unique_terms[j - 1]);
				mystrcpy(unique_terms[j - 1], unique_terms[j]);
				mystrcpy(unique_terms[j], temp);
			}
		}
	}
}
void write_term_IDs()
{
	cout << "\n<==Writing file term ids==>\n";
	ofstream termout("termids.txt");
	if (termout.is_open())
	{
		for (int i = 0; unique_terms[i][0] != '\0'; i++)
		{
			termout << i << "\t";
			for (int j = 0; unique_terms[i][j] != '\0'; j++)
			{
				termout << unique_terms[i][j];
			}
			termout << endl;
		}
	}
	termout.close();
}
void write_doc_index()
{
	cout << "\n<==Writing file doc index==>\n";
	ofstream fout("doc_index.txt");
	for (int k = 0; doc_list[k][0] != '\0'; k++)
	{
		for (int l = 0; unique_terms[l][0] != '\0'; l++)
		{
			ifstream fin(doc_list[k]);
			if (fin.is_open() == 1)
			{
				char word[100] = { '\0' };
				int n = 0;
				int position[1000];
				position[0] = -1;
				for (int i = 0; i < 100; i++, position[i] = -1);
				for (int m = 0; !fin.eof(); m++)
				{
					fin >> word;
					char temp[100] = { '\0' };
					int j = 0;
					for (int i = 0; word[i] != '\0'; i++)
					{
						if ((word[i] > 64 && word[i] < 91))
						{
							temp[j] = word[i] + 32;
							j++;
						}
						if ((word[i] > 96 && word[i] < 123))
						{
							temp[j] = word[i];
							j++;
						}
					}
					temp[j] = '\0';

					if (mystrcmp(temp, unique_terms[l]) == 0)
					{
						position[n] = m;
						n++;
					}
				}
				if (position[0] != -1)
				{

					fout << k << "\t" << l << "\t";
					for (int i = 0; position[i] != -1; i++)
					{
						fout << position[i];
						if (position[i + 1] != -1)
							fout << "\t";
					}fout << endl;
					n = 0;
				}
			}
			fin.close();
		}
	}
	fout.close();
}
int get_termid(char arr[])
{
	cout << "\n<==Getting term id==>\n";
	bool found = false;
	for (int i = 0; stop_word[i][0] != '\0' && !found; i++)
	{
		if (mystrcmp(stop_word[i], arr) == 0)
			found = true;

		else
			found = false;
	}
	char temp[100] = { '\0' };
	for (int i = 0; arr[i] != '\0'; i++)
	{
		if ((arr[i] > 64 && arr[i] < 91))
		{
			temp[i] = arr[i] + 32;
		}
		for (int i = 0; i < 1000 && !found; i++)
		{
			bool found = true;
			char arr1[20] = { '\0' };
			if (mystrcmp(arr, unique_terms[i]) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}
void read_docs_list()
{
	cout << "\n<==Reading file doc list==>\n";
	ifstream file_docs("docids.txt");
	if (file_docs.is_open())
	{
		int docids = -1;
		char doc[20];
		for (int i = 0, j = 0; i < 50; i++)
		{
			doc_list[i][j] = -1;
		}
		for (int i = 0; i < 50 && (file_docs.eof() != 1); i++)
		{
			file_docs >> docids >> doc;

			for (int j = 0; doc[j] != '\0'; j++)
			{
				doc_list[i][j] = doc[j];
			}
		}
	}
	else
	{
		cout << "File not opened\n";
	}
	file_docs.close();
}
void read_unique_terms()
{
	cout << "\n<==Reading file term ids==>\n";
	ifstream file_terms("termids.txt");
	if (file_terms.is_open())
	{
		int termID = -1;
		char term[100];
		for (int i = 0; i < 1000 && (file_terms.eof() != 1); i++)
		{
			file_terms >> termID >> term;

			for (int j = 0; term[j] != '\0'; j++)
			{
				unique_terms[i][j] = term[j];
			}
		}
	}
	else
	{
		cout << "File not opened\n";
	}
	file_terms.close();
}
void find_term_in_documents(int tID)
{
	cout << "\n<==Getting position in all files==>\n";
	ifstream file_doc_index("doc_index.txt");
	if (file_doc_index.is_open())
	{
		int dID = 0;
		if (tID == -1)
			term_frequency_in_all_documents[dID][0] = -2;
		else
		{
			while (dID < 11)
			{
				int docID = -1, termID = -1;
				file_doc_index >> docID >> termID;
				while (docID != dID)
				{
					int ignorep = 0;
					int next = file_doc_index.peek();
					while (10 != next)
					{
						file_doc_index >> ignorep;
						next = file_doc_index.peek();
					}
					file_doc_index >> docID >> termID;
				}
				bool found = false;
				while (docID == dID && !found)
				{
					if (termID == tID)
					{
						int position = -1;
						int next = file_doc_index.peek();
						int i = 0;
						while (10 != next)
						{
							file_doc_index >> position;
							if (position != 0)
								term_frequency_in_all_documents[dID][i] = position;
							else
								term_frequency_in_all_documents[dID][i] = -2;
							next = file_doc_index.peek();
							i++;
						}
						found = true;
						dID++;
					}
					else
					{
						int ignorep = 0;
						int next = file_doc_index.peek();
						while (10 != next)
						{
							file_doc_index >> ignorep;
							next = file_doc_index.peek();
						}
						file_doc_index >> docID >> termID;
					}
					if (docID == dID + 1)
					{
						term_frequency_in_all_documents[dID][0] = -1;
						dID++;
					}
					if (file_doc_index.eof())
					{
						term_frequency_in_all_documents[dID][0] = -1;
						dID++;
					}
				}
			}
		}
	}
	else
		cout << "File not opened\n";
	file_doc_index.close();
}
void print_1d_array(int arr[])
{
	bool found = true;
	for (int j = 0; arr[j] != '\0' && found; j++)
	{
		if (arr[j] == -2)
			cout << "0 ";
		else if (arr[0] == -1)
		{
			cout << "Termid does not exit in this document";
			found = false;
		}
		else if (arr[j] != '\0')
			cout << arr[j] << " ";
	}
}
void print_2d_arr(int arr[][100], int rows)
{
	cout << "\nPrinting 2d-->printing 1d\n";
	cout << "\nThe position in all files are\n";
	if (arr[0][0] != -2)
	{
		for (int i = 0; i < rows; i++)
		{
			cout << " IN DOCUMENT " << i << " POSITION IS/ARE :";
			print_1d_array(arr[i]);
			cout << endl;
		}
	}
	else
		cout << "Term not present in any documents \n";
}
void write_history(char arr[])
{
	cout << "\n<==Writing history in file==>\n";
	ofstream fout("history.txt", ios_base::app);
	if (fout.is_open())
	{
		struct tm temp;
		time_t raw = time(0);
		localtime_s(&temp, &raw);
		fout << endl << arr << "\t\t";
		fout << temp.tm_hour << ":" << temp.tm_min << ":" << temp.tm_sec << "\t\t";
		fout << temp.tm_mday << ":" << temp.tm_mon << ":" << 1900 + temp.tm_year;
	}
}
int is_sub(char arr[])
{
	while (unique_terms[z][0] != '\0')
	{
		bool found = true;
		if (mystrlen(arr) < mystrlen(unique_terms[z]))
		{
			for (int j = 0; arr[j] != '\0' && found; j++)
			{
				if (arr[j] != unique_terms[z][j])
					found = false;
			}
			if (found)
			{
				z++;
				return z - 1;
			}
		}
		z++;
	}
	return -1;
}
int hamming(char arr[], char arr1[])
{
	int i = 0;
	if (mystrlen(arr) == mystrlen(arr1))
	{
		for (int j = 0; arr[j] != '\0'; j++)
		{
			if (arr[j] != arr1[j])
				i++;
		}
		return i;
	}
	return 4;
}
int main()
{
	cout << "<==WELCOME TO SEARCH ENGINE==>\n";
	int x;
	cout << "Enter 1 to continue with part 1 \n";
	cout << "Enter 2 to skip part 1 \n";
	cout << "Your option :";
	cin >> x;
	while (x != 0)
	{
		if (x == 1)
		{
			read_doc_list();
			read_stop_words();
			write_docs_with_IDs();
			cout << "\n<==Getting unique terms from all files==>\n";
			for (int i = 0; doc_list[i][0] != '\0'; i++)
			{
				convert_to_tokens(doc_list[i]);
			}
			sort();
			write_term_IDs();
			write_doc_index();
		}
		if (x == 2)
		{
			read_docs_list();
			read_unique_terms();
		}
		if (x == 1 || x == 2)
		{
			int h = 0;
			char arr[100] = { '\0' };
			cout << "\nType term :";
			cin >> arr;
			for (int u = 0; arr[u] != '\0'; u++)
			{
				if ((arr[u] > 64 && arr[u] < 91))
					arr[u] = arr[u] + 32;
			}
			bool wrong_spell = false;
			if (get_termid(arr) != -1)
			{
				find_term_in_documents(get_termid(arr));
				wrong_spell = true;
			}
			if (!wrong_spell)
			{
				cout << "\n<==Checking is the term is prefix of any word==>\n";
				int b = -2;
				int a = 0;
				while (b != -1)
				{
					b = is_sub(arr);
					if (b != -1 && mystrlen(arr) < mystrlen(unique_terms[b]))
					{
						if (a == 0)
						{
							cout << "\nThe term you entered does not exist in files, following is the list of words with their ids matching your term\n";
						}
						cout << b << "\t" << unique_terms[b] << endl;
						a++;
						wrong_spell = true;
					}
				}
				if (a != 0)
				{
					cout << "\nEnter the term id you want to search :";
					cin >> h;
					find_term_in_documents(h);
				}
			}
			if (get_termid(arr) == -1 && !wrong_spell)
			{
				cout << "\n<==Checking if you have entered wrong spelling==>\n";
				int a = 0;
				for (int i = 0; unique_terms[i][0] != '\0'; i++)
				{
					if (hamming(arr, unique_terms[i]) < 3)
					{
						if (a == 0)
							cout << "\nThe term you entered does not exist in files, following is the list of words with their ids matching your term\n";
						a = 2;
						cout << i << "\t" << unique_terms[i] << "\n";
					}
				}
				if (a == 2)
				{
					cout << "\nEnter the term id you want to search :";
					cin >> h;
					find_term_in_documents(h);
				}
			}

			cout << "The positions of term in all the documents are :\n";
			print_2d_arr(term_frequency_in_all_documents, 11);
			write_history(arr);
			cout << "\n\nIf you want to continue press 1 or press 0 to exit:\n";
			cin >> x;
			if (x == 1)
				x = 2;
		}
	}
	if (x == 0)
	{
		char history[150][150] = { '\0' };
		ifstream fin("history.txt");
		if (fin.is_open())
		{
			for (int i = 0; !fin.eof(); i++)
			{
				fin >> history[i];
			}
		}
		cout << "=================\n";
		cout << "   Your History  \n";
		cout << "=================\n";
		int a = 0;
		for (int i = 0; history[i][0] != '\0';a++, i++)
		{
			if (a != 2)
			{
				cout << history[i]<<"\t\t";
			}
			else
			{
				cout << history[i] ;
				a = -1;
				cout << endl;
			}
		}
	}
	return 0;
}