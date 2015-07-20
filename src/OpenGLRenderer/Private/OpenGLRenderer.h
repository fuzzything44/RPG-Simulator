#pragma once
#include "Renderer.h"

#include <list>
#include <vector>
#include <unordered_map>
#include <thread>
#include <future>
#include <functional>

#include <boost/lockfree/spsc_queue.hpp>

class OpenGLModel;
class OpenGLMaterialInstance;
class OpenGLTexture;
class OpenGLWindow;
class OpenGLMaterialSource;
class OpenGLModelData;

class OpenGLRenderer : public Renderer
{

	friend class OpenGLModel;


	struct RenderThread
	{
		RenderThread(std::thread&& thread) : isInLoop(true), thread(std::move(thread)) {}
		RenderThread() : isInLoop(true) {}

		const RenderThread& operator=(std::thread&& rightThread) { thread = std::move(rightThread); return *this; };

		operator std::thread&(){ return thread; }

		std::thread& getThread() { return thread; }

		~RenderThread()
		{
			isInLoop = false;
			if (thread.joinable())
				thread.join();
		}

		std::atomic<bool> isInLoop;
	private:
		std::thread thread;
	};


public:


	OpenGLRenderer();

	virtual ~OpenGLRenderer() override;

	virtual void init() override;

	void showLoadingImage();

	virtual Window& getWindow() override;
	const Window& getWindow() const override;

	virtual std::unique_ptr<Model> newModel() override;
	virtual std::shared_ptr<Texture> getTexture(const path_t& name) override;
	virtual std::shared_ptr<MaterialSource> getMaterialSource(const path_t& name) override;
	virtual std::unique_ptr<TextureLibrary> newTextureLibrary() override;
	virtual std::unique_ptr<MaterialInstance> newMaterial(std::shared_ptr<MaterialSource> source) override;
	virtual std::shared_ptr<ModelData> newModelData(const std::string& name) override;
	virtual std::unique_ptr<ModelData> newModelData() override;

	/// <summary> Renders the next frame. </summary>
	virtual bool update(float deltaTime) override;

	/// <summary> Sets camera to render at. </summary>
	///
	/// <param name="newCamera"> The camera it should render at. </param>
	virtual void setCurrentCamera(CameraComponent& newCamera) override;


	CameraComponent& getCurrentCamera() override;
	const CameraComponent& getCurrentCamera() const override;

	virtual void drawDebugOutlinePolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugLine(vec2* locs, uint32 numLocs, Color color) override;
	virtual void drawDebugSolidPolygon(vec2* verts, uint32 numVerts, Color color) override;
	virtual void drawDebugOutlineCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSolidCircle(vec2 center, float radius, Color color) override;
	virtual void drawDebugSegment(vec2 p1, vec2 p2, Color color) override;

	template<typename Function>
	inline auto runOnRenderThreadSync(Function& func)->decltype(func());

	inline bool isOnRenderThread() { return std::this_thread::get_id() == renderThread.getThread().get_id(); }

private:

	void initRenderer();
	void renderLoop();


	boost::lockfree::spsc_queue<std::function<void()> > queue;
	RenderThread renderThread;

	std::unique_ptr<OpenGLWindow> window;
	std::unique_ptr<OpenGLMaterialInstance> debugDraw;

	// then delete our atomics
	std::atomic<CameraComponent*> currentCamera;
	std::future<void> lastFrame;
	std::promise<void> tickPromise;
	std::atomic<bool> shouldExit;

	// delete our caches and models first
	std::list<OpenGLModel*> models;
	std::unordered_map<path_t, std::weak_ptr<OpenGLTexture> > textures;
	std::unordered_map<path_t, std::weak_ptr<OpenGLMaterialSource> > matSources;
	std::unordered_map<std::string, std::weak_ptr<OpenGLModelData>> modelDataCache;

};

template<typename Function>
inline auto OpenGLRenderer::runOnRenderThreadSync(Function& func) -> decltype(func())
{
	if (isOnRenderThread())
	{
		return func();
	}

	using retType = decltype(func());

	std::packaged_task<retType()> pack{ func };

	queue.push([&pack]
	{
		pack();
	});


	return pack.get_future().get();
}


