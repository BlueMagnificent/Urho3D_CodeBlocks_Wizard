#ifndef [PROJECT_HDR]_H
#define [PROJECT_HDR]_H

class [CLASS_PREFIX] : public Urho3D::Application
{

	URHO3D_OBJECT([CLASS_PREFIX], Application)
	
public:
    [CLASS_PREFIX](Urho3D::Context* context);

    virtual void Setup();
    virtual void Start();
    virtual void Stop();

    void HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};

#endif // [PROJECT_HDR]_H