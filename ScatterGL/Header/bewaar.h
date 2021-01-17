#pragma once

////activating shader
materialShader.use();

materialShader.setMat4("projection", projection);
materialShader.setMat4("view", view);

//setting surface test things
materialShader.setInt("material.diffuse", 0);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, boxTexture.texture);
materialShader.setInt("material.specular", 1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularBoxTexture.texture);
materialShader.setVec3("material.ambient", surface.material.ambient);
materialShader.setFloat("material.shine", surface.material.shine);

materialShader.setVec3("light.direction", sunLight.direction);
materialShader.setVec3("light.ambient", sunLight.ambient);
materialShader.setVec3("light.diffuse", sunLight.diffuse);
materialShader.setVec3("light.specular", sunLight.specular);
materialShader.setVec3("viewPosition", camera.position);
glm::mat4 testModel = glm::mat4(1.0f);
testModel = glm::translate(testModel, glm::vec3(0.0f, 0.0f, 0.0f));
testModel = glm::scale(testModel, glm::vec3(10.0f, 1.0f, 10.0f));
materialShader.setMat4("model", testModel);
surface.drawObject();
glBindTexture(GL_TEXTURE_2D, 0);

//setting material properties
materialShader.setInt("material.diffuse", 0);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, woodTexture.texture);
materialShader.setVec3("material.ambient", cubeObject.material.ambient);
materialShader.setFloat("material.shine", cubeObject.material.shine);

materialShader.setVec3("light.position", sunLight.direction);
materialShader.setVec3("light.ambient", sunLight.ambient);
materialShader.setVec3("light.diffuse", sunLight.diffuse);
materialShader.setVec3("light.specular", sunLight.specular);

materialShader.setVec3("viewPosition", camera.position);

//view & projection matrix calculations
glm::mat4 model = glm::mat4(1.0f);
materialShader.setMat4("model", model);
cubeObject.drawObject();



//draw lamp object
naturalLightShader.use();
naturalLightShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
naturalLightShader.setMat4("projection", projection);
naturalLightShader.setMat4("view", view);
model = glm::mat4(1.0f);
model = glm::translate(model, sunLight.direction);
model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
naturalLightShader.setMat4("model", model);
cubeObject.drawObject();

testShader.use();
testShader.setInt("material.diffuse", 0);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, boxTexture.texture);
testShader.setInt("material.specular", 1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, specularBoxTexture.texture);
testShader.setVec3("material.ambient", surface.material.ambient);
testShader.setFloat("material.shine", surface.material.shine);

testShader.setVec3("light.direction", sunLight.direction);
testShader.setVec3("light.ambient", sunLight.ambient);
testShader.setVec3("light.diffuse", sunLight.diffuse);
testShader.setVec3("light.specular", sunLight.specular);
testShader.setVec3("viewPosition", camera.position);
int viewLoc = glGetUniformLocation(testShader.ID, "view");
int projectionLoc = glGetUniformLocation(testShader.ID, "projection");
testShader.setMat4("projection", projection);
glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
testShader.setMat4("view", view);
glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


for (unsigned int i = 0; i < 5; i++)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cubePositions[i]);
	float angle = 20.0f * i;
	angle = glfwGetTime() * i * 20.0f;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	testShader.setMat4("model", model);
	cubeObject.drawObject();
}