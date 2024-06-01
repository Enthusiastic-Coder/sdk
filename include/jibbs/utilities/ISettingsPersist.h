#pragma once

struct ISettingsPersist
{
    virtual void saveSettings() = 0;
    virtual void loadSettings() = 0;
};
