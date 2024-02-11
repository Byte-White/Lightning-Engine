#define MAGMA_ENTRYPOINT
#include "Magma/Magma.h"
#include "Engine.h"

mg::Application* CreateApplication(int argc, char** argv)
{
	LightningEngine* app = new LightningEngine();
	app->SetTitle("Lightning Engine");
	app->SetSize(1600, 800);
	return app;
}
