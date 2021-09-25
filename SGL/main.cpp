#include <sgl_window.h>

int main() {

	auto win = sgl::SGLWindow(1280, 720, "myWindow");

	win.execute();

	return 0;

}