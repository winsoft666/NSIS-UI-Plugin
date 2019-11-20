#pragma once

class SetupPageInterface {
public:
    virtual tstring GetInstallDirectory() = 0;
    virtual void SetInstallStepDescription(const tstring &description, int progressValue = -1) = 0;
};