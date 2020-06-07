#pragma once

#include <memory>
#include <mutex>

template<typename T>
class Singleton {
protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static std::shared_ptr<T> m_instance;
	static std::once_flag m_onceFlag;
public:
	static T* GetInstance()
	{
		std::call_once( m_onceFlag, []
		{
			m_instance.reset( new T );
		} );

		return m_instance.get();
	}
};

template<typename T>
std::shared_ptr<T> Singleton<T>::m_instance = nullptr;

template<typename T>
std::once_flag Singleton<T>::m_onceFlag;
