#include "Framework.h"

int main()
{
	Framework* framework = new Framework();

	if (framework != nullptr)
	{
		const int width = 480;
		const int height = 320;
		const char* name = "Hello Library";

		if (framework->Initialize(name, width, height))
		{
			framework->Update();
			framework->Destory();
		}

		delete framework;
		framework = nullptr;
	}
}