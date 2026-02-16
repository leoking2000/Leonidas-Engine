#pragma once
#include <chrono>
#include "LeoTypes.h"

namespace leo
{
    class Timer
    {
    public:
        Timer() { Reset(); }

        void Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        f32 ElapsedSec() const
        {
            std::chrono::duration<f32> elapsed = std::chrono::high_resolution_clock::now() - m_Start;
            return elapsed.count();
        }

        f32 ElapsedMillis() const
        {
            std::chrono::duration<f32, std::milli> elapsed = std::chrono::high_resolution_clock::now() - m_Start;
            return elapsed.count();
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

    class FrameTimer
    {
    public:
        FrameTimer()
        {
            Reset();
        }

        // Call at the start of each frame to update delta
        void Tick()
        {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<f32> frameElapsed = now - m_LastTime;
            m_DeltaTime = frameElapsed.count();
            m_LastTime = now;

            m_TotalTime += m_DeltaTime;
            m_FrameCount++;
        }

        // Reset timer to zero
        void Reset()
        {
            auto now = std::chrono::high_resolution_clock::now();
            m_LastTime = now;
            m_DeltaTime = 0.0f;
            m_TotalTime = 0.0f;
            m_FrameCount = 0;
        }

        f32 FPS() const { return 1.0f / m_DeltaTime; }

        // Returns delta time in seconds for the last frame
        f32 DeltaTime() const { return m_DeltaTime; }

        // Returns total time since Reset() in seconds
        f32 TotalTime() const { return m_TotalTime; }

        // Returns number of frames since Reset()
        u64 FrameCount() const { return m_FrameCount; }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
        f32 m_DeltaTime = 0.0f;
        f32 m_TotalTime = 0.0f;
        u64 m_FrameCount = 0;
    };
}
