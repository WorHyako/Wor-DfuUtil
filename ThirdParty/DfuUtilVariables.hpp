#pragma once

/**
 * See DfuUtilVariables.cpp
 */
extern "C" {

extern int verbose;
extern struct dfu_if *dfu_root;
extern char *match_path;
extern int match_vendor;
extern int match_product;
extern int match_vendor_dfu;
extern int match_product_dfu;
extern int match_config_index;
extern int match_iface_index;
extern int match_iface_alt_index;
extern int match_devnum;
extern const char *match_iface_alt_name;
extern const char *match_serial;
extern const char *match_serial_dfu;

}
