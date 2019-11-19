#pragma once

class PluginContext {
  public:
    static PluginContext *Instance();
    ~PluginContext();
    void SetExtraParameters(ExtraParameters *param);
    ExtraParameters* GetExtraParameters();

    void SetParentHwnd(HWND h);
    HWND parentHwnd() const;

    void BindInstallEvent(const tstring &eventName, long nsisFuncAddress);
    void BindButtonClickedEvent(const tstring &buttonName, long nsisFuncAddress);

    bool ExecuteNsisFunction(long funcAddress);

    bool ExecuteInstallEventFunction(const tstring &installEvent);
    bool ExecuteButtonClickedEventFunction(const tstring &buttonName);
  private:
    PluginContext();

    HWND m_parentHwnd;
    ExtraParameters *m_pluginParms;
    std::map<tstring, long> m_installEventBindMap;
    std::map<tstring, long> m_buttonClickedEventBindMap;
};
