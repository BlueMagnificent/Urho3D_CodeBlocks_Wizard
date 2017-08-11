#ifndef [PROJECT_HDR]_H
#define [PROJECT_HDR]_H


enum CameraPosition
{
    CAM_FRONT = 100,
    CAM_BACK,
    CAM_RIGHT,
    CAM_LEFT,
    CAM_UP,
    CAM_DOWN
};

using namespace Urho3D;

class [CLASS_PREFIX] : public Urho3D::Application
{
	URHO3D_OBJECT([CLASS_PREFIX], Application)
	
public:
    [CLASS_PREFIX](Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();

    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

private:
    void CreateScene();
    void CreateInstructionTexts();
    void SetupViewport();

    void SetupOrbitingCam();
    void UpdateOrbitingCam();
    void SetCameraPosition(CameraPosition pos);


    void OnKeyPressed(int key);

    void SubscribeToEvents();

    SharedPtr<Scene> scene_;

    SharedPtr<Node> cameraNode_;
    SharedPtr<Node> camPitchNode_;
    SharedPtr<Node> camYawNode_;
    SharedPtr<Node> camBaseNode_;

    SharedPtr<Text> instructionText_;
};

#endif // [PROJECT_HDR]_H