#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>

#include <Urho3D/Input/InputEvents.h>

#include "[CLASS_PREFIX].h"

[CLASS_PREFIX]::[CLASS_PREFIX](Urho3D::Context *context) : Application(context)
{
}

void [CLASS_PREFIX]::Setup()
{
	engineParameters_["ResourcePaths"] = "CoreData;Data";
    engineParameters_["LogName"]   = "[CLASS_PREFIX].log";
}

void [CLASS_PREFIX]::Start()
{
    SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER([CLASS_PREFIX], HandleKeyDown));
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
}


URHO3D_DEFINE_APPLICATION_MAIN([CLASS_PREFIX])
