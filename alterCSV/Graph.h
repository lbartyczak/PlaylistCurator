#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <utility>
#include <stack>
#include <queue>
#include <fstream>
using namespace std;

class Graph
{

public:
    struct Node
    {
        string trackName;
        string artist;
        string genre;
        float score;
        bool accousticness;
        string id;
        vector<int> neighbors;
        Node(){};
    };

    Graph(){};

    void writeCSV()
    {
        ofstream file;
        file.open("output/NodeData-reg_tolerance.csv", ios::out | ios::binary);

        for (auto iter : graph)
        {
            file << iter->genre << "||";
            file << iter->trackName << "||";
            file << iter->artist << "||";
            file << iter->id << "||";
            file << iter->score << "||";
            for (auto num : iter->neighbors)
                file << num << " ";
            file << endl;
        }

        file.close();
    }

    void addNode(string trackName, string artist, string genre, float score, bool accousticness, string id)
    {
        Node *n = new Node;
        n->trackName = trackName;
        n->artist = artist;
        n->genre = genre;
        n->score = score;
        n->accousticness = accousticness;
        n->id = id;
        int similarGenre = 0;

        for (int i = 0; i < graph.size(); i++)
        {
            float tolerance = 0.0;
            similarGenre = compareGenres(genre, graph.at(i)->genre);

            if (similarGenre > 0)
            {
                if (similarGenre == 2)
                    tolerance = .001;
                if (similarGenre == 1)
                    tolerance = .0005;

                if (accousticness == graph.at(i)->accousticness)
                {
                    tolerance += .0001;
                }
                else
                {
                    tolerance -= .0001;
                }

                if (abs(graph.at(i)->score - score) <= tolerance)
                {
                    graph.at(i)->neighbors.push_back(graph.size());
                    n->neighbors.push_back(i);
                }
            }
        }

        graph.push_back(n);
    }

    int compareGenres(string genre1, string genre2)
    {
        // Rap
        if (genre1 == "Rap")
        {
            if (genre2 == "Rap" || genre2 == "Hip-Hop")
            {
                return 2;
            }
            else if (genre2 == "R&B" || genre2 == "Pop")
            {
                return 1;
            }
            return 0;
        }
        // Hip-Hop
        else if (genre1 == "Hip-Hop")
        {
            if (genre2 == "Rap" || genre2 == "Hip-Hop")
            {
                return 2;
            }
            else if (genre2 == "R&B" || genre2 == "Pop")
            {
                return 1;
            }
            return 0;
        }
        // R&B
        else if (genre1 == "R&B")
        {
            if (genre2 == "R&B")
            {
                return 2;
            }
            else if (genre2 == "Hip-Hop" || genre2 == "Rap" || genre2 == "Soul" || genre2 == "Blues")
            {
                return 1;
            }
            return 0;
        }
        // Soul
        else if (genre1 == "Soul")
        {
            if (genre2 == "Soul" || genre2 == "Blues")
            {
                return 2;
            }
            else if (genre2 == "R&B" || genre2 == "Jazz" || genre2 == "Country" || genre2 == "Folk")
            {
                return 1;
            }
            return 0;
        }
        // Blues
        else if (genre1 == "Blues")
        {
            if (genre2 == "Blues" || genre2 == "Soul")
            {
                return 2;
            }
            else if (genre2 == "R&B" || genre2 == "Jazz" || genre2 == "Country" || genre2 == "Folk")
            {
                return 1;
            }
            return 0;
        }
        // Jazz
        else if (genre1 == "Jazz")
        {
            if (genre2 == "Jazz")
            {
                return 2;
            }
            else if (genre2 == "Reggae" || genre2 == "Reggaeton" || genre2 == "Ska" || genre2 == "Soul" || genre2 == "Blues")
            {
                return 1;
            }
            return 0;
        }
        // Reggae
        else if (genre1 == "Reggae")
        {
            if (genre2 == "Reggae" || genre2 == "Reggaeton" || genre2 == "Ska")
            {
                return 2;
            }
            else if (genre2 == "Jazz" || genre2 == "World")
            {
                return 1;
            }
            return 0;
        }
        // Reggaeton
        else if (genre1 == "Reggaeton")
        {
            if (genre2 == "Reggae" || genre2 == "Reggaeton" || genre2 == "Ska")
            {
                return 2;
            }
            else if (genre2 == "Jazz" || genre2 == "World")
            {
                return 1;
            }
            return 0;
        }
        // Ska
        else if (genre1 == "Ska")
        {
            if (genre2 == "Reggae" || genre2 == "Reggaeton" || genre2 == "Ska")
            {
                return 2;
            }
            else if (genre2 == "Jazz" || genre2 == "World")
            {
                return 1;
            }
            return 0;
        }
        // World
        else if (genre1 == "World")
        {
            if (genre2 == "World")
            {
                return 2;
            }
            else if (genre2 == "Reggae" || genre2 == "Reggaeton" || genre2 == "Ska")
            {
                return 1;
            }
            return 0;
        }
        // Pop
        else if (genre1 == "Pop")
        {
            if (genre2 == "Pop")
            {
                return 2;
            }
            else if (genre2 == "Hip-Hop" || genre2 == "Rap" || genre2 == "Dance" || genre2 == "Electronic" || genre2 == "A Capella" || genre2 == "Indie" || genre2 == "Alternative")
            {
                return 1;
            }
            return 0;
        }
        // Dance
        else if (genre1 == "Dance")
        {
            if (genre2 == "Dance" || genre2 == "Electronic")
            {
                return 2;
            }
            else if (genre2 == "Pop")
            {
                return 1;
            }
            return 0;
        }
        // Electronic
        else if (genre1 == "Electronic")
        {
            if (genre2 == "Electronic" || genre2 == "Dance")
            {
                return 2;
            }
            else if (genre2 == "Pop")
            {
                return 1;
            }
            return 0;
        }
        // Indie
        else if (genre1 == "Indie")
        {
            if (genre2 == "Indie" || genre2 == "Alternative")
            {
                return 2;
            }
            else if (genre2 == "Pop" || genre2 == "Rock")
            {
                return 1;
            }
            return 0;
        }
        // Alt
        else if (genre1 == "Alternative")
        {
            if (genre2 == "Indie" || genre2 == "Alternative")
            {
                return 2;
            }
            else if (genre2 == "Pop" || genre2 == "Rock")
            {
                return 1;
            }
            return 0;
        }
        // Rock
        else if (genre1 == "Rock")
        {
            if (genre2 == "Rock")
            {
                return 2;
            }
            else if (genre2 == "Alternative" || genre2 == "Indie" || genre2 == "Folk" || genre2 == "Country")
            {
                return 1;
            }
            return 0;
        }
        // Folk
        else if (genre1 == "Folk")
        {
            if (genre2 == "Folk" || genre2 == "Country")
            {
                return 2;
            }
            else if (genre2 == "Rock" || genre2 == "Soul" || genre2 == "Blues")
            {
                return 1;
            }
            return 0;
        }
        // Country
        else if (genre1 == "Country")
        {
            if (genre2 == "Country" || genre2 == "Folk")
            {
                return 2;
            }
            else if (genre2 == "Rock" || genre2 == "Soul" || genre2 == "Blues")
            {
                return 1;
            }
            return 0;
        }
        // Soundtrack
        else if (genre1 == "Soundtrack")
        {
            if (genre2 == "Soundtrack")
            {
                return 2;
            }
            else if (genre2 == "Children's Music" || genre2 == "Comedy" || genre2 == "Movie" || genre2 == "Classical")
            {
                return 1;
            }
            return 0;
        }
        // Children's Music
        else if (genre1 == "Children's Music")
        {
            if (genre2 == "Children's Music")
            {
                return 2;
            }
            else if (genre2 == "Soundtrack")
            {
                return 1;
            }
            return 0;
        }
        // Comedy
        else if (genre1 == "Comedy")
        {
            if (genre2 == "Comedy")
            {
                return 2;
            }
            else if (genre2 == "Soundtrack")
            {
                return 1;
            }
            return 0;
        }
        // Movie
        else if (genre1 == "Movie")
        {
            if (genre2 == "Movie")
            {
                return 2;
            }
            else if (genre2 == "Soundtrack" || genre2 == "Anime")
            {
                return 1;
            }
            return 0;
        }
        // Anime
        else if (genre1 == "Anime")
        {
            if (genre2 == "Anime")
            {
                return 2;
            }
            else if (genre2 == "Movie")
            {
                return 1;
            }
            return 0;
        }
        // Classical
        else if (genre1 == "Classical")
        {
            if (genre2 == "Classical")
            {
                return 2;
            }
            else if (genre2 == "Opera" || genre2 == "Soundtrack")
            {
                return 1;
            }
            return 0;
        }
        // Opera
        else if (genre1 == "Opera")
        {
            if (genre2 == "Opera")
            {
                return 2;
            }
            else if (genre2 == "A Capella" || genre2 == "Classical")
            {
                return 1;
            }
            return 0;
        }
        // A Capella
        else if (genre1 == "A Capella")
        {
            if (genre2 == "A Capella")
            {
                return 2;
            }
            else if (genre2 == "Opera" || genre2 == "Pop")
            {
                return 1;
            }
            return 0;
        }
        return 0;
    }

private:
    vector<Node *> graph;
};