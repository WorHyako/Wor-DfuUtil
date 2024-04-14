#pragma once

#include <cstdint>

namespace WorDfuUtils {

    /**
     * @brief   Describes framework status.
     *          <p/>
     *          Describe both of LibUsb and dfu-util condition.
     *
     * @author  WorHyako
     */
    enum class FrameworkStatus
            : std::uint8_t {
        /**
         * Zero-check state.
         */
        Deinited = 0b0,

        /**
         * LibUsb framework has been initialized.
         */
        Inited = 0b1,

        /**
         * Device port has been opened by LibUsb framework.
         */
        DeviceOpened = 0b10,

        /**
         * Dfu device was found by dfu-util framework and wrote to <code>dfu_root</code> variable.
         */
        DfuDeviceFound = 0b100,

        /**
         * Device interface has been claimed by LibUsb framework.
         */
        InterfaceClaimed = 0b1000,

        /**
         * Alternative interface has been set for current device.
         */
        AltInterfaceSelected = 0b10000,
    };
}
