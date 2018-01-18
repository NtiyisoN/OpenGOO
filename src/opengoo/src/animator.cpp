#include "animator.h"

#include "GameEngine/entity.h"
#include "opengoo.h"

void Animator::ProcessRotateTransformFrame()
{
    auto& frame = mTransformFrames[mCurrentFrameTime];
    auto angle = frame.angle;
    if (frame.nextFrameIndex != -1)
    {
        if (frame.interpolation == KeyFrame::INTERPOLATION_LINEAR)
        {
            float deltaAngle = mTransformFrames[frame.nextFrameIndex].angle - angle;
            float deltaTime = mFrameTimes[frame.nextFrameIndex] - mFrameTimes[mCurrentFrameTime];
            assert(deltaTime != 0.0f);
            auto rotationSpeed = mSpeedIsNegative ? (deltaAngle / deltaTime) : -(deltaAngle / deltaTime);
            auto delata = rotationSpeed * (mCurrentTime - mFrameTimes[mCurrentFrameTime]);
            angle += delata;
        }
    }

    mTarget->GetGraphic()->SetAngle(angle);
}

void Animator::Update()
{
    std::for_each(mTransformTypes.begin(), mTransformTypes.end(),
                  [this](TransformType aType) { ProcessTransformFrame(aType); });

    mCurrentTime += GAME->GetDeltaTime() * 0.001f * mSpeed; // 1s = 1000ms
    auto nextFrameTime = mCurrentFrameTime + 1;
    if (nextFrameTime < mFrameTimes.size() && mCurrentTime < mFrameTimes[nextFrameTime])
    {
        return;
    }

    mCurrentFrameTime = nextFrameTime % mFrameTimes.size();
    if (mCurrentFrameTime == 0)
    {
        mCurrentTime = 0;
    }
}