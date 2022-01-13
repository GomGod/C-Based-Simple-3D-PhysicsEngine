#ifndef SIMPLEPHYSICSENGINE_OBJECTKEY_H
#define SIMPLEPHYSICSENGINE_OBJECTKEY_H

#include<iostream>

enum class ObjectType
{
	SPHERE
};

struct ObjectKey
{
public:
	ObjectType oType;
	float* infos;
	
	bool operator == (ObjectKey& key)
	{
		int mCount{ 0 };

		if (key.oType != oType)
			return false;

		switch (oType)
		{
		case ObjectType::SPHERE:
			mCount = 3;
			break;
		default:
			break;
		}

		for (auto i = 0; i < mCount; i += 1)
		{
			infos + i == key.infos + i;
		}

		return false;
	};

};

class ObjectKeyHahser
{
	size_t operator()(const ObjectKey& key) const
	{
		size_t ret;

		int mCount{ 0 };

		switch (key.oType)
		{
		case ObjectType::SPHERE:
			mCount = 3;
			break;
		default:
			break;
		}

		for (auto i = 0; i < mCount; i += 1)
		{
			float value{ key.infos[i] };
			ret ^= std::hash<float>(1.0f);
		}

		return false;
	}
};

#endif