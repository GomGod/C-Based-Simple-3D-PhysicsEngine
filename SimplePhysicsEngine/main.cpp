#include"main.h"

int main(int argc, char* argv[])
{
	PhysicsWorld* pWorld{new PhysicsWorld()};
	utils::Object* testObj = new utils::Object(utils::Vector3{ 0,0,0 }, utils::Vector3{ 0,0,0 }, utils::Vector3{ 2,0,3 }, 1);	
	pWorld->InitPhysics();
	pWorld->AddObject(testObj);
	

	cout << pWorld->GetObjectsNumber() << "\n";
	for (int i = 0; i < 300; i += 1)
	{		
		cout << testObj->Position << "\n";
	}	
	pWorld->RemoveObject(testObj);
	cout << pWorld->GetObjectsNumber() << "\n";
	

	return 0;
}
