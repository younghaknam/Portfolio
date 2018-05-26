#pragma once

template <typename T>
class Singleton
{
public:
	Singleton() {}
	virtual ~Singleton() {}

	static T* GetSingleton()
	{ 
		if (singleton_ == nullptr)
			singleton_ = new T;

		return singleton_;
	}

	static void Release()
	{
		if (singleton_ == nullptr)
			return;

		delete singleton_;
		singleton_ = nullptr;
	}

private:
	static T* singleton_;
};

template <typename T> T* Singleton<T>::singleton_ = nullptr;
