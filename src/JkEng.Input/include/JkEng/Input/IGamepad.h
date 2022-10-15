#pragma once

namespace JkEng::Input
{
    class IGamepad
    {
    public:
        virtual ~IGamepad() = default;

        virtual const std::string DisplayName() = 0;

        virtual bool IsConnected() = 0;

        virtual bool RightFaceButton() = 0;
        virtual bool LeftFaceButton() = 0;
        virtual bool TopFaceButton() = 0;
        virtual bool BottomFaceButton() = 0;

        virtual bool DPadRight() = 0;
        virtual bool DPadLeft() = 0;
        virtual bool DPadUp() = 0;
        virtual bool DPadDown() = 0;

        virtual bool LeftStickButton() = 0;
        virtual bool RightStickButton() = 0;

        virtual bool LeftTriggerButton() = 0;
        virtual bool RightTriggerButton() = 0;

        virtual bool LeftBumperButton() = 0;
        virtual bool RightBumperButton() = 0;

        virtual bool SelectButton() = 0;
        virtual bool StartButton() = 0;

        virtual float LeftStickX() = 0;
        virtual float LeftStickY() = 0;

        virtual float RightStickX() = 0;
        virtual float RightStickY() = 0;
    };
}
