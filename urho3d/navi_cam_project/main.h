#ifndef [PROJECT_HDR]_H
#define [PROJECT_HDR]_H


using namespace Urho3D;

class [CLASS_PREFIX] : public Application
{

	URHO3D_OBJECT([CLASS_PREFIX], Application)
	
public:
    [CLASS_PREFIX](Urho3D::Context* context);
	
	/// Setup before engine initialization. Modifies the engine parameters.
    virtual void Setup();
    /// Setup after engine initialization.
    virtual void Start();

private:
	/// Construct the scene content.
	void CreateScene();
	/// Set up a viewport for displaying the scene.
    void SetupViewport();
    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
	/// Handle keydown event
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	/// Read input and moves the camera.
    void MoveCamera(float timeStep);
	
	/// Scene
    SharedPtr<Scene> scene_;
	/// Camera Scene Node
    SharedPtr<Node> cameraNode_;
	
	/// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;
};

#endif // [PROJECT_HDR]_H