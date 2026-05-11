#include "Scene.h"

#include <stb_image.h>

#include <memory>
#include <tracy/Tracy.hpp>

#include "CameraManager.h"
#include "Graphics.h"
#include "GraphicsCommunicationManager.h"
#include "ModelReader.h"
#include "Quaternion.h"
#include "StaticModelData.h"
#include "StaticProjectionCamera.h"
#include "Vector3.h"

StaticModelData model_data = {};

Scene::Scene() {
    auto cam = std::make_shared<StaticProjectionCamera>(60.f, (float)4 / 3,
                                                        0.1f, 1000.f);
    CameraManager::get().setMainCamera(cam);

    File::ModelReader reader("./Assets/Gem.fbx");
    auto vertices = reader.readVertices();
    auto indices = reader.readIndices();

    auto renderer = Graphics::getRenderEngine();
    model_data.position = {0, 0, 10};
    model_data.mesh =
        renderer->addMesh(vertices.data(), vertices.size() * sizeof(Vertex),
                          indices.data(), indices.size() * sizeof(uint32_t));

    int width;
    int height;
    int channels;
    unsigned char* data =
        stbi_load("./Assets/Gem.png", &width, &height, &channels, 0);

    model_data.albedo = renderer->addTexture(data, width, height);
}

void Scene::update(float deltaTime) {
    ZoneScoped;

    sky.draw();
    world.update(deltaTime);

    GraphicsCommunicationManager::get().send(model_data);
}

void Scene::setupSystems() {}
