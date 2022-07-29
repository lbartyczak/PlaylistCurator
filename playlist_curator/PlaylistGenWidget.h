#pragma once

#include <Wt/WBreak.h>
#include <Wt/WServer.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WWidget.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WString.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WMenuItem.h>
#include <Wt/WComboBox.h>
#include <Wt/WMenu.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTable.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cctype>
#include <ctime>
#include <codecvt>
#include <locale>
#include <string>

#include "Graph.h"

class PlaylistGenWidget : public Wt::WContainerWidget
{
public:
	PlaylistGenWidget();
private:
	Wt::WContainerWidget* homePage;
	Wt::WContainerWidget* playlistPage;
	Wt::WText* title;
	Wt::WText* info;
	Wt::WContainerWidget* playlistDijk;
	Wt::WContainerWidget* playlistBFS;
	Wt::WPushButton* generateButton;
	Wt::WPushButton* newButton;
	Wt::WLineEdit* songPrompt1;
	Wt::WLineEdit* songPrompt2;
	Wt::WTabWidget* menu;
	Wt::WComboBox* numbers;

	Graph data;

	void handleInternalPath();
	void showPlaylist();
	void getData(Graph& data);
};

