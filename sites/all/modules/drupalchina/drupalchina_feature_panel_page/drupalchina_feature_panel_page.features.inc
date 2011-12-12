<?php
/**
 * @file
 * drupalchina_feature_panel_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalchina_feature_panel_page_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}
