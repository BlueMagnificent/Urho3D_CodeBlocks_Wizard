#include <Urho3D/Urho3D.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>

#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>

#include "[CLASS_PREFIX].h"


[CLASS_PREFIX]::[CLASS_PREFIX](Urho3D::Context *context) : Application(context)
{
}

void [CLASS_PREFIX]::Setup()
{
    if (!engineParameters_.Contains(EP_RESOURCE_PREFIX_PATHS))
       engineParameters_[EP_RESOURCE_PREFIX_PATHS] = GetSubsystem<FileSystem>()->GetCurrentDir();
       
    engineParameters_[EP_RESOURCE_PATHS] = "CoreData;Data";
    engineParameters_[EP_LOG_NAME]   = "[CLASS_PREFIX].log";
    engineParameters_[EP_FULL_SCREEN]  = false;
    engineParameters_[EP_HEADLESS]    = false;
}

void [CLASS_PREFIX]::Start()
{
    CreateScene();

    CreateInstructionTexts();

    SetupOrbitingCam();

    SetupViewport();

    SubscribeToEvents();
}

void [CLASS_PREFIX]::Stop()
{
}

void [CLASS_PREFIX]::HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    using namespace Urho3D::KeyDown;
    // Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
    int key = eventData[P_KEY].GetInt();
    if (key == Urho3D::KEY_ESCAPE)
        engine_->Exit();
    else if(key == Urho3D::KEY_I)
    {
        instructionText_->SetVisible(!instructionText_->IsVisible());
    }
    else
    {
        OnKeyPressed(key);
    }
}

void [CLASS_PREFIX]::HandleUpdate(StringHash eventType, VariantMap &eventData)
{
    UpdateOrbitingCam();
}

void [CLASS_PREFIX]::CreateScene()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    scene_ = new Scene(context_);

    scene_->CreateComponent<Octree>();

    Node* planeNode = scene_->CreateChild("Plane");
    planeNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));

    StaticModel* planeObject = planeNode->CreateComponent<StaticModel>();
    planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
    planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

    Node* modelNode = scene_->CreateChild("ModelNode");
    modelNode->SetScale(0.3f);

    StaticModel* model = modelNode->CreateComponent<StaticModel>();
    model->SetModel(cache->GetResource<Model>("Models/Mushroom.mdl"));
    model->SetMaterial(cache->GetResource<Material>("Materials/Mushroom.xml"));


    Node* lightNode = scene_->CreateChild("DirectionalLightNode");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));


    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
}


void [CLASS_PREFIX]::CreateInstructionTexts()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    Text* toggleText = ui->GetRoot()->CreateChild<Text>();
    toggleText->SetText("Press I to toggle instruction");

    toggleText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    toggleText->SetAlignment(HA_LEFT, VA_TOP);


    instructionText_ = ui->GetRoot()->CreateChild<Text>();
    String instructions = "To Orbit : Alt + LMB(Drag)\nTo Pan : Shift + Alt + LMB(Drag)\nZoom in : MouseWheel Forward or Key W";
    instructions += "\nZoom out : MouseWheel Backward or Key S\nFront View : Key 1\nBack View : Ctrl + Key 1";
    instructions += "\nRight View : Key 3\nLeft View : Ctrl + Key 3\nTop View : Key 7\nBottom View : Ctrl + Key 7";
    instructionText_->SetText(instructions);
    instructionText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    instructionText_->SetAlignment(HA_LEFT, VA_TOP);
    instructionText_->SetPosition(0, 20);

}

void [CLASS_PREFIX]::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();

    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0,viewport);
}

void [CLASS_PREFIX]::SetupOrbitingCam()
{
    camBaseNode_ = scene_->CreateChild("Base Cam Node");
    camYawNode_ = camBaseNode_->CreateChild("Cam Yaw Node");
    camPitchNode_ = camYawNode_->CreateChild("Cam Pitch Node");

    cameraNode_ = camPitchNode_->CreateChild("Cam Node");
    cameraNode_->CreateComponent<Camera>();

    cameraNode_->Translate(Vector3(0,0,-10));
}

void [CLASS_PREFIX]::UpdateOrbitingCam()
{
    Input* input = GetSubsystem<Input>();
    float dist;


    if(input->GetMouseButtonDown(MOUSEB_LEFT))
    {
        if(input->GetKeyDown(KEY_ALT) && input->GetKeyDown(KEY_SHIFT))
        {
            //Pan around the Screen
            dist = (cameraNode_->GetPosition().Length()) / 500;
            Matrix3 rotationMatrix = cameraNode_->GetWorldTransform().RotationMatrix();

            Vector3 direction = rotationMatrix * Vector3(input->GetMouseMoveX() * -dist, input->GetMouseMoveY() * dist,0);


            camBaseNode_->Translate(direction);

        }
        else if(input->GetKeyDown(KEY_ALT))
        {
            //Orbit at a pivot
            camPitchNode_->Pitch(input->GetMouseMoveY() * 0.25f);
            camYawNode_->Yaw(input->GetMouseMoveX() * 0.25f);

        }
    }
    else if(input->GetMouseMoveWheel() != 0)
    {
        cameraNode_->Translate(Vector3(0,0,input->GetMouseMoveWheel() * 0.5));
    }
    //In Forward and Backwards movement in case of laptop touchpad
    else if(input->GetKeyDown('W'))
    {
        cameraNode_->Translate(Vector3(0,0,0.07f));
    }
    else if(input->GetKeyDown('S'))
    {
        cameraNode_->Translate(Vector3(0,0,-0.07f));
    }
}

void [CLASS_PREFIX]::SetCameraPosition(CameraPosition pos)
{
    camPitchNode_->SetRotation(Quaternion(0.0f,0.0f,0.0f));
    camYawNode_->SetRotation(Quaternion(0.0f,0.0f,0.0f));

    switch(pos)
    {
    //case CAM_FRONT:
    //there is no need for this since the cam is already
    //positioned at CAM_FRONT after the orientation of the cam nodes are
    //reset

    case CAM_BACK:
        camYawNode_->Yaw(180);
        break;
    case CAM_RIGHT:
        camYawNode_->Yaw(-90);
        break;
    case CAM_LEFT:
        camYawNode_->Yaw(90);
        break;
    case CAM_UP:
        camPitchNode_->Pitch(-90);
        break;
    case CAM_DOWN:
        camPitchNode_->Pitch(90);
        break;

    }
}

void [CLASS_PREFIX]::OnKeyPressed(int key)
{
    Input* input = GetSubsystem<Input>();
    bool ctrlDown = input->GetKeyDown(KEY_CTRL);

    switch(key)
    {
    case '1':
        SetCameraPosition(ctrlDown ? CAM_BACK : CAM_FRONT);
        break;

    case '3':
        SetCameraPosition(ctrlDown ? CAM_LEFT : CAM_RIGHT);
        break;

    case '7':
        SetCameraPosition(ctrlDown ? CAM_UP :  CAM_DOWN );
        break;

    }

}

void [CLASS_PREFIX]::SubscribeToEvents()
{
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER([CLASS_PREFIX], HandleKeyDown));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER([CLASS_PREFIX], HandleUpdate));
}

URHO3D_DEFINE_APPLICATION_MAIN([CLASS_PREFIX])
