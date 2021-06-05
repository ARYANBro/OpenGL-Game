#pragma once

#include <chrono>

class DeltaTime
{
public:
	using Clock = std::chrono::high_resolution_clock;

public:
	void Calculate() 
	{
		auto now = Clock::now();
		m_DeltaTime = now - m_Previous;
		m_Previous = now;
	}

	operator float() const noexcept { return m_DeltaTime.count(); }

private:
	std::chrono::duration<double> m_DeltaTime;
	std::chrono::time_point<Clock> m_Previous;
};