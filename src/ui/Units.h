//
// Created by Tobiathan on 1/15/23.
//

#pragma once

namespace tui {
    struct percent {
        float value;
    };

    constexpr percent operator"" _pct(long double value) {
        return percent { static_cast<float>(value) };
    }

    constexpr percent operator"" _pct(unsigned long long value) {
        return percent { static_cast<float>(value) };
    }


    struct fraction {
        float value;
    };

    constexpr fraction operator"" _fr(long double value) {
        return fraction { static_cast<float>(value) };
    }

    constexpr fraction operator"" _fr(unsigned long long value) {
        return fraction { static_cast<float>(value) };
    }
}
