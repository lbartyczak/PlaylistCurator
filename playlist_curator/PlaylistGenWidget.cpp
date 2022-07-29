#include "PlaylistGenWidget.h"

PlaylistGenWidget::PlaylistGenWidget() {
	auto* app = Wt::WApplication::instance();

	getData(data);

	setHeight(500);

	this->setStyleClass("page");

	//HOMEPAGE
	homePage = addNew<Wt::WContainerWidget>();
	homePage->setStyleClass("home-page");
    {
        //title
        title = homePage->addWidget(std::make_unique<Wt::WText>("Playlist Curator"));
        title->setStyleClass("title");
        homePage->addNew<Wt::WBreak>();

        //get first song
        songPrompt1 = homePage->addWidget(std::make_unique<Wt::WLineEdit>());
        songPrompt1->setPlaceholderText("Enter the first song");
        songPrompt1->setStyleClass("input1");
        homePage->addNew<Wt::WBreak>();

        //get second song
        songPrompt2 = homePage->addWidget(std::make_unique<Wt::WLineEdit>());
        songPrompt2->setPlaceholderText("Enter the first song");
        songPrompt2->setStyleClass("input2");
        homePage->addNew<Wt::WBreak>();

        //number of songs displayed
        auto* container = homePage->addWidget(std::make_unique<Wt::WContainerWidget>());
        container->addWidget(std::make_unique<Wt::WText>("Max number of songs: "));
        numbers = container->addWidget(std::make_unique<Wt::WComboBox>());
        container->setMargin(13, Wt::Side::Top);
        numbers->addItem("10");
        numbers->addItem("25");
        numbers->addItem("50");
        numbers->addItem("100");
        numbers->addItem("All");
        numbers->setCurrentIndex(4);
        numbers->setStyleClass("count");

        //generate button
        generateButton = homePage->addWidget(std::make_unique<Wt::WPushButton>("Generate"));
        generateButton->setStyleClass("button");
        generateButton->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/curator"));
    }

    //PLAYLISTPAGE
    playlistPage = addNew<Wt::WContainerWidget>();
    playlistPage->setStyleClass("playlist-page");
    {
        //info bar
        auto* infoBar = playlistPage->addWidget(std::make_unique<Wt::WContainerWidget>());
        infoBar->setStyleClass("info-bar");
        {
            //playlist and algorithm info
            info = infoBar->addWidget(std::make_unique<Wt::WText>());
            info->setStyleClass("info");
            infoBar->addNew<Wt::WBreak>();

            //new playlist button
            newButton = infoBar->addWidget(std::make_unique<Wt::WPushButton>("New Playlist"));
            newButton->setStyleClass("button");
            newButton->setLink(Wt::WLink(Wt::LinkType::InternalPath, ""));
        }

        //playlist tabs
        menu = playlistPage->addWidget(std::make_unique<Wt::WTabWidget>());
        menu->setStyleClass("tab-bar");

        playlistDijk = addWidget(std::make_unique<Wt::WContainerWidget>());
        playlistDijk->setOverflow(Wt::Overflow::Auto);
        playlistDijk->setStyleClass("tab-child");
        menu->addTab(std::unique_ptr<Wt::WContainerWidget>(playlistDijk), "Dijkstra", 
            Wt::ContentLoading::Eager)->setStyleClass("tab-label");

        playlistBFS = addWidget(std::make_unique<Wt::WContainerWidget>());
        playlistBFS->setOverflow(Wt::Overflow::Auto);
        playlistBFS->setStyleClass("tab-child");
        menu->addTab(std::unique_ptr<Wt::WContainerWidget>(playlistBFS), "BFS", 
            Wt::ContentLoading::Eager)->setStyleClass("tab-label-b");
    }

    playlistPage->hide();

    menu->currentChanged().connect([=] {
        menu->itemAt(0)->setStyleClass("tab-label-b");
        menu->itemAt(1)->setStyleClass("tab-label-b");
        menu->currentItem()->setStyleClass("tab-label");
        });
	
	songPrompt1->enterPressed().connect([=] {
		songPrompt2->setFocus();
		});
	songPrompt2->enterPressed().connect([=] {
		generateButton->setFocus();
		});

    app->internalPathChanged().connect([=] {
        handleInternalPath();
        });
}

void PlaylistGenWidget::handleInternalPath() {
    auto* app = Wt::WApplication::instance();

    if (app->internalPath() == "/curator")
    {
        homePage->hide();
        showPlaylist();
    }
    else {
        playlistPage->hide();
        homePage->show();
        songPrompt1->setText("");
        songPrompt2->setText("");
        playlistBFS->clear();
        playlistDijk->clear();
        menu->setCurrentIndex(0);
    }
}

void PlaylistGenWidget::showPlaylist() {
    playlistPage->show();

    //set up converter for strings
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //convert strings
    std::string song1 = converter.to_bytes(songPrompt1->text().value());
    std::string song2 = converter.to_bytes(songPrompt2->text().value());
    std::string num = converter.to_bytes(numbers->currentText());
    int numb = 1;
    if (num != "All") numb = stoi(num);

    //get song data
    Graph::Node* a = data.getNodeID(song1);
    Graph::Node* b = data.getNodeID(song2);

    auto t1 = time(NULL);
    vector<Graph::Node*> dijk = data.dijkstra(a, b);
    auto t2 = time(NULL);

    auto t3 = time(NULL);
    vector<Graph::Node*> bfs = data.bfs(a, b);
    auto t4 = time(NULL);

    float numSongs = 1;
    if (numb != 1 && numb < dijk.size()) numSongs = dijk.size() / (static_cast<unsigned long long>(numb));  

    if (dijk.size() > 1) {
        int count = numb;
        if (numSongs == 1) count = dijk.size();
        auto* cont = playlistDijk->addWidget(std::make_unique<Wt::WContainerWidget>());
        cont->setStyleClass("tab-content");
        cont->addWidget(std::make_unique<Wt::WText>(dijk[dijk.size()-1]->trackName + " by " + dijk[dijk.size()-1]->artist));
        for (int i = count-2; i >= 0; i--) {
            auto* cont = playlistDijk->addWidget(std::make_unique<Wt::WContainerWidget>());
            cont->setStyleClass("tab-content");
            cont->addWidget(std::make_unique<Wt::WText>(dijk[i*numSongs]->trackName + " by " + dijk[i*numSongs]->artist));
        }
    }
    else {
        auto* cont = playlistDijk->addWidget(std::make_unique<Wt::WText>("Not Connected : ("));
    }

    numSongs = 1;
    if (numb != 1 && numb < bfs.size()) numSongs = (bfs.size() / (static_cast<unsigned long long>(numb)));

    if (bfs.size() > 1) {
        if (numSongs == 1) numb = bfs.size();
        for (int i = 0; i < numb - 1; i++) {
            auto* cont = playlistBFS->addWidget(std::make_unique<Wt::WContainerWidget>());
            cont->setStyleClass("tab-content");
            cont->addWidget(std::make_unique<Wt::WText>(bfs[i*numSongs]->trackName + " by " + bfs[i*numSongs]->artist));           
        }
        auto* cont = playlistBFS->addWidget(std::make_unique<Wt::WContainerWidget>());
        cont->setStyleClass("tab-content");
        cont->addWidget(std::make_unique<Wt::WText>(bfs[bfs.size()-1]->trackName + " by " + bfs[bfs.size()-1]->artist));
    }
    else {
        auto* cont = playlistDijk->addWidget(std::make_unique<Wt::WText>("Not Connected : ("));
    }

    //set info bar text
    info->setText(Wt::WString("You chose {1} and {2}! Dijkstras took {3} seconds and found {4} songs, BFS took {5} seconds and found {6} songs")
        .arg(songPrompt1->text()).arg(songPrompt2->text()).arg(t2 - t1).arg(dijk.size()).arg(t4 - t3).arg(bfs.size()));

}

void PlaylistGenWidget::getData(Graph& data) {
    std::string fileName = "Dataset/NodeData-reg_tolerance.csv";
    std::fstream file(fileName, std::ios::in);
    string line, word;
    vector<string> row;
    float score = 0.0;
    //bool accousticness = false;
    int count = 0;
    int track = 0;
    //bool inQuotes = false;

    if (file.is_open())
    {
        while (getline(file, line)) {
            row.clear();
            track = 0;
            word = line;

            while (true) {
                //Check if search is at the end of the line
                if (track > word.length() - 1) {
                    break;
                }

                //Checks for word delimeter
                if (word.at(track) == '|' && word.at(track + 1) == '|')
                {
                    row.push_back(word.substr(0, track));

                    if (word.substr(track) == "||") {
                        word = "";
                        break;
                    }

                    word = word.substr(track + 2);
                    track = 0;
                }

                track++;
            }
            row.push_back(word);

            //NEW: index 0 is genre, 1 is trackname, 2 is artist, 3 is ID, 4 is score, 5 is vector of neighbors
            count++;
            score = (stof(row[4]));
            string vec = row[5];
            vector<int> neigh;

            while (vec.length() > 0)
            {
                int index = vec.find(' ');
                neigh.push_back(stoi(vec.substr(0, index)));
                if (vec.find(' ') == vec.length() - 1) break;
                vec = vec.substr(index + 1);
            }

            data.addNode(row[1], row[2], row[0], score, neigh, row[3]);
        }
    }
}