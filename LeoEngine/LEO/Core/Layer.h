#pragma once

namespace LEO
{
    /// Base class for layers in the application
    class Layer
    {
	public:
		/// Called when the Layer is created, called only onces.	
		virtual void OnCreate() {}
		/// Called when the Layer is deleted, called only onces.	
		virtual void OnDestroy() {}
	
		/// Called every frame
		virtual void OnUpdate() {}
		/// Called every frame
		virtual void OnRender() {}
        /// Called every frame when scene is activated, runs ImGui render code
        virtual void OnRenderImGui() {}
    public:
		virtual ~Layer() = default;
    };
}
