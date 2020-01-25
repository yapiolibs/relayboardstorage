#include "KeyEventRelayStrings.h"

namespace relay {
    namespace names {
        const char *RelayFlagsKeys::ACTIVATION_FLAGS = "activation_flags";

        const char *RelayKeys::RELAY = "relay_actuation";

        namespace KeyEventRelaysAction {
            char SCOPE[]{"KeyEventRelaysAction"};
            char KEY_CODE[]{"key_code"};

            namespace RelaysAction {
                char SCOPE[]{"RelaysAction"};
                char KEY_EVENT_TYPE[]{"event_type"};
                char RELAYS_ACTION[]{"relay_actuation"};
            }
        }
    }
}