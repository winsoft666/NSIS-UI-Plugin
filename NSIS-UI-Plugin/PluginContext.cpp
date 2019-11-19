#include "stdafx.h"
#include "PluginContext.h"

PluginContext::PluginContext() :
    m_parentHwnd(NULL)
    , m_pluginParms(NULL) {

}

PluginContext *PluginContext::Instance() {
    static PluginContext *context = NULL;
    if (!context) {
        context = new PluginContext();
    }
    return context;
}


PluginContext::~PluginContext() {

}

void PluginContext::SetExtraParameters(ExtraParameters *param) {
    m_pluginParms = param;
}

ExtraParameters *PluginContext::GetExtraParameters() {
    return m_pluginParms;
}

void PluginContext::SetParentHwnd(HWND h) {
    m_parentHwnd = h;
}

HWND PluginContext::parentHwnd() const {
    return m_parentHwnd;
}

void PluginContext::BindInstallEvent(const tstring &eventName, long nsisFuncAddress) {
    m_installEventBindMap[eventName] = nsisFuncAddress;
}

void PluginContext::BindButtonClickedEvent(const tstring &eventName, long nsisFuncAddress) {
    m_buttonClickedEventBindMap[eventName] = nsisFuncAddress;
}

bool PluginContext::ExecuteNsisFunction(long funcAddress) {
    if (m_pluginParms) {
        m_pluginParms->ExecuteCodeSegment(funcAddress, NULL);
        return true;
    }
    return false;
}

bool PluginContext::ExecuteInstallEventFunction(const tstring &installEvent) {
    if (m_pluginParms && m_installEventBindMap.find(installEvent) != m_installEventBindMap.end()) {
        m_pluginParms->ExecuteCodeSegment(m_installEventBindMap[installEvent] - 1, NULL);
        return true;
    }
    return false;
}

bool PluginContext::ExecuteButtonClickedEventFunction(const tstring &buttonName) {
    if (m_pluginParms && m_buttonClickedEventBindMap.find(buttonName) != m_buttonClickedEventBindMap.end()) {
        m_pluginParms->ExecuteCodeSegment(m_buttonClickedEventBindMap[buttonName] - 1, NULL);
        return true;
    }
    return false;
}
