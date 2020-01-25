#pragma once

#include <inttypes.h>

namespace relay {
    namespace names {
        struct RelayFlagsKeys {
            static const char *ACTIVATION_FLAGS;
        };

        struct RelayKeys {
            static const char *RELAY;
        };
        namespace KeyEventRelaysAction {
            extern char SCOPE[];
            extern char KEY_CODE[];

            namespace RelaysAction {
                extern char SCOPE[];
                extern char KEY_EVENT_TYPE[];
                extern char RELAYS_ACTION[];
            }
        }
    }
}
