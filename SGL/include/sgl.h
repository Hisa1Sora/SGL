#ifndef SGL_H
#define SGL_H

#include <sgl_window.h>

namespace sgl {

	class SGL {
	public:
		SGL() {}
		~SGL() {}

		void execute() {
			auto win = sgl::SGLWindow(1280, 720, "myWindow");

			win.open();
			win.draw();
			win.close();
		}
	};

}

#endif // !SGL_H
