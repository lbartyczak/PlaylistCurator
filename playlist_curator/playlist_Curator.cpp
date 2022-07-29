#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WServer.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WCssDecorationStyle.h>

#include "PlaylistGenWidget.h"

int main(int argc, char* argv[])
{
	return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
		auto app = std::make_unique<Wt::WApplication>(env);

		auto root = app->root();

		app->useStyleSheet("styleSheet.css");

		PlaylistGenWidget* mainPg = root->addNew<PlaylistGenWidget>();


		return app;

		});
}