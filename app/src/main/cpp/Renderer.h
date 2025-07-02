#ifndef ANDROIDGLINVESTIGATIONS_RENDERER_H
#define ANDROIDGLINVESTIGATIONS_RENDERER_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <memory>

#include "Model.h"
#include "Shader.h"
#include "TextureAsset.h"

struct android_app;

class Renderer {
public:
    /*!
     * @param pApp the android_app this Renderer belongs to, needed to configure GL
     */
    inline Renderer(android_app *pApp) :
            app_(pApp),
            display_(EGL_NO_DISPLAY),
            surface_(EGL_NO_SURFACE),
            context_(EGL_NO_CONTEXT),
            width_(0),
            height_(0),
            shaderNeedsNewProjectionMatrix_(true) {
        initRenderer();
    }

    virtual ~Renderer();

    /*!
     * Handles input from the android_app.
     *
     * Note: this will clear the input queue
     */
    void handleInput();

    /*!
     * Renders all the models in the renderer
     */
    void render();

    bool init(AAssetManager* assetManager);
    void setViewport(int width, int height);
    void render(GameModel& model);
    void handleTouch(float x, float y);

private:
    /*!
     * Performs necessary OpenGL initialization. Customize this if you want to change your EGL
     * context or application-wide settings.
     */
    void initRenderer();

    /*!
     * @brief we have to check every frame to see if the framebuffer has changed in size. If it has,
     * update the viewport accordingly
     */
    void updateRenderArea();

    /*!
     * Creates the models for this sample. You'd likely load a scene configuration from a file or
     * use some other setup logic in your full game.
     */
    void createModels();

    void renderBox(const GameObject& box);
    void renderTarget(const GameObject& target);
    void renderUI();
    void renderParticle(const Particle& particle, const std::string& textureName);

    android_app *app_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLint width_;
    EGLint height_;

    bool shaderNeedsNewProjectionMatrix_;

    Shader* shader;
    TextureAsset* boxTexture;
    TextureAsset* targetTexture;

    int screenWidth;
    int screenHeight;
    float projectionMatrix[16];

    std::unique_ptr<Shader> shader_;
    std::vector<Model> models_;
};

#endif //ANDROIDGLINVESTIGATIONS_RENDERER_H