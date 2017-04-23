#include "App.h"
#include <iostream>

namespace basicgraphics{

using namespace std;
using namespace glm;

App::App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight) {

    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

	sphere.reset(new Sphere(vec3(0), 1.0, vec4(1.0)));
}

void App::onEvent(shared_ptr<Event> event) {
	if (event->getName() == "mouse_pointer") {
		mousePos = vec2(event->get2DData());
	}
}

vec3 App::mousePosToRay(mat4 view, mat4 projection) {
	vec2 normalizedDeviceCoords((2 * mousePos.x) / (float)_windowWidth - 1,
							  -((2 * mousePos.y) / (float)_windowHeight - 1));
	vec4 rayPt = vec4(normalizedDeviceCoords, -1.0, 1.0);
	vec4 camCoords = inverse(projection) * rayPt;
	camCoords = vec4(camCoords.x, camCoords.y, -1.0, 0.0);

	vec3 worldDir = vec3(inverse(view) * camCoords);
	return normalize(worldDir);
}

void App::onRenderGraphics() {
    vec3 eye_world(-3,5,5);
    // Setup the camera with a good initial position and view direction to see the table
    glm::mat4 view = glm::lookAt(eye_world, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    
    // Setup the projection matrix so that things are rendered in perspective
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 100.0f);
    
    // Setup the model matrix
    glm::mat4 model(1.0);
    
    // Update shader variables
    _shader.setUniform("view_mat", view);
    _shader.setUniform("projection_mat", projection);
    _shader.setUniform("model_mat", model);
    _shader.setUniform("eye_world", eye_world);


	//Draw a sphere where the user's mouse is.
	//Credit: http://antongerdelan.net/opengl/raycasting.html

	//Generate ray
	vec3 worldRay = mousePosToRay(view, projection);

	//Calculate ray intesection with y=0 plane
	float t = -eye_world.y / worldRay.y;
	vec3 pt = eye_world + (worldRay * t);
	pt.y = 0;

	//Draw sphere at point
	sphere->draw(_shader, translate(mat4(1.0), pt) * scale(mat4(1.0), vec3(0.1)));
}
}//namespace



