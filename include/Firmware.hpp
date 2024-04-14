#pragma once

#include "FrameworkStatus.hpp"

#include <string>

class dfu_if;
class dfu_file;

namespace WorDfuUtils {

    /**
     * @brief   Object to upload/download(in future) firmware from/to usb device in DFU mode.
     *          <p/>
     *          <code>Firmware</code> can't switch usb device to DFU mode automatically.
     *          Switch device to DFU mode manually and run <code>Firmware</code> methods then.
     *          <p/>
     *          Current appearance of this class is temporary and has been wrote for article.
     *          This class and whole API will changed in future.
     *
     * @usage
     * @code
     *          Firmware firmware;
     *          std::string filePath = "pathOrNameToBinaryFile.bin";
     *          int transferLimit = 150'000;
     *          int transferSize = 1024;
     *          auto uploadingResult = firmware.Upload(filePath, transferLimit, transferSize);
     *          if (uploadingResult) {
     *              std::printf("Smth was ducked up.\n");
     *              . . .
     *          }
     * @endcode
     *
     * @author  WorHyako
     */
    class Firmware final {
    public:
        /**
         * @brief Ctor.
         */
        Firmware() noexcept;

        /**
         * @brief Dtor.
         */
        ~Firmware() = default;

        /**
         * @brief   Upload firmware from usb device to local binary file if it possible.
         *          Device must be in DFU mode already.
         *          <p/>
         *          Overwrite existing file.
         *
         * @param filePath      Path to created binary file. Path may store absolute or relative format.
         *
         * @param transferLimit Max size for transfer process.
         *
         * @param transferSize  Size of one iteration
         *
         * @return  <code>true</code>   Firmware was successful loaded to local binary file.
         *          <p/>
         *          <code>false</code>  Fail in firmware loading.
         */
        [[nodiscard]]
        bool Upload(const std::string& filePath, int transferLimit, int transferSize = -1) noexcept;

    private:
        /**
         * @brief   Handle to dfu device.
         *          <p/>
         *          In current version it's just example to duplication global variable <code>dfu_root</code>.
         */
        [[maybe_unused]]
        [[deprecated]]
        dfu_if* dfuDevice;

        /**
         * @brief   Field contains data about firmware:
         *          file name, vid/pid pointer, pointer to file loaded into memory and etc.
         */
        [[maybe_unused]]
        [[deprecated]]
        dfu_file* firmwareFile;
    };
}
