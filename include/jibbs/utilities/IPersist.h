#pragma once

struct IQSettingPersist
{
    virtual void saveSettings() = 0;
    virtual void loadSettings() = 0;
};
