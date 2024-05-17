#include "pch.hpp"

#include "Firmware.hpp"
#include "FrameworkStatus.hpp"

#include <cstdio>
#include <filesystem>
#include <fcntl.h>
#include <sstream>

using namespace WorDfuUtil;

namespace {

    /**
     * @brief   Handle to current LibUsb session.
     */
    libusb_context *ctx;

    /**
     * @brief   Stores current framework state. Uses in each Firmware method to check LibUsb and dfu-util state.
     *
     * @see     WorDfuUtils::FrameworkStatus
     */
    std::uint8_t frameworkStatus = static_cast<std::uint8_t>(FrameworkStatus::Deinited);

    /**
     * @brief   Just short checking to <code>dfu_root</code> to <code>nullptr</code>.
     *
     * @return  <code>true</code>   Dfu device found and parsed to <code>dfu_root</code> variable.
     *          <p/>
     *          <code>false</code>  Can't find dfu device. Try to run <code>probe_device(libusb_context*)</code> again.
     *
     * @author  WorHyako
     */
    [[nodiscard]]
    bool isDfuDeviceFound() noexcept {
        std::printf("Cannot find DFU device.\n");
        return ::dfu_root != nullptr;
    }

    /**
     * @brief   Checks connected dfu device to correct status.
     *
     * @return  <code>true</code>   Dfu device ready to operations.
     *          <p/>
     *          <code>false</code>  Incorrect dfu device status.
     *
     * @author  WorHyako
     */
    [[nodiscard]]
    bool checkDfuStatus() {
        if (!::isDfuDeviceFound()) {
            return false;
        }
        dfu_status dfuStatus {};
        const int ec = dfu_get_status(::dfu_root, &dfuStatus);
        if (ec < 0) {
            std::printf("Cannot check device status: %s\n", libusb_error_name(ec));
            return false;
        }
        if (dfuStatus.bStatus != 0 || dfuStatus.bState != DFU_STATE_dfuIDLE) {
            std::printf("Bad DFU device status: %s\n", dfu_status_to_string(dfuStatus.bStatus));
            std::printf("Bad DFU device state: %s\n", dfu_state_to_string(dfuStatus.bState));
            return false;
        }
        return true;
    }

    /**
     * @brief   Deinitializes dfu-util and LibUsb frameworks, considering frameworks condition.
     *          <p/>
     *          Function must be called after each <code>prepareFrameworks()</code>.
     *
     * @author  WorHyako
     */
    void deinitFrameworks() noexcept {
        if (::frameworkStatus == static_cast<std::uint8_t>(FrameworkStatus::Deinited)) {
            return;
        }

        if (::frameworkStatus & static_cast<std::uint8_t>(FrameworkStatus::DeviceOpened)) {
            libusb_close(::dfu_root->dev_handle);
        }

        if (::frameworkStatus & static_cast<std::uint8_t>(FrameworkStatus::DfuDeviceFound)) {
            disconnect_devices();
        }

        if (::frameworkStatus & static_cast<std::uint8_t>(FrameworkStatus::Inited)) {
            libusb_exit(::ctx);
        }
    }

    /**
     * @brief   Prepares LibUsb and dfu-util frameworks to upload firmware.
     *          <p/>
     *          Don't forget to call <code>::deinitFrameworks()</code> after this function.
     *
     * @return  <code>true</code>   Founded device ready to upload firmware.
     *          <p/>
     *          <code>false</code>  Fail in framework setting or DFU device.
     *
     * @author  WorHyako
     */
    bool prepareFrameworks() {
        int ec = libusb_init(&::ctx);
        if (ec < 0) {
            std::printf("LibUsb initialization error - %s.\n", libusb_error_name(ec));
            return false;
        }
        ::frameworkStatus |= static_cast<std::uint8_t>(FrameworkStatus::Inited);

        probe_devices(::ctx);
        if (!::isDfuDeviceFound()) {
            return false;
        }
        ::frameworkStatus |= static_cast<std::uint8_t>(FrameworkStatus::DfuDeviceFound);

        ec = libusb_open(::dfu_root->dev, &::dfu_root->dev_handle);
        if (ec < 0) {
            std::printf("Cannot open device - %s.\n", libusb_error_name(ec));
            return false;
        }
        ::frameworkStatus |= static_cast<std::uint8_t>(FrameworkStatus::DeviceOpened);

        return ::checkDfuStatus();
    }
}

bool Firmware::Upload(const std::string &filePath, int transferLimit, int transferSize) noexcept {
    if (!::prepareFrameworks()) {
        ::deinitFrameworks();
        return false;
    }

    if (std::filesystem::exists(filePath)) {
        std::ignore = std::filesystem::remove(filePath);
    }

    const int fileDescriptor = open(filePath.c_str(), O_WRONLY | O_BINARY | O_CREAT | O_EXCL | O_TRUNC, 0666);
    if (fileDescriptor < 0) {
        std::printf("Cannot create and open file %s for writing.\n", filePath.c_str());
        ::deinitFrameworks();
        return false;
    }

    std::stringstream dfuUtilOptions;
    dfuUtilOptions << "-s 0x08000000";
    dfuUtilOptions << ":" << transferLimit;

    if (transferSize < 0) {
        transferSize = libusb_le16_to_cpu(::dfu_root->func_dfu.wTransferSize);
        if (transferSize == 0) {
            std::printf("Device return zero transfer size. Specify it manually.\n");
            close(fileDescriptor);
        }
        if (transferSize < ::dfu_root->bMaxPacketSize0) {
            transferSize = ::dfu_root->bMaxPacketSize0;
        }
    }

    const int ec = dfuse_do_upload(::dfu_root,
                                   transferSize,
                                   fileDescriptor,
                                   dfuUtilOptions.str().c_str());
    close(fileDescriptor);
    if (ec < 0) {
        std::printf("Error in firmware uploading.\n");
    }

    ::deinitFrameworks();
    return true;
}
