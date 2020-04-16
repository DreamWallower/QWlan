#include "QHotspot.h"
#include <QPair>
#include <winrt/Windows.Networking.Connectivity.h>
#include <winrt/Windows.Networking.NetworkOperators.h>

using namespace winrt;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Networking::NetworkOperators;

#pragma comment(lib, "windowsapp")

QHotspot::QHotspot(QObject* parent)
    : QObject(parent) {
}

QHotspot::~QHotspot() {
    init_apartment(apartment_type::single_threaded); // apartment_type::multi_threaded
    auto connectionProfile{NetworkInformation::GetInternetConnectionProfile()};
    auto tetheringManager{NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile)};

    if (TetheringOperationalState::Off != tetheringManager.TetheringOperationalState()) {
        auto ioAsync = tetheringManager.StopTetheringAsync();
    }
    clear_factory_cache();
    uninit_apartment();
}

// !!!WORK ON WINDOWS10 ONLY!!!
bool QHotspot::init() {
    // 查询上次设置的wifi名称和密码
    // 返回当前的状态，     TRUE - 热点已经开了
    //                   FALSE - 热点关闭状态
    bool state;
    init_apartment(apartment_type::single_threaded); // apartment_type::multi_threaded
    auto connectionProfile{NetworkInformation::GetInternetConnectionProfile()};
    auto tetheringManager = NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
    auto configuration{tetheringManager.GetCurrentAccessPointConfiguration()};
    state = (TetheringOperationalState::On == tetheringManager.TetheringOperationalState());
    m_originSsid = QString::fromWCharArray(configuration.Ssid().c_str());
    m_originPassword = QString::fromWCharArray(configuration.Passphrase().c_str());
    clear_factory_cache();
    uninit_apartment();
    return state;
}

void QHotspot::config(const QString& ssid, const QString& password) {
    // 设置wifi名称和密码
    init_apartment(apartment_type::single_threaded); // apartment_type::multi_threaded
    auto connectionProfile{NetworkInformation::GetInternetConnectionProfile()};
    auto tetheringManager = NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
    NetworkOperatorTetheringAccessPointConfiguration config;
    config.Passphrase(hstring(reinterpret_cast<const wchar_t*>(ssid.utf16())));
    config.Ssid(hstring(reinterpret_cast<const wchar_t*>(password.utf16())));

    m_originSsid = ssid;
    m_originPassword = password;
    auto ioAsync = tetheringManager.ConfigureAccessPointAsync(config);
    clear_factory_cache();
    uninit_apartment();
}

QList<QPair<QString, QString>> QHotspot::getClient() {
    // 返回连接热点的设备
    QList<QPair<QString, QString>> clientList;
    init_apartment(apartment_type::single_threaded); // apartment_type::multi_threaded
    auto connectionProfile{NetworkInformation::GetInternetConnectionProfile()};
    auto tetheringManager = NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);
    auto list = tetheringManager.GetTetheringClients();

    for (auto member : list) {
        // 得到已连接设备名
        auto hostNames = member.HostNames().GetAt(0).ToString();
        // 得到已连接设备的mac地址
        auto MacAddress = member.MacAddress();
        clientList.push_back(qMakePair(QString::fromWCharArray(hostNames.c_str()), QString::fromWCharArray(MacAddress.c_str())));
    }
    clear_factory_cache();
    uninit_apartment();
    return clientList;
}

void QHotspot::start() {
    // 开启热点
    emit starting();
    init_apartment(); // apartment_type::multi_threaded
    auto connectionProfile{NetworkInformation::GetInternetConnectionProfile()};
    auto tetheringManager = NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile);

    if (TetheringOperationalState::Off == tetheringManager.TetheringOperationalState()) {
        auto ioAsync = tetheringManager.StartTetheringAsync();
        auto fResult = ioAsync.get();
    }
    clear_factory_cache();
    uninit_apartment();
    emit started();
}

void QHotspot::stop() {
    // 关闭热点
    emit stopping();
    init_apartment(); // apartment_type::multi_threaded
    auto connectionProfile{NetworkInformation::GetInternetConnectionProfile()};
    auto tetheringManager{NetworkOperatorTetheringManager::CreateFromConnectionProfile(connectionProfile)};

    if (TetheringOperationalState::On == tetheringManager.TetheringOperationalState()) {
        auto ioAsync = tetheringManager.StopTetheringAsync();
        auto fResult = ioAsync.get();
    }
    clear_factory_cache();
    uninit_apartment();
    emit stopped();
}