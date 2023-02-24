#include <iostream>
#include <fstream>
#include <sstream>
#include "scene.h"
#include "ppm.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "argument count error" << endl;
		return -1;
	}

	Scene scene = Scene::parse(argv[1]);
	unsigned char* data = scene.tracing();
	save_imageP6(scene.resC, scene.resR, scene.output.c_str(), data);
	return 0;
}