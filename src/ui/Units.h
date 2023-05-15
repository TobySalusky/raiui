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

    struct view_width_pct {
        float value;
    };

    constexpr view_width_pct operator"" _vw(long double value) {
        return view_width_pct { static_cast<float>(value) };
    }

    constexpr view_width_pct operator"" _vw(unsigned long long value) {
        return view_width_pct { static_cast<float>(value) };
    }

    struct view_height_pct {
        float value;
    };

    constexpr view_height_pct operator"" _vh(long double value) {
        return view_height_pct { static_cast<float>(value) };
    }

    constexpr view_height_pct operator"" _vh(unsigned long long value) {
        return view_height_pct { static_cast<float>(value) };
    }
}
