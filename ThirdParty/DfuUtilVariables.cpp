#include "DfuUtilVariables.hpp"

#include <cstddef>

/**
 * Global variables, which are required by dfu-util.
 * <p/>
 * I'll try to container it in future.
 */
int verbose = 0;
struct dfu_if *dfu_root = nullptr;
char *match_path = nullptr;
int match_vendor = -1;
int match_product = -1;
int match_vendor_dfu = -1;
int match_product_dfu = -1;
int match_config_index = -1;
int match_iface_index = -1;
int match_iface_alt_index = -1;
int match_devnum = -1;
const char *match_iface_alt_name = nullptr;
const char *match_serial = nullptr;
const char *match_serial_dfu = nullptr;
