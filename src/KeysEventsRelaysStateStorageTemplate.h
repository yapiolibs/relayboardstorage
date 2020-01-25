#pragma once

#include <inttypes.h>
#include <SettingsStorage.h>
#include "KeysEventsRelaysStateTemplate.h"
#include "main/static_settings.h"


#include "KeysEventsRelaysStateStorageTemplate.h"

#include <inttypes.h>
#include <WString.h>
#include <ArduinoJson.h>
#include "KeyEventRelayStrings.h"



template <class KeysEventsRelaysStateTemplate>
class KeysEventsRelaysStateStorageTemplate : public SettingsStorage {

public:
    KeysEventsRelaysStateStorageTemplate();

    //! loads relayboardsettings from given argument
    //! \param a
    void loadKeyRelayActions(const KeysEventsRelaysStateTemplate &a);

    //! writes relayboardsettings to given argument
    //! \param a
    void writeKeyRelayActions(KeysEventsRelaysStateTemplate &a) const;

    void resetDocument() {
        document.clear();
        is_document_empty = true;
    }

protected:
    int8_t loadSettings(const char *file_name) override;

    static void loadRelayActuation(const typename KeysEventsRelaysStateTemplate::KeyCodeElementType::ElementType &a, JsonVariant node);

    static void writeRelayActuation(typename KeysEventsRelaysStateTemplate::KeyCodeElementType::ElementType &a, const JsonVariant &node);

    static void loadKeyEventRelayAction(const typename KeysEventsRelaysStateTemplate::KeyCodeElementType &a, JsonVariant node);

    static void writeKeyEventRelayAction(typename KeysEventsRelaysStateTemplate::KeyCodeElementType &a, const JsonVariant &node);

    static void loadKeysName(const typename KeysEventsRelaysStateTemplate::KeyNamesArrayType &names, JsonVariant node);

    static void writeKeysName(typename KeysEventsRelaysStateTemplate::KeyNamesArrayType &names, const JsonVariant &node);

    static void loadRelaysName(const typename KeysEventsRelaysStateTemplate::RelaysNameArrayType &names, JsonVariant node);

    static void writeRelaysName(typename KeysEventsRelaysStateTemplate::RelaysNameArrayType &names, const JsonVariant &node);

    static void loadRelaysFlag(const uint16_t &flags, JsonVariant node);

    static void writeRelaysFlag(uint16_t &flags, const JsonVariant &node);

    static void writePersistFlag(bool &flag, const JsonVariant &node);

    static void loadPersistFlag(const bool &flag, JsonVariant node);

    bool is_document_empty{true};
};


template <class T>
KeysEventsRelaysStateStorageTemplate<T>::KeysEventsRelaysStateStorageTemplate() : SettingsStorage("/conf/key_relay_conf.json") {}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::loadKeyRelayActions(const KeysEventsRelaysStateTemplate &a) {
    JsonVariant root = getDocumentRoot();
    JsonVariant keys_actions = root.getOrAddMember("ka");
    JsonVariant persist = root.getOrAddMember("persist");
    JsonVariant names = root.getOrAddMember("names");
    JsonVariant relays_flag = root.getOrAddMember("saved_state");
    JsonVariant keys_name = names.getOrAddMember("k");
    JsonVariant relays_name = names.getOrAddMember("r");

    loadPersistFlag(a.persist_state, persist);
    loadRelaysFlag(a.relay_flags, relays_flag);
    loadKeysName(a.keys_name, keys_name);
    loadRelaysName(a.relays_name, relays_name);

    uint8_t i = 0;
    for (const auto &r : a.key_code) {
        loadKeyEventRelayAction(r, keys_actions.getOrAddMember(String(i++)));
    }
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
        loadKeyEventRelayAction(const typename KeysEventsRelaysStateTemplate::KeyCodeElementType &a, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &e : a.event_type) {
        loadRelayActuation(e, node.getOrAddMember(String(i++)));
    }
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
        loadRelayActuation(const typename KeysEventsRelaysStateTemplate::KeyCodeElementType::ElementType &a, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &r : a.relay_actuation) {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull()) {
            node.add(Relay::uint8FromActuation(r));
        } else {
            array_element.set(Relay::uint8FromActuation(r));
        }
    }
}

template <class T>
int8_t KeysEventsRelaysStateStorageTemplate<T>::loadSettings(const char *file_name) {
    Serial.println("KeyRelaySettingsStorage::loadSettings: ...");
    uint8_t result = SettingsStorage::loadSettings(file_name);
    if (0 == result) {
        is_document_empty = false;
        Serial.println("KeyRelaySettingsStorage::loadSettings: failed to load document");
    }
    return result;
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
        loadKeysName(const typename KeysEventsRelaysStateTemplate::KeyNamesArrayType &names, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &name : names) {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull()) {
            node.add(name);
        } else {
            array_element.set(name);
        }
    }
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
loadRelaysName(const typename KeysEventsRelaysStateTemplate::RelaysNameArrayType &names, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &name : names) {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull()) {
            node.add(name);
        } else {
            array_element.set(name);
        }
    }
}

template <class T>
void KeysEventsRelaysStateStorageTemplate<T>::loadPersistFlag(const bool &flag, JsonVariant node) {
    node.set(flag);
}

template <class T>
void KeysEventsRelaysStateStorageTemplate<T>::loadRelaysFlag(const uint16_t &flags, JsonVariant node) {
    node.set(flags);
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
        writeKeyRelayActions(KeysEventsRelaysStateTemplate &a) const {

    if (is_document_empty) {
        Serial.println(
                "KeyRelaySettingsStorage::writeKeyRelayActions: failed to write relayboardsettings since no document is loaded");
        return;
    }

    JsonVariant root = getDocumentRoot();
    JsonVariant keys_actions = root.getOrAddMember("ka");
    JsonVariant persist = root.getMember("persist");
    JsonVariant names = root.getMember("names");
    JsonVariant relays_flag = root.getMember("saved_state");
    JsonVariant keys_name = names.getMember("k");
    JsonVariant relays_name = names.getMember("r");

    writePersistFlag(a.persist_state, persist);
    writeRelaysFlag(a.relay_flags, relays_flag);
    writeKeysName(a.keys_name, keys_name);
    writeRelaysName(a.relays_name, relays_name);

    uint8_t i = 0;
    for (auto &r : a.key_code) writeKeyEventRelayAction(r, keys_actions.getOrAddMember(String(i++)));

}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
        writeKeysName(typename KeysEventsRelaysStateTemplate::KeyNamesArrayType &names, const JsonVariant &node) {
    uint8_t i = 0;

    if (!node.isNull())
        for (String &name : names) {//KeyNamesElementType
            JsonVariant value = node.getElement(i++);
            if (!value.isNull()) {
                name = value.as<String>();
            }
        }
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
        writeRelaysName(typename KeysEventsRelaysStateTemplate::RelaysNameArrayType &names, const JsonVariant &node) {
    uint8_t i = 0;

    for (auto &name : names) {
        JsonVariant value = node.getElement(i++);
        if (!value.isNull()) {
            name = value.as<String>();
        } else {
            name = i;
        }
    }
}

template <class T>
void KeysEventsRelaysStateStorageTemplate<T>::writePersistFlag(bool &flag, const JsonVariant &node) {
    if (!node.isNull())
        flag = node.as<uint16_t>();
}

template <class T>
void KeysEventsRelaysStateStorageTemplate<T>::writeRelaysFlag(uint16_t &flags, const JsonVariant &node) {
    if (!node.isNull())
        flags = node.as<uint16_t>();
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
writeKeyEventRelayAction(typename KeysEventsRelaysStateTemplate::KeyCodeElementType &a, const JsonVariant &node) {
    uint8_t i = 0;
    for (auto &e : a.event_type) writeRelayActuation(e, node.getMember(String(i++)));
}

template <class KeysEventsRelaysStateTemplate>
void KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysStateTemplate>::
writeRelayActuation(typename KeysEventsRelaysStateTemplate::KeyCodeElementType::ElementType &a, const JsonVariant &node) {
    uint8_t i = 0;

    if (!node.isNull())
        for (auto &r : a.relay_actuation) {
            JsonVariant value = node.getElement(i++);
            if (!value.isNull()) {
                r = Relay::actuationFromUint8(value.as<uint8_t>());
            }
        }
}
