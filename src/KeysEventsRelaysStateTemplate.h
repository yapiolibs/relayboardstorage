#pragma once

#include <type_traits>
#include <array>

#include <HardwareSerial.h>

#include <KeyEvent.h>
#include <RelayTypes.h>

template<typename Element, uint8_t elements_count>
struct ArrayTypeTemplate {
    using ElementType = Element;
    using ArrayType = std::array<Element, elements_count>;
};

template<uint8_t relays_count>
struct RelayActuationTemplate {
    using actuation_type = ArrayTypeTemplate<Relay::Actuation, relays_count>;
    using ElementType = typename actuation_type::ElementType;
    using ArrayType = typename actuation_type::ArrayType;

    ArrayType relay_actuation{{static_cast<Relay::Actuation>(0)}};
};

template<uint8_t key_event_types_count, uint8_t relays_count>
struct EventTypeRelayActionTemplate {
    using event_type_type = ArrayTypeTemplate<RelayActuationTemplate<relays_count>, key_event_types_count>;
    using ElementType = typename event_type_type::ElementType;
    using ArrayType = typename event_type_type::ArrayType;

    ArrayType event_type{{}};
};

template<uint8_t keys_count, uint8_t key_event_types_count, uint8_t relays_count>
struct KeyEventRelayActionTemplate {
    using key_code_type = ArrayTypeTemplate<EventTypeRelayActionTemplate<key_event_types_count, relays_count>, keys_count>;
    using KeyCodeElementType = typename key_code_type::ElementType;
    using KeyCodeArrayType = typename key_code_type::ArrayType;

    KeyCodeArrayType key_code{{}};

};

template<uint8_t keys_count, uint8_t key_event_types_count, uint8_t relays_count>
struct KeyEventRelayStateTemplate : public KeyEventRelayActionTemplate<keys_count, key_event_types_count, relays_count> {
    using keys_names_type = ArrayTypeTemplate<String, keys_count>;
    using KeyNamesElementType = typename keys_names_type::ElementType;
    using KeyNamesArrayType = typename keys_names_type::ArrayType;

    using relays_names_type = ArrayTypeTemplate<String, relays_count>;
    using RelaysNameElementType = typename relays_names_type::ElementType;
    using RelaysNameArrayType = typename relays_names_type::ArrayType;

    bool persist_state{true};
    uint16_t relay_flags{0};
    KeyNamesArrayType keys_name{{"k"}};
    RelaysNameArrayType relays_name{{"r"}};
};

template<uint8_t keys_count, uint8_t key_event_types_count, uint8_t relays_count> using KeysEventsRelaysStateTemplate =
KeyEventRelayStateTemplate<keys_count, key_event_types_count, relays_count>;