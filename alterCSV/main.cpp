#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cctype>
#include <unordered_set>
#include "Graph.h"

void getData(Graph &data)
{
	std::string fileName = "Dataset/SpotifyFeatures.csv";
	std::fstream file(fileName, std::ios::in);
	string line, word;
	vector<string> row;
	float score = 0.0;
	bool accousticness = false;
	int count = 0;
	int track = 0;
	bool inQuotes = false;

	if (file.is_open())
	{
		getline(file, line);

		std::unordered_set<std::string> ids;

		while (getline(file, line))
		{
			row.clear();
			track = 0;
			word = line;

			// parse input
			while (true)
			{
				// Check if search is at the end of the line
				if (track > word.length() - 1)
				{
					break;
				}
				// Check if the word starts with a " which means it potentially has
				// specical chars
				if (!inQuotes && word.at(0) == '\"')
				{
					inQuotes = true;
					word = word.substr(1);
				}
				// Checks for word delimeter
				if (word.at(track) == ',' && !isspace(word.at(track + 1)))
				{
					if (inQuotes)
					{
						if (word.at(track - 1) == '\"')
						{
							inQuotes = false;
						}
					}
					if (!inQuotes)
					{
						if (word.at(track - 1) == '\"')
						{
							row.push_back(word.substr(0, track - 1));
						}
						else
						{
							row.push_back(word.substr(0, track));
						}
						word = word.substr(track + 1);
						track = 0;
					}
				}
				track++;
			}
			row.push_back(word);

			// watch out for duplicates
			if (ids.insert(row[3]).second == false)
				continue;
			;

			// Index 0  is genre, 1 is artist name, 2 is song title, 5 is accoustiness
			// 6 is dancebility, 8 is energy, and 17 is valence
			if (count % 1000 == 0)
			{
				cout << count << endl;
			}

			count++;

			score = (stof(row[6]) + stof(row[8]) + stof(row[17])) / 3.0;

			accousticness = false;
			if (stof(row[5]) > .8)
			{
				accousticness = true;
			}

			// add node to graph
			data.addNode(row[2], row[1], row[0], score, accousticness, row[3]);
		}
	}
	else
		cout << "didnt open" << endl;
	file.close();
}

int main()
{
	Graph data;
	getData(data);
	data.writeCSV();
}
